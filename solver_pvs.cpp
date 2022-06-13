#include <cassert>
#include <iostream>
#include "solver_pvs.h"
#include "movelist.h"
#include "board.h"

using namespace std;

namespace eia_v0_5
{
    Move SolverPVS::get_move(MS time)
    {
        timer.set();
        nodes = 0;
        to_think = time;
        thinking = true;

        for (int p = 0; p < PIECE_N; p++)
            for (int s = 0; s < 64; s++)
                history[p][s] = 0;

        Move best = Empty; 
        for (int depth = 1; depth <= MAX_PLY; depth++)
        {
            int val = pvs(-INF, INF, depth);

            cout << "info depth " << depth
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
		    if (!thinking) break;
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

        while (Move move = B->get_next_move())
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
        while (auto move = B->get_next_move())
        {
            if (!B->make(move)) continue;

            if (depth > 1) count += perft_inner(depth - 1);
            else count++;
            B->unmake(move);
        }
        return count;
    }

    bool SolverPVS::time_lack() const
    {
        if (!thinking) return true;
        const MS time_to_move = to_think / 30;
        if (timer.getms() > time_to_move)
        {
            thinking = false;
            return true;
        }
        return false;
    }

    int SolverPVS::ply() const
    {
        return B->ply(states);
    }

    int SolverPVS::pvs(int alpha, int beta, int depth)
    {
        if (depth <= 0) return B->eval(E);
        if (time_lack()) return 0;

        const bool in_pv = (beta - alpha) > 1;
        bool search_pv = true;
        B->best() = Move();
        int val = -INF;
        nodes++;

        int legal = 0;

        B->init_node();
        B->generate<true>();
        B->generate<false>();

        while (Move move = B->get_next_move())
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
                //hash_type = Hash_Exact;
                B->best() = move;
                search_pv = false;

                if (val >= beta)
                {
                    //if (!IS_CAP_OR_PROM(move) && !in_check)
                        //B->update_killers(move, depth);

                    alpha = beta;
                    //hash_type = Hash_Beta;
                    break;
                }
            }
        }

        if (!legal)
	    {
            int in_check = B->in_check();
		    return in_check > 0 ? -INF + ply() : 0; // contempt();
	    }

    #ifdef SEARCH_HASHING
        H->set(B->state->hash, B->state->best, depth, alpha, hash_type);
    #endif

        return alpha;
    }
}
