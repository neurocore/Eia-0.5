#pragma once
#include "player.h"
#include "timer.h"

namespace eia_v0_5
{
    struct State
    {
        Castling castling;
        SQ en_passant;
        int fifty = 0;

        Piece cap;
        U64 hash;
        Move curr, best;
        Move killer[2];
        MoveList ml;
    };

    class SolverPVS : public Player
    {
        Eval * E;
        MS to_think;
        Timer timer;

        U64 nodes;
        mutable bool thinking;
        State undo[MAX_PLY];
        State * state = undo;
        int history[PIECE_N][SQUARE_N];

    public:
        SolverPVS(Board * board) : Player(board) {}
        virtual Move get_move(MS time);
        virtual U64 perft(int depth);
        bool time_lack() const;
        int pvs(int alpha, int beta, int depth);
    };
}