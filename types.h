#pragma once

namespace eia_v0_5
{
    using U8  = unsigned char;
    using U16 = unsigned short;
    using U32 = unsigned int;
    using U64 = unsigned long long;

    using S8  = char;
    using S16 = short;
    using S32 = int;
    using S64 = long long;

    using MB = U32; // megabytes

    enum Color { BLACK, WHITE, COLOR_N };

    enum SQ : U8
    {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8, SQUARE_N
    };

    extern inline U64 bit(SQ sq) { return 1ull << sq; }

    enum class Dir { U, D, L, R, UL, UR, DL, DR };

    enum Castling : U8
    { 
        BOO  = 1 << 0,
        WOO  = 1 << 1,
        BOOO = 1 << 2,
        WOOO = 1 << 3,
        CALL = BOO | WOO | BOOO | WOOO
    };

    const U64 SPAN_WOO  = bit(F1) | bit(G1);
    const U64 SPAN_WOOO = bit(B1) | bit(C1) | bit(D1);
    const U64 SPAN_BOO  = bit(F8) | bit(G8);
    const U64 SPAN_BOOO = bit(B8) | bit(C8) | bit(D8);
}
