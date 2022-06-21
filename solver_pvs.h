#pragma once
#include "solver.h"
#include "timer.h"
#include "state.h"
#include "hash.h"

namespace eia_v0_5
{
    class History;
    class SolverPVS : public Solver
    {
        Board * B;
        Hash * H;
        Eval * E;
        MS to_think;
        Timer timer;

        U64 nodes;
        int max_ply;
        State states[MAX_PLY];
        History * history;
        HashStats * hash_stats;

    public:
        explicit SolverPVS(Engine * engine);
        virtual ~SolverPVS();
        virtual void set(const Board * board) override { B->set(board); }
        virtual Move get_move(MS time) override;
        virtual U64 perft(int depth) override;
        U64 perft_inner(int depth);
        void shift_killers();
        bool time_lack() const;
        void check_input() const;
        int ply() const;
        int pvs(int alpha, int beta, int depth);
        int qs(int alpha, int beta);
    };
}
