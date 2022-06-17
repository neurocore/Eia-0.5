#pragma once
#include "types.h"
#include "board.h"

namespace eia_v0_5
{
    using MS = U64;

    // Real (through communication with GUI) or fake player
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
        virtual ~Reader() {}
        virtual void set(const Board * board) override {}
        virtual Move get_move(MS time) override;
        virtual U64 perft(int depth) override { return 0; }
    };
}
