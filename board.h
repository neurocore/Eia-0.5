#pragma once
#include <string>
#include "bitboard.h"
#include "movelist.h"
#include "magics.h"
#include "state.h"

using std::string;

namespace eia_v0_5
{
    class Eval;

    struct BoardInner
    {
        U64 piece[PIECE_N];
        U64 occ[COLOR_N];
        int sq[SQUARE_N];
        int wtm = 1;

        State * state;

        BoardInner(State * state) : state(state) {}

        template<PieceType pt>
        U64 attack(SQ sq) const;
        U64 attack(Piece p, SQ sq) const;
    };

    class Board : private BoardInner
    {
    public:
        explicit Board(State * state) : BoardInner(state) { clear(); }
        void print() const;

        void clear();
        void reset();
        bool from_fen(string fen);
        void set(const Board * board);
        string make_fen();
        string prettify(const Move & move);
        int ply(const State * states) const;
        int eval(const Eval * E) const;

        bool is_attacked(SQ king, U64 occupied, int opposite = 0) const;
        bool in_check(int opposite = 0) const;

        bool make(const Move & move, bool self = false);
        void unmake(const Move & move);

        template<bool captures = false>
        void generate();

        template<bool full = false>
        bool operator == (const Board * B) const;

        Move recognize(string move) const;

        void init_node() { state->ml.clear(); }
        Move get_next_move() const { return state->ml.get_next_move(); }
        
        int to_move() const { return wtm; }

        Move & curr() const { return state->curr; }
        Move & best() const { return state->best; }

        void restore_path(int ply = 10);
        bool check_consistency();

    private:
        void parse_fen_board(string str);
        void parse_fen_color(string str);
        void parse_fen_castling(string str);
        void parse_fen_ep(string str);

        template<bool captures = false>
        U64 att_mask();

        template<bool full = true>
        void place(SQ square, int p);

        template<bool full = true>
        void remove(SQ square);

        template<PieceType pt, bool captures = false>
        void gen();
    };

    template<bool captures>
    void Board::generate()
    {
        MoveList & ml = state->ml;

        const U64 me = occ[wtm];
        const U64 opp = occ[wtm ^ 1];
        const U64 o = me | opp;

        gen<PieceType::KNIGHT, captures>();
        gen<PieceType::BISHOP, captures>();
        gen<PieceType::ROOK,   captures>();
        gen<PieceType::QUEEN,  captures>();
        gen<PieceType::KING,   captures>();

        if constexpr (!captures)
        if (wtm) // Castlings
        {
            if ((state->castling & WOO) && !(o & SPAN_WOO))
                ml.add_move(E1, G1, F_KCASTLE);

            if ((state->castling & WOOO) && !(o & SPAN_WOOO))
                ml.add_move(E1, C1, F_QCASTLE);
        }
        else
        {
            if ((state->castling & BOO) && !(o & SPAN_BOO))
                ml.add_move(E8, G8, F_KCASTLE);

            if ((state->castling & BOOO) && !(o & SPAN_BOOO))
                ml.add_move(E8, C8, F_QCASTLE);
        }

        Piece p = BP + wtm;
        if (wtm)
        {
            // TODO: apply if constexpr on captures and quiets

            if constexpr (!captures)
			for (U64 bb = piece[p] & shift<Dir::D>(~o); bb; bb = rlsb(bb)) // Forward & promotion
			{
				SQ s = bitscan(bb);
                if (y_(s) == 6) ml.add_prom(s, s + 8);
                else            ml.add_move(s, s + 8);
			}

            if constexpr (captures)
            {
			    for (U64 bb = piece[p] & shift<Dir::DL>(opp); bb; bb = rlsb(bb)) // Attacks
			    {
				    SQ s = bitscan(bb);
                    if (y_(s) == 6) ml.add_capprom(s, s + 7);
                    else            ml.add_move(s, s + 7, F_CAP);
			    }

            
			    for (U64 bb = piece[p] & shift<Dir::DR>(opp); bb; bb = rlsb(bb))
			    {
				    SQ s = bitscan(bb);
                    if (y_(s) == 6) ml.add_capprom(s, s + 9);
                    else            ml.add_move(s, s + 9, F_CAP);
			    }
            }

            if constexpr (!captures)
			for (U64 bb = piece[p] & (~o >> 8) & (~o >> 16); bb; bb = rlsb(bb)) // Double move
			{
				SQ s = bitscan(bb);
                if (y_(s) == 1) ml.add_move(s, s + 16, F_PAWN2);
			}

            if constexpr (captures)
			if (state->en_passant)
			{
				for (U64 bb = piece[p] & moves(WP - wtm, state->en_passant); bb; bb = rlsb(bb))
				{
                    ml.add_move(bitscan(bb), state->en_passant, F_EP);
				}
			}
        }
        else
        {
            if constexpr (!captures)
			for (U64 bb = piece[p] & shift<Dir::U>(~o); bb; bb = rlsb(bb)) // Forward & promotion
			{
				SQ s = bitscan(bb);
                if (y_(s) == 1) ml.add_prom(s, s - 8);
                else            ml.add_move(s, s - 8);
			}

            if constexpr (captures)
            {
			    for (U64 bb = piece[p] & shift<Dir::UR>(opp); bb; bb = rlsb(bb)) // Attacks
			    {
				    SQ s = bitscan(bb);
                    if (y_(s) == 1) ml.add_capprom(s, s - 7);
                    else            ml.add_move(s, s - 7, F_CAP);
			    }

			    for (U64 bb = piece[p] & shift<Dir::UL>(opp); bb; bb = rlsb(bb))
			    {
				    SQ s = bitscan(bb);
                    if (y_(s) == 1) ml.add_capprom(s, s - 9);
                    else            ml.add_move(s, s - 9, F_CAP);
			    }
            }

            if constexpr (!captures)
			for (U64 bb = piece[p] & (~o << 8) & (~o << 16); bb; bb = rlsb(bb)) // Double move
			{
				SQ s = bitscan(bb);
                if (y_(s) == 6) ml.add_move(s, s - 16, F_PAWN2);
			}

            if constexpr (captures)
			if (state->en_passant)
			{
				for (U64 bb = piece[p] & moves(WP - wtm, state->en_passant); bb; bb = rlsb(bb))
				{
                    ml.add_move(bitscan(bb), state->en_passant, F_EP);
				}
			}
        }
    }

