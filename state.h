#pragma once
#include "board.h"

namespace eia_v0_5
{
    enum class Stage { Hash, Attacks, Killers, Quiet, Ending };

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

        Stage stage;
        Move curr, best;
        Move killer[2];
        MoveList ml;
    };
}
