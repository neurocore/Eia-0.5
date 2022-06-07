#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

#include <ostream>
#include "consts.h"

using std::ostream;

namespace eia_v0_5
{
    extern inline bool is_pawn(Piece p) { return p < BN; }

    extern inline int x_(SQ sq) { return sq & 7; }
    extern inline int y_(SQ sq) { return sq >> 3; }
    extern inline SQ sq_(int x, int y) { return SQ((y << 3) + x); }

    ostream & operator << (ostream & os, const Piece & p);
    ostream & operator << (ostream & os, const SQ & sq);

    SQ operator ++ (SQ & sq);
    SQ operator -- (SQ & sq);

    extern constexpr SQ operator + (SQ sq, int val);
    extern constexpr SQ operator - (SQ sq, int val);

    extern int dist(SQ a, SQ b);

    SQ & operator += (SQ & sq, int val);
    SQ & operator -= (SQ & sq, int val);

    extern constexpr Castling   operator |  (const Castling & a, const Castling & b);
    extern constexpr Castling & operator |= (Castling & lhs, const Castling & rhs);
    extern constexpr Castling   operator &  (const Castling & a, const Castling & b);
    extern constexpr Castling & operator &= (Castling & lhs, const Castling & rhs);

    extern constexpr Castling   operator -  (const Castling & a, const SQ & square);

    template<Dir dir> U64 shift(U64 b);
    extern constexpr inline U64 shift(U64 b, Dir dir);

    template<typename T, typename R>
    constexpr inline R compare(T x, T y, R lt, R eq, R gt)
    {
        return x < y ? lt : (y < x ? gt : eq);
    }

    template <typename T> int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    extern U64 rand64();
}
