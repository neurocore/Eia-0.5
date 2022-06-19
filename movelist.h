#pragma once
#include <iterator>
#include <limits>
#include "moves.h"

namespace eia_v0_5
{
    enum class GenStage
    {
        HASH, GEN_CAPS, WIN_CAPS, EQ_CAPS, KILLER1, KILLER2,
        GEN_QUIET, QUIET, BAD_CAPS
    };

    struct MoveVal // POD
    {
        Move move;
        int val;
    };

    class MoveList
    {
        Move hashmove;
        MoveVal moves[256];
        GenStage stage = GenStage::HASH;
        MoveVal * first, * last;

    public:
        MoveList(Move hashmove = Move())
            : hashmove(hashmove)
        {
            clear();
        }

        void clear(Move hash_move = None);
        bool empty() const { return last == first; }
        U64  count() const { return last -  first; }

        Move get_next_move();
        Move get_best_move(int lower_bound = -INT_MAX);

        void add(Move move, int val = 0);
        void add_move(SQ from, SQ to, Flags flags = F_QUIET);
        void add_prom(SQ from, SQ to);
        void add_capprom(SQ from, SQ to);
        void print() const;

        void set_values();

    private:
        void remove(MoveVal * ptr);
        void remove_hashmove();
        void remove_hashmove_and_killers();
    };
}
