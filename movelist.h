#pragma once
#include <limits>
#include "moves.h"

namespace eia_v0_5
{
    enum class GenStage
    {
        HASH, GEN_CAPS, WIN_CAPS, EQ_CAPS, KILLER1, KILLER2,
        GEN_QUIET, QUIET, BAD_CAPS
    };

    class MoveVal
    {
        Move move_;
        int val_;

    public:
        MoveVal(Move move = Move(), int val = 0)
            : move_(move), val_(val)
        {}

        Move move() const { return move_; }
        int val() const { return val_; }
        friend bool operator == (const MoveVal & a, const MoveVal & b);
        friend bool operator <  (const MoveVal & a, const MoveVal & b);
    };

    class MoveList
    {
        Move hashmove;
        MoveVal moves[256];
        MoveVal * first = moves;
        MoveVal * last = moves;
        GenStage stage = GenStage::HASH;

    public:
        MoveList(Move hashmove = Move())
            : hashmove(hashmove)
        {}

        void clear(Move hash_move = Move());
        Move get_best_move(int lower_bound = -INT_MAX);

        void add(Move move);
        void print() const;
        Move get_next_move() const;

        void set_values();
        void generate_all();
        void generate_attacks();
        void generate_quiets();
        void generate_evasions();
        void sort();

    private:
        void remove(MoveVal * mv);
        void remove_hashmove();
        void remove_hashmove_and_killers();
    };
}