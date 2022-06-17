#include "eval_basic.h"
#include "board.h"

namespace eia_v0_5
{
    int EvalBasic::eval(const BoardInner * I) const
    {
        int score[] = {-100, 100, -300, 300, -300, 300, -500, 500, -900, 900};
        int val = 0;
        
        for (int i = 0; i < Piece::BK; i++)
        {
            U64 bb = I->piece[i];
            val += score[i] * popcnt(bb);
        }

        val += evaluate<BISHOP>(I, 1) - evaluate<BISHOP>(I, 0);
        val += evaluate<KNIGHT>(I, 1) - evaluate<KNIGHT>(I, 0);
        val += evaluate<ROOK>(I, 1)   - evaluate<ROOK>(I, 0);
        val += evaluate<QUEEN>(I, 1)  - evaluate<QUEEN>(I, 0);

        return I->wtm ? val : -val;
    }
}