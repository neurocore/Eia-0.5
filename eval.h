#pragma once
#include "board.h"

namespace eia_v0_5
{
    struct BoardInner;
    class Eval
    {
    public:
        virtual int eval(const BoardInner & inner) const = 0;
    };

    class EvalEmpty : public Eval
    {
    public:
        int eval(const BoardInner & inner) const override { return 0; }
    };

    class EvalSimple : public Eval
    {
    public:
        int eval(const BoardInner & inner) const override;
    };
}