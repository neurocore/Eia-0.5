#include "eval.h"
#include "consts.h"

namespace eia_v0_5
{
    ostream & operator << (ostream & os, const Score & score)
    {
        if      (score.val >  MATE) os << "mate " <<  (INF - score.val) / 2 + 1;
        else if (score.val < -MATE) os << "mate " << -(INF + score.val) / 2 - 1;
        else                        os << "cp " << score.val;
        return os;
    }
}