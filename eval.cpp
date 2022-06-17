#include "board.h"
#include "eval.h"

namespace eia_v0_5
{
    int EvalSimple::eval(const BoardInner * I) const
    {
        int score[] = {-100, 100, -300, 300, -300, 300, -500, 500, -900, 900};
        int val = 0;
        
        for (int i = 0; i < Piece::BK; i++)
        {
            U64 bb = I->piece[i];
            val += score[i] * popcnt(bb);
        }

        return I->wtm ? val : -val;
    }
}
