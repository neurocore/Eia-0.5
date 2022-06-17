#pragma once
#include "types.h"

namespace eia_v0_5
{
    const U64 EMPTY  = 0x0000000000000000ull;
    const U64 FULL   = 0xFFFFFFFFFFFFFFFFull;
    const U64 BIT    = 0x0000000000000001ull;
    const U64 LIGHT  = 0x55AA55AA55AA55AAull;
    const U64 DARK   = ~LIGHT;

    const U64 FILE_A = 0x0101010101010101ull;
    const U64 FILE_B = 0x0202020202020202ull;
    const U64 FILE_C = 0x0404040404040404ull;
    const U64 FILE_D = 0x0808080808080808ull;
    const U64 FILE_E = 0x1010101010101010ull;
    const U64 FILE_F = 0x2020202020202020ull;
    const U64 FILE_G = 0x4040404040404040ull;
    const U64 FILE_H = 0x8080808080808080ull;

    const U64 RANK_1 = 0x00000000000000FFull;
    const U64 RANK_2 = 0x000000000000FF00ull;
    const U64 RANK_3 = 0x0000000000FF0000ull;
    const U64 RANK_4 = 0x00000000FF000000ull;
    const U64 RANK_5 = 0x000000FF00000000ull;
    const U64 RANK_6 = 0x0000FF0000000000ull;
    const U64 RANK_7 = 0x00FF000000000000ull;
    const U64 RANK_8 = 0xFF00000000000000ull;

    extern const char * FILES, * FILOW, * RANKS;
    extern const char * ENG_NAME, * ENG_VERS, * ENG_AUTH;
    extern const char * STARTPOS;

    const U32 TIME_DEFAULT = 60000;
    const U32 INC_DEFAULT = 1000;
    const U64 RESPONSE_TIME = 1000;

    const U32 MAX_MOVES = 256;
    const U32 MAX_PLY   = 128;

    const int INF  = 32767;
    const int MATE = 32000;
}
