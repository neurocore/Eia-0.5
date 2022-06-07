#pragma once
#include "eval.h"

namespace eia_v0_5
{
    using MS = U8;

    class Player
    {
    public:
        Player(Board * board) : B(board) {}
        virtual ~Player() {}
        virtual Move get_move(MS time) = 0;
        virtual U64 perft(int depth) = 0;

    protected:
        Board * const B;
    };

    class Reader : public Player
    {
    public:
        Reader(Board * board) : Player(board) {}
        virtual Move get_move(MS time);
        virtual U64 perft(int depth) { return 0; }
        virtual ~Reader() {}
    };
}