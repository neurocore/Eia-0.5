#pragma once
#include <string>
#include "movelist.h"
#include "eval.h"

using std::string;

namespace eia_v0_5
{
    struct BoardInner
    {
        U64 piece[PIECE_N];
        U64 occ[COLOR_N];
        int sq[SQUARE_N];
        int wtm = 1;

        Castling castling;
        SQ en_passant;
        int fifty = 0;
    };

    class Eval;
    class Board
    {
        BoardInner inner;

    public:
        void print() const;

        void clear();
        void reset();
        bool from_fen(string fen);
        string make_fen();
        string prettify(const Move & move);
        int eval(const Eval * E) const;
        bool make(const Move & move);
        void unmake(const Move & move);

    private:
        void parse_fen_board(string str);
        void parse_fen_color(string str);
        void parse_fen_castling(string str);
        void parse_fen_ep(string str);
    };
}