#pragma once
#include "board.h"

namespace eia_v0_5
{
    // For certain board it contains:
    // - undo move information
    // - tree search variables
    struct State
    {
        Castling castling;
        SQ en_passant;
        Piece cap;
        U8 fifty;

        U64 hash;

        Move curr, best;
        Move killer[2];
        MoveList ml;
    };
}
