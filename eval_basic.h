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
        const U64 own = I->piece[BP ^ wtm];
        const U64 opp = I->piece[WP ^ wtm];
        const U64 rank_own = wtm ? RANK_5 : RANK_4;
        const U64 rank_opp = wtm ? RANK_7 : RANK_2;
        const int passer_val[] = {0, 0, 0, 5, 10, 20, 35, 50};
        const int doubled_val[] = {0, -20, -30, -35, -43, -45, -46, -47, -48};
        const int backward_penalty = 20;
        const int isolator_penalty = 15;

        int val = 0;

        for (U64 bb = own; bb; bb = rlsb(bb))
        {
            // 1. Passers and candidates /////////////////////////////////////////

            const SQ sq = bitscan(bb);
            const U64 sentries = att_span(wtm, sq) & opp;
            const U64 neighbors = att_span(wtm, sq) & own;

            if (!(front(wtm, sq) & opp)) // half-open file
            {
                if (!sentries) // passer
                {
                    val += 3 * passer_val[wtm ? y_(sq) : 7 - y_(sq)];
                }
                else if (bb & rank_own) // candidate on 5th rank
                {
                    const SQ stop = wtm ? sq + 8 : sq - 8;
                    const U64 helpers = att_span(wtm ^ 1, stop) & own;
                    if (popcnt(helpers) >= popcnt(sentries))
                        val += passer_val[wtm ? y_(sq) : 7 - y_(sq)];
                }
            }

            // 2. Backward pawn (penalty) ////////////////////////////////////////

            bool found_neighbor = false;
            const auto dir = wtm ? Dir::U : Dir::D;
            for (U64 att = I->attack<PAWN>(sq); att; att = shift(att, dir))
            {
                if (att & own) // First, find closest neighbor
                {
                    found_neighbor = true;
                    continue;
                }

                if (found_neighbor) // Do opponent control stop-square?
                {
                    if (att & opp) val -= backward_penalty;
                    break;
                }
            }

            // 3. Isolator (penalty) /////////////////////////////////////////////

            if (!(adj_files(sq) & own)) val -= isolator_penalty;
        }

        // 4. Doubled pawns penalty //////////////////////////////////////////////

        for (int x = 0; x < 8; x++)
        {
            U64 file = own & files[x];
            val -= doubled_val[popcnt(file)];
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
