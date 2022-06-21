#pragma once
#include <ostream>

using std::ostream;

namespace eia_v0_5
{
    struct BoardInner;
    class Eval
    {
    public:
        virtual int eval(const BoardInner * I) const = 0;
    };

    struct Score { int val; }; // newtype idiom
    ostream & operator << (ostream & os, const Score & score);
}
