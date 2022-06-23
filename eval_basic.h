#pragma once
#include "eval.h"
#include "board.h"
#include "piece.h"

namespace eia_v0_5
{
    class EvalBasic : public Eval
    {
    public:
        virtual int eval(const BoardInner * I) const override;

        template<PieceType pt>
        int evaluate(const BoardInner * I, int wtm) const;
    };

    template<>
    inline int EvalBasic::evaluate<PAWN>(const BoardInner * I, int wtm) const
    {
        const U64 rank_own = wtm ? RANK_5 : RANK_4;
        const U64 rank_opp = wtm ? RANK_7 : RANK_2;
        const int passer_val[] = {0, 0, 0, 5, 10, 20, 35, 50};

        int val = 0;
        const U64 own = I->piece[BP ^ wtm];
        const U64 opp = I->piece[WP ^ wtm];
        for (U64 bb = own; bb; bb = rlsb(bb))
        {
            const SQ sq = bitscan(bb);

            if (!(front(wtm, sq) & opp)) // Potential candidate passer
            {
                U64 sentries = att_span(wtm, sq) & opp;
                
                if (!sentries) // Passer
                {
                    val += 3 * passer_val[wtm ? y_(sq) : 7 - y_(sq)];
                }
                else if (bb & rank_own) // Candidate on 5th rank
                {
                    SQ stop = wtm ? sq + 8 : sq - 8;
                    U64 helpers = att_span(wtm ^ 1, stop) & own;
                    if (popcnt(helpers) >= popcnt(sentries))
                        val += passer_val[wtm ? y_(sq) : 7 - y_(sq)];
                }
            }
        }

        return val;
    }

    template<PieceType pt>
    inline int EvalBasic::evaluate(const BoardInner * I, int wtm) const
    {
        int val = 0;
        const Piece p = to_piece(pt, wtm);

        for (U64 bb = I->piece[p]; bb; bb = rlsb(bb))
        {
            const SQ sq = bitscan(bb);
            const U64 att = I->attack<pt>(sq);
            val += 3 * popcnt(att);
        }
        return val;
    }
}
