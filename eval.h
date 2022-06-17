#pragma once

namespace eia_v0_5
{
    struct BoardInner;
    class Eval
    {
    public:
        virtual int eval(const BoardInner * I) const = 0;
    };
}
