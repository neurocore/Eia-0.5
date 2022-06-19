#pragma once
#include <ostream>
#include <array>
#include "types.h"

using std::ostream;

namespace eia_v0_5
{
    class BBTables
    {
        const U64 DEBRUIJN = 0x03f79d71b4cb0a89ull;

        U32 lut[65536];
        const int btscn[64] =
        {
             0,  1, 48,  2, 57, 49, 28,  3,
            61, 58, 50, 42, 38, 29, 17,  4,
            62, 55, 59, 36, 53, 51, 43, 22,
            45, 39, 33, 30, 24, 18, 12,  5,
            63, 47, 56, 27, 60, 41, 37, 16,
            54, 35, 52, 21, 44, 32, 23, 11,
            46, 26, 40, 15, 34, 20, 31, 10,
            25, 14, 19,  9, 13,  8,  7,  6
        };

    public:
        BBTables();

    private:
        friend int popcnt(U64 bb);
        friend SQ bitscan(U64 bb);
    };

    struct BitBoard { U64 val; }; // newtype idiom
    ostream & operator << (ostream & os, const BitBoard & bb);
    extern void print64(U64 bb);

    extern BBTables * BBT;
    extern U64 lsb(U64 bb);
    extern U64 rlsb(U64 bb);
    extern U64 msb(U64 bb);
    extern int popcnt(U64 bb);
    extern SQ bitscan(U64 bb);
}
