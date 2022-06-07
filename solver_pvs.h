#pragma once
#include "player.h"
#include "timer.h"
#include "state.h"

namespace eia_v0_5
{
    class SolverPVS : public Player
    {
        Eval * E;
        MS to_think;
        Timer timer;

        U64 nodes;
        mutable bool thinking;
        State states[MAX_PLY];
        int history[PIECE_N][SQUARE_N];

    public:
        SolverPVS(Board * board) : Player(board) {}
        virtual Move get_move(MS time);
        virtual U64 perft(int depth);
        bool time_lack() const;
        int ply() const;
        int pvs(int alpha, int beta, int depth);
    };
}
