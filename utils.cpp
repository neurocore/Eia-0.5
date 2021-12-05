#include "utils.h"

namespace eia_v0_5
{
    template <typename T>
    int operator + (const T & val)
    {
        return static_cast<int>(val);
    }

    ostream & operator << (ostream & os, const Piece & p)
    {
        os << "pPnNbBrRqQkK"[p];
        return os;
    }

    ostream & operator << (ostream & os, const SQ & sq)
    {
        os << FILOW[x_(sq)] << RANKS[y_(sq)];
        return os;
    }

    SQ operator ++ (SQ & sq, int)
    {
        sq = static_cast<SQ>(+sq + 1);
        return sq;
    }

    SQ operator -- (SQ & sq, int)
    {
        sq = static_cast<SQ>(+sq - 1);
        return sq;
    }

    SQ operator + (SQ sq, int val)
    {
        return static_cast<SQ>(+sq + val);
    }

    SQ operator - (SQ sq, int val)
    {
        return static_cast<SQ>(+sq - val);
    }

    SQ & operator += (SQ & sq, int val)
    {
        sq = static_cast<SQ>(+sq + val);
        return sq;
    }

    SQ & operator -= (SQ & sq, int val)
    {
        sq = static_cast<SQ>(+sq - val);
        return sq;
    }

    Castling operator | (const Castling & a, const Castling & b)
    {
        return static_cast<Castling>(+a | +b);
    }

    Castling & operator |= (Castling & lhs, const Castling & rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    template<> constexpr U64 shift<Dir::U>(U64 b)  { return b << 8; }
    template<> constexpr U64 shift<Dir::D>(U64 b)  { return b >> 8; }
    template<> constexpr U64 shift<Dir::R>(U64 b)  { return (b & ~FILE_H) << 1; }
    template<> constexpr U64 shift<Dir::L>(U64 b)  { return (b & ~FILE_A) >> 1; }
    template<> constexpr U64 shift<Dir::UR>(U64 b) { return (b & ~FILE_A) << 7; }
    template<> constexpr U64 shift<Dir::UL>(U64 b) { return (b & ~FILE_H) << 9; }
    template<> constexpr U64 shift<Dir::DR>(U64 b) { return (b & ~FILE_A) >> 9; }
    template<> constexpr U64 shift<Dir::DL>(U64 b) { return (b & ~FILE_H) >> 7; }

    constexpr U64 shift(U64 b, Dir dir)
    {
        switch (dir)
        {
            case Dir::U:  return shift<Dir::U>(b);  break;
            case Dir::D:  return shift<Dir::D>(b);  break;
            case Dir::L:  return shift<Dir::L>(b);  break;
            case Dir::R:  return shift<Dir::R>(b);  break;
            case Dir::UL: return shift<Dir::UL>(b); break;
            case Dir::UR: return shift<Dir::UR>(b); break;
            case Dir::DL: return shift<Dir::DL>(b); break;
            case Dir::DR: return shift<Dir::DR>(b); break;
        }
        return b;
    }

    U64 rand64()
    {
        U64 r = 0ull;
        r += +(rand() & 0x7FFF);
        r <<= 15;
        r += +(rand() & 0x7FFF);
        r <<= 15;
        r += +(rand() & 0x7FFF);
        r <<= 15;
        r += +(rand() & 0x7FFF);
        r <<= 15;
        r += +(rand() & 0x7FFF);
        r <<= 15;
        return r;
    }
}
