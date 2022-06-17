#pragma once
#include <ostream>
#include <vector>
#include "consts.h"

using std::ostream;
using Ray = std::pair<int, int>;
using Rays = std::vector<Ray>;

namespace eia_v0_5
{
    enum Piece : U8     { BP, WP, BN, WN, BB, WB, BR, WR, BQ, WQ, BK, WK, PIECE_N, NOP = 13 };
    enum PieceType : U8 { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECE_TYPE_N };

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
        friend U64 moves(Piece piece, SQ sq);
        friend int direction(SQ i, SQ j);
        friend U64 between(SQ i, SQ j);
        friend U64 forward_one(Color color, SQ j);
    };

    extern PieceTables * PT;
    extern ostream & operator << (ostream & os, const Piece & p);
    extern inline Piece operator + (Piece p, int val) { return static_cast<Piece>(+p + val); }
    extern inline Piece operator - (Piece p, int val) { return static_cast<Piece>(+p - val); }
    extern inline Piece operator ^ (Piece p, int val) { return static_cast<Piece>(+p ^ val); }
    extern inline Piece to_piece(PieceType pt, int wtm) { return static_cast<Piece>(wtm ^ ((+pt) << 1)); }
    extern inline bool is_pawn(Piece p) { return p < BN; }
    extern inline bool is_king(Piece p) { return p == BK || p == WK; }
    extern inline U64 moves(Piece piece, SQ sq) { return PT->att[piece][sq]; }
    extern inline int direction(SQ i, SQ j) { return PT->dir[i][j]; }
    extern inline U64 between(SQ i, SQ j) { return PT->between[i][j]; }
    extern inline U64 forward_one(Color color, SQ sq) { return PT->forward_one[color][sq]; }
}
