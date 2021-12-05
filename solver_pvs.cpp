#include "solver_pvs.h"
#include "board.h"

namespace eia_v0_5
{
    Move SolverPVS::get_move(MS time)
    {
        Move best;
        nodes = 0;
        thinking = true;

        for (int p = 0; p < PIECE_N; p++)
            for (int s = 0; s < 64; s++)
                history[p][s] = 0;

        for (int depth = 1; depth <= MAX_PLY; depth++)
        {
            int val = pvs(-INF, INF, depth);
		    if (!thinking) break;
        }

        thinking = false;
        return best;
    }

    U64 SolverPVS::perft(int depth)
    {
        return U64();
    }

    bool SolverPVS::time_lack() const
    {
        if (!thinking) return true;
        const MS time_to_move = to_think / 100;
        if (timer.getms() > time_to_move)
        {
            thinking = false;
            return true;
        }
        return false;
    }

    int SolverPVS::pvs(int alpha, int beta, int depth)
    {
        /*
        if (depth <= 0) return B->eval(E);
        if (time_lack()) return 0;

        const bool in_pv = (beta - alpha) > 1;
        bool search_pv = true;
        state->best = Move();
        int val = -INF;
        nodes++;

        int legal = 0;
        state->ml.clear();
        while (Move move = state->ml.get_next_move())
        {
            state++;
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
            state--;

            if (val > alpha)
            {
                alpha = val;
                //hash_type = Hash_Exact;
                state->best = move;
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
		    return in_check > 0 ? -INF + B->state - B->undo : 0; // contempt();
	    }

    #ifdef SEARCH_HASHING
        H->set(B->state->hash, B->state->best, depth, alpha, hash_type);
    #endif

        return alpha;
        */

        return 0;
    }
}