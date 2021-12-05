#pragma once
#include <ostream>
#include <string>
#include "utils.h"

using std::ostream;
using std::string;

namespace eia_v0_5
{
    class Move
    {
        SQ from_, to_;
        PieceType prom_;

    public:
        enum Type { Empty, Null };

        Move() : prom_(KING) {}
        Move(SQ from, SQ to, PieceType prom)
            : from_(from), to_(to), prom_(prom)
        {}

        SQ from()       const { return from_; }
        SQ to()         const { return to_; }
        bool empty()    const { return prom_ == KING; }
        operator bool() const { return !(empty()); }

        U16 pack() const;
        static Move unpack(U16 val);

        bool operator == (const Move & move) const;
        friend ostream & operator << (ostream & os, const Move & move);
    };
}