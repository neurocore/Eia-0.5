#include <sstream>
#include "moves.h"

using namespace std;

namespace eia_v0_5
{
    U16 Move::pack() const
    {
        return (from_ & 077) | (to_ & 077) << 6 | prom_ << 12;
    }

    Move Move::unpack(U16 val)
    {
        return Move(SQ(val & 077), SQ((val >> 6) & 077), PieceType(val >> 12));
    }

    bool Move::operator == (const Move & move) const
    {
        return from_ == move.from() && to_ == move.to();
    }

    ostream & operator << (ostream & os, const Move & move)
    {
        os << move.from() << move.to();
        return os;
    }
}
