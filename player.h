#pragma once
#include "types.h"
#include "board.h"

namespace eia_v0_5
{
    using MS = U64;

    class Player
    {
    public:
        Player() {}
        virtual ~Player() {}
        virtual void set(const Board * board) = 0;
        virtual Move get_move(MS time) = 0;
        virtual U64 perft(int depth) = 0;
    };

    class Solver : public Player
    {
    protected:
        mutable bool thinking  = false;
        mutable bool debugging = false;
        mutable bool infinite  = false;

    public:
        void stop() { thinking = false; }
        void debug(bool val) { debugging = val; }
        void analysis(bool val) { infinite = val; }
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
