#include <cassert>
#include <iostream>
#include "engine.h"
#include "solver_pvs.h"
#include "eval_basic.h"
#include "movelist.h"
#include "board.h"

using namespace std;

namespace eia_v0_5
{
    SolverPVS::SolverPVS(Engine * engine) : Solver(engine)
    {
        history = new History;
        hash_stats = new HashStats;
        H = new Hash(hash_stats);
        B = new Board(states);
        E = new EvalBasic;
    }

    SolverPVS::~SolverPVS()
    {
        delete E;
        delete B;
        delete H;
        delete history;
        delete hash_stats;
    }

    Move SolverPVS::get_move(MS time)
    {
        timer.set();
        max_ply = 0;
        nodes = 0;
        to_think = time;
        thinking = true;

        history->clear();
        shift_killers();

        Move best = None; 
        for (int depth = 1; depth <= MAX_PLY; depth++)
        {
            int val = pvs(-INF, INF, depth);
		    if (!thinking) break;

            if (depth > max_ply) max_ply = depth;

            cout << "info depth " << depth
                 << " seldepth " << max_ply
                 << " score ";

            if      (val >  MATE) cout << "mate " <<  (INF - val) / 2 + 1;
            else if (val < -MATE) cout << "mate " << -(INF + val) / 2 - 1;
            else                  cout << "cp " << val;

            cout << " nodes " << nodes
                 << " time " << timer.getms()
                 << " pv " << B->best()
                 << endl;

            best = B->best();
            if (val > MATE || val < -MATE) break;
        }

        cout << "bestmove " << best << endl;

        thinking = false;
        return best;
    }

    U64 SolverPVS::perft(int depth)
    {
        U64 count = EMPTY;

        cout << "-- Perft " << depth << endl;
        B->print();

        timer.set();

        B->init_node();
        B->generate<true>();
        B->generate<false>();

        while (Move move = B->get_next_move(history))
        {
            if (!B->make(move)) continue;
            cout << B->prettify(move) << " - ";

            U64 cnt = perft_inner(depth - 1);
            count += cnt;

            cout << cnt << endl;
            B->unmake(move);
        }

        long long time = timer.getms();
        double knps = static_cast<double>(count) / (time + 1);

        cout << endl;
        cout << "Count: " << count << endl;
        cout << "Time: " << time << " ms" << endl;
        cout << "Speed: " << knps << " knps" << endl;
        cout << endl << endl;

        return count;
    }

    U64 SolverPVS::perft_inner(int depth)
    {
        if (depth <= 0) return 1;

        B->init_node();
        B->generate<true>();
        B->generate<false>();

        U64 count = EMPTY;
        while (auto move = B->get_next_move(history))
        {
            if (!B->make(move)) continue;

            if (depth > 1) count += perft_inner(depth - 1);
            else count++;
            B->unmake(move);
        }
        return count;
    }

    void SolverPVS::shift_killers()
    {
        for (State * s = states; s < states + MAX_PLY - 2; s++)
        {
            s->killer[0] = (s + 2)->killer[0];
            s->killer[1] = (s + 2)->killer[1];
        }
    }

    bool SolverPVS::time_lack() const
    {
        if (!thinking) return true;
        if (infinite) return false;
        const MS time_to_move = to_think / 30;
        if (timer.getms() > time_to_move)
        {
            thinking = false;
            return true;
        }
        return false;
    }

    void SolverPVS::check_input() const
    {
        MS time = timer.getms();
        if (time - last_response > RESPONSE_TIME)
        {
            if (input_available())
            {
                engine->read_input();
            }
            last_response = time;
        }
    }

    int SolverPVS::ply() const
    {
        return B->ply(states);
    }

    int SolverPVS::pvs(int alpha, int beta, int depth)
    {
        if (depth <= 0) return qs(alpha, beta);
        check_input();
        if (time_lack()) return 0;

        const bool in_pv = (beta - alpha) > 1;
        HashType hash_type = HASH_ALPHA;
        bool search_pv = true;
        B->best() = Move();
        int val = -INF;
        nodes++;

        int legal = 0;

        // 1. Retrieving hash move

        HashEntry * he = 0;
        Move hash_move = None;

        he = H->get(B->hash());
        if (he)
        {
            if (B->is_pseudolegal(he->move))
                hash_move = he->move;

            if (ply() > 0) // Not in root
            {
                if (he->depth >= depth)
                {
                    int v = he->val;
                    if      (v >  MATE && v <=  INF) v -= ply();
                    else if (v < -MATE && v >= -INF) v += ply();

                    // Exact score
                    if (he->type == HASH_EXACT) return v;
                    else if (he->type == HASH_ALPHA && v <= alpha) return alpha;
                    else if (he->type == HASH_BETA  && v >= beta) return beta;
                }
            }
        }

        B->init_node(hash_move);

        while (Move move = B->get_next_move(history))
        {
            if (!B->make(move)) continue;
            legal++;
            int new_depth = depth - 1;
            bool reduced = false;

            if (search_pv)
                val = -pvs(-beta, -alpha, new_depth);
            else
            {
                val = -pvs(-alpha - 1, -alpha, new_depth);
                if (val > alpha && val < beta)
                    val = -pvs(-beta, -alpha, new_depth);
            }

            if (reduced && val >= beta)
                val = -pvs(-beta, -alpha, new_depth + 1);

            B->unmake(move);

            if (val > alpha)
            {
                alpha = val;
                hash_type = HASH_EXACT;
                B->best() = move;
                search_pv = false;

                if (val >= beta)
                {
                    int in_check = B->in_check();
                    if (!is_cap_or_prom(move) && !in_check)
                        B->update_moves_stats(move, depth, history);

                    alpha = beta;
                    hash_type = HASH_BETA;
                    break;
                }
            }
        }

        if (!legal)
	    {
            int in_check = B->in_check();
		    return in_check > 0 ? -INF + ply() : 0; // contempt();
	    }

        H->set(B->hash(), B->best(), alpha, hash_type, depth, ply());

        return alpha;
    }

    int SolverPVS::qs(int alpha, int beta)
    {
        int standpat = B->eval(E);
        if (standpat >= beta) return beta;
        if (alpha < standpat) alpha = standpat;

        if (ply() > max_ply) max_ply = ply();
        nodes++;

        B->init_node();

        while (Move move = B->get_next_move_qs())
        {
            if (!B->make(move)) continue;
            int val = -qs(-beta, -alpha);
            B->unmake(move);

            if (val >= beta) return beta;
            if (val > alpha) alpha = val;
        }

        return alpha;
    }
}
