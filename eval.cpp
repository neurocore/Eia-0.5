#include "eval.h"

namespace eia_v0_5
{
    int EvalSimple::eval(const BoardInner & inner) const
    {
        return inner.wtm ? 100 : -100;
    }
}
