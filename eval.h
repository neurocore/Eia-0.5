#pragma once

namespace eia_v0_5
{
    struct BoardInner;
    class Eval
    {
    public:
        virtual int eval(const BoardInner * I) const = 0;
    };

    class EvalEmpty : public Eval
    {
    public:
        int eval(const BoardInner * I) const override { return -INF; }
    };

    class EvalSimple : public Eval
    {
    public:
        int eval(const BoardInner * I) const override;
    };
}
