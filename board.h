#pragma once
#include <string>
#include "bitboard.h"
#include "movelist.h"
#include "magics.h"
#include "state.h"
#include "eval.h"

using std::string;

namespace eia_v0_5
{
    class Eval;
    class Board
    {
        U64 piece[PIECE_N];
        U64 occ[COLOR_N];
        int sq[SQUARE_N];
        int wtm = 1;

        State * state;

    public:
        explicit Board(State * state) : state(state) { clear(); }
        void print() const;

        void clear();
        void reset();
        bool from_fen(string fen);
        void set(const Board * board);
        string make_fen();
        string prettify(const Move & move);
        int ply(const State * states) const;
        int eval(const Eval * E) const;

        bool is_attacked(SQ king, U64 occupied, int opposite = 0);

        bool make(const Move & move);
        void unmake(const Move & move);

        template<bool full = false>
        bool operator == (const Board * B) const;

        void init_node() { state->ml.clear(); }
        Move get_next_move() const { return state->ml.get_next_move(); }

    private:
        void parse_fen_board(string str);
        void parse_fen_color(string str);
        void parse_fen_castling(string str);
        void parse_fen_ep(string str);

        template<bool captures = false>
        U64 att_mask();

        template<PieceType pt>
        U64 attack(SQ sq);

        U64 attack(Piece p, SQ sq);

        template<bool full = true>
        void place(SQ square, int p);

        template<bool full = true>
        void remove(SQ square);
    };
}