    template<> inline U64 Board::att_mask<true>()  { return occ[wtm ^ 1]; }
    template<> inline U64 Board::att_mask<false>() { return ~(occ[0] | occ[1]); }

    template<> inline U64 BoardInner::attack<PieceType::BISHOP>(SQ sq) const { return b_att(occ[0] | occ[1], sq); }
    template<> inline U64 BoardInner::attack<PieceType::ROOK>(SQ sq) const { return r_att(occ[0] | occ[1], sq); }
    template<> inline U64 BoardInner::attack<PieceType::QUEEN>(SQ sq) const { return q_att(occ[0] | occ[1], sq); }
    template<PieceType pt> inline U64 BoardInner::attack(SQ sq) const
    {
        return moves(static_cast<Piece>((+pt) << 1), sq);
    }

    template<bool full>
    inline void Board::place(SQ square, int p)
    {
        piece[p]   ^= (BIT << square);
        occ[p & 1] ^= (BIT << square);
        sq[square]  = p;

        if (full)
        {
            //state->pst += E->pst[p][square];
            //state->mkey += matkey[p];

            //state->hash ^= hash_key[p][square];
            //if (IS_PAWN(p)) state->phash ^= hashKey[p][square];
        }
    }

    template<bool full>
    inline void Board::remove(SQ square)
    {
        int p = sq[square];

        piece[p]   ^= (BIT << square);
        occ[p & 1] ^= (BIT << square);
        sq[square]  = NOP;

        if (full)
        {
            //state->pst -= E->pst[p][square];
            //state->mkey -= matkey[p];

            //state->hash ^= hash_key[p][square];
            //if (IS_PAWN(p)) state->phash ^= hashKey[p][square];
        }
    }

    template<PieceType pt, bool captures>
    inline void Board::gen()
    {
        MoveList & ml = state->ml;

        Piece p = static_cast<Piece>(2 * pt + wtm);
        const U64 mask = att_mask<captures>();

        for (U64 bb = piece[p]; bb; bb = rlsb(bb))
	    {
		    SQ s = bitscan(bb);
            for (U64 att = attack(p, s) & mask; att; att = rlsb(att))
            {
                ml.add_move(s, bitscan(att), captures ? F_CAP : F_QUIET);
            }
	    }
    }

    template<bool full>
    bool Board::operator == (const Board * B) const
    {
        if (wtm != B->wtm) return false;
        if (occ[0] != B->occ[0]) return false;
        if (occ[1] != B->occ[1]) return false;

        for (int i = 0; i < PIECE_N; i++)
            if (piece[i] != B->piece[i]) return false;

        for (int i = 0; i < SQUARE_N; i++)
            if (sq[i] != B->sq[i]) return false;

        if constexpr (full)
        {
            if (state->en_passant != B->state->en_passant) return false;
            if (state->castling != B->state->castling) return false;
            if (state->fifty != B->state->fifty) return false;
            if (state->hash != B->state->hash) return false;
            if (state->cap != B->state->cap) return false;
        }

        return true;
    }
}
