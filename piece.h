#pragma once
#include <vector>
#include "consts.h"

using Ray = std::pair<int, int>;
using Rays = std::vector<Ray>;

namespace eia_v0_5
{
    class PieceTables
    {
        U64 att[PIECE_N][SQUARE_N];
        int dir[SQUARE_N][SQUARE_N];
        U64 between[SQUARE_N][SQUARE_N];
        U64 forward_one[COLOR_N][SQUARE_N];

    public:
        PieceTables();

    private:
        void init_piece(Piece piece, Rays rays, bool slider = false);
        friend U64 att(Piece piece, SQ sq);
        friend int dir(SQ i, SQ j);
        friend U64 between(SQ i, SQ j);
        friend U64 forward_one(Color color, SQ j);
    };

    extern PieceTables * PT;
    inline U64 att(Piece piece, SQ sq) { return PT->att[piece][sq]; }
    inline int dir(SQ i, SQ j) { return PT->dir[i][j]; }
    inline U64 between(SQ i, SQ j) { return PT->between[i][j]; }
    inline U64 forward_one(Color color, SQ sq) { return PT->forward_one[color][sq]; }
}
