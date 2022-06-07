#pragma once
#include "eval.h"

namespace eia_v0_5
{
    using MS = U8;

    class Player
    {
    public:
        Player() {}
        virtual ~Player() {}
        virtual void set(const Board * board) = 0;
        virtual Move get_move(MS time) = 0;
        virtual U64 perft(int depth) = 0;
    };

    class Reader : public Player
    {
    public:
        Reader() : Player() {}
        virtual ~Reader() {}
        virtual void set(const Board * board) override {}
        virtual Move get_move(MS time) override;
        virtual U64 perft(int depth) override { return 0; }
    };
}
