#pragma once
#include "eval.h"
#include "board.h"
#include "piece.h"

namespace eia_v0_5
{
    class EvalBasic : public Eval
    {
    public:
        int eval(const BoardInner * I) const override;

        template<PieceType pt>
        int evaluate(const BoardInner * I, int wtm) const;
    };

    template<>
    inline int EvalBasic::evaluate<PAWN>(const BoardInner * I, int wtm) const
    {
        return 0;
    }

    template<PieceType pt>
    inline int EvalBasic::evaluate(const BoardInner * I, int wtm) const
    {
        int val = 0;
        Piece p = to_piece(pt, wtm);

        for (U64 bb = I->piece[p]; bb; bb = rlsb(bb))
        {
            SQ sq = bitscan(bb);
            U64 att = I->attack<pt>(sq);
            val += 3 * popcnt(att);
        }
        return val;
    }
}
