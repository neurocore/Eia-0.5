#pragma once
#include "player.h"
#include "timer.h"
#include "state.h"

namespace eia_v0_5
{
    class SolverPVS : public Solver
    {
        Board * B;
        Eval * E;
        MS to_think;
        Timer timer;

        U64 nodes;
        State states[MAX_PLY];
        int history[PIECE_N][SQUARE_N];

    public:
        SolverPVS() : Solver() { B = new Board(states); E = new EvalSimple; }
        virtual ~SolverPVS() { delete E; delete B; }
        virtual void set(const Board * board) override { B->set(board); }
        virtual Move get_move(MS time) override;
        virtual U64 perft(int depth) override;
        U64 perft_inner(int depth);
        bool time_lack() const;
        int ply() const;
        int pvs(int alpha, int beta, int depth);
    };
}
