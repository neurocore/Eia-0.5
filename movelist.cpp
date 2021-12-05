#include "movelist.h"

namespace eia_v0_5
{
    bool operator == (const MoveVal & a, const MoveVal & b)
    {
        return a.move() == b.move();
    }

    bool operator < (const MoveVal & a, const MoveVal & b)
    {
        return a.val() > b.val();
    }
}