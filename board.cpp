#include <exception>
#include <cassert>
#include <iostream>
#include <sstream>
#include "piece.h"
#include "board.h"
#include "array.h"
#include "eval.h"

using namespace std;

namespace eia_v0_5
{
    void Board::clear()
    {
        wtm = 1;
        for (int i = 0; i < PIECE_N;  i++) piece[i] = EMPTY;
        for (int i = 0; i < COLOR_N;  i++) occ[i] = EMPTY;
        for (int i = 0; i < SQUARE_N; i++) sq[i] = NOP;
    }

    void Board::reset()
    {
        from_fen(STARTPOS);
    }

    int Board::ply(const State * states) const
    {
        return static_cast<int>(state - states);
    }

    string Board::make_fen()
    {
        return string();
    }

    string Board::prettify(const Move & move)
    {
        stringstream ss;
        int piece = sq[get_from(move)];
        ss << "pPnNbBrRqQkK?"[piece];
        ss << get_from(move);
        ss << get_to(move);
        return ss.str();
    }

    int Board::eval(const Eval * E) const
    {
        return E->eval(this);
    }

    bool Board::is_attacked(SQ king, U64 occupied, int opposite) const
    {
        int col = wtm ^ opposite;
        if (moves(BN ^ col, king) & piece[WN ^ col]) return true; // Knights
        if (moves(BP ^ col, king) & piece[WP ^ col]) return true; // Pawns
        if (moves(BK ^ col, king) & piece[WK ^ col]) return true; // King

        if (b_att(occupied, king) & (piece[WB ^ col] | piece[WQ ^ col])) return true; // Bishops & queens
        if (r_att(occupied, king) & (piece[WR ^ col] | piece[WQ ^ col])) return true; // Rooks & queens

        return false;
    }

    bool Board::in_check(int opposite) const
    {
        SQ king = bitscan(piece[BK ^ wtm ^ opposite]);
        return is_attacked(king, occ[0] | occ[1], opposite);
    }

    bool Board::is_pseudolegal(Move move) const
    {
        return true;
        const SQ from = get_from(move);
        const SQ to = get_to(move);

        if (!(bit(from) & occ[wtm])) return false;
        const Piece p = static_cast<Piece>(sq[from]);
        if (is_pawn(p)) return true;
        U64 att = attack(p, from);

        return att & bit(to);
    }

    bool Board::make(const Move & move, bool self)
    {
        const SQ from = get_from(move);
        const SQ to = get_to(move);

        State * old = state;
        if (!self) state++;
        state->castling = old->castling - from - to;
        state->hash = old->hash
                    ^ HT->hash_ep[old->en_passant]
                    ^ HT->hash_castle[old->castling]
                    ^ HT->hash_wtm;
        state->cap = static_cast<Piece>(sq[to]);
        state->en_passant = A1;
        state->fifty = 0;

        state->best = None;
        state->ml.clear();

        const Piece p = static_cast<Piece>(sq[from]);
        const Flags flags = get_flags(move);

        switch (flags)
        {
            case F_CAP:
            {
                remove(from);
                remove(to);
                place(to, p);
                break;
            }

            case F_KCASTLE:
            {
                remove(from);
                remove(to + 1);
                place(to, p);
                place(to - 1, BR + wtm);
                break;
            }

            case F_QCASTLE:
            {
                remove(from);
                remove(to - 2);
                place(to, p);
                place(to + 1, BR + wtm);
                break;
            }

            case F_NPROM: case F_BPROM: case F_RPROM: case F_QPROM:
            {
                const int prom = 2 * (flags - F_NPROM) + BN + wtm;

                remove(from);
                place(to, prom);
                break;
            }

            case F_NCAPPROM: case F_BCAPPROM: case F_RCAPPROM: case F_QCAPPROM:
            {
                const int prom = 2 * (flags - F_NCAPPROM) + BN + wtm;

                remove(from);
                remove(to);
                place(to, prom);
                break;
            }

            case F_EP:
            {
                const SQ cap = sq_(x_(to), y_(from));

                remove(cap);
                remove(from);
                place(to, p);
                break;
            }

            case F_PAWN2: // ---------- FALL THROUGH! ------------
            {
                state->en_passant = static_cast<SQ>((from + to) / 2);
            }

            default:
            {
                remove(from);
                place(to, p);

                if (!is_pawn(p)) state->fifty = old->fifty + 1;
            }
        }

        //state->hash ^= hash_ep[state->ep] ^ hash_castle[state->castling] ^ hash_wtm;
        wtm ^= 1;

        if (in_check(1))
        {
            unmake(move);
            return false;
        }

        switch (flags)
        {
            case F_KCASTLE: case F_QCASTLE:
            {
                const U64 o = occ[0] | occ[1];
                const SQ mid = static_cast<SQ>((from + to) / 2);

                if (is_attacked(from, o, 1)
                ||  is_attacked(mid, o, 1)
                ||  is_attacked(to, o, 1))
                {
                    unmake(move);
                    return false;
                }
            }
        }

        state->curr = move;

        return true;
    }

    void Board::unmake(const Move & move)
    {
        const Flags flags = get_flags(move);
        const SQ from = get_from(move);
        const SQ to = get_to(move);
        const Piece p = static_cast<Piece>(sq[to]);

        wtm ^= 1;

        switch (flags)
        {
            case F_CAP: 
            {
                assert(state->cap != NOP);
                remove<false>(to);
                place<false>(from, p);
                place<false>(to, state->cap);
                break;
            }

            case F_KCASTLE:
            {
                remove<false>(to - 1);
                remove<false>(to);
                place<false>(to + 1, BR + wtm);
                place<false>(from, p);
                break;
            }

            case F_QCASTLE:
            {
                remove<false>(to + 1);
                remove<false>(to);
                place<false>(to - 2, BR + wtm);
                place<false>(from, p);
                break;
            }

            case F_NPROM: case F_BPROM: case F_RPROM: case F_QPROM:
            {
                remove<false>(to);
                place<false>(from, BP + wtm);
                break;
            }

            case F_NCAPPROM: case F_BCAPPROM: case F_RCAPPROM: case F_QCAPPROM:
            {
                remove<false>(to);
                place<false>(from, BP + wtm);
                place<false>(to, state->cap);
                break;
            }

            case F_EP:
            {
                const SQ cap = sq_(x_(to), y_(from));

                remove<false>(to);
                place<false>(cap, p ^ 1);
                place<false>(from, p);
                break;
            }

            default:
            {
                remove<false>(to);
                place<false>(from, p);
            }
	    }

        state--;
    }

    Move Board::recognize(string move) const
    {
        if (move.size() < 4) return None;

        int x1 = recognize_x(tolower(move[0]));
        int y1 = recognize_y(move[1]);

        int x2 = recognize_x(tolower(move[2]));
        int y2 = recognize_y(move[3]);

        if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) return None;

        SQ from = sq_(x1, y1);
        SQ to = sq_(x2, y2);

        Piece pie = static_cast<Piece>(sq[from]);
        Piece cap = static_cast<Piece>(sq[to]);
        Flags flags = F_QUIET;

        if (is_pawn(pie))
        {
            if (wtm)
            {
                if      (y1 == 1 && y2 == 3) flags = F_PAWN2;   // Pawn double
                else if (to == state->en_passant) flags = F_EP; // En passant
                else if (y1 == 6)                               // Promotion
                {
                    if (move.size() < 5) return None;
                    flags = recognize_prom(move[4], cap != NOP);
                    if (flags == F_QUIET) return None;
                }
            }
            else
            {
                if      (y1 == 6 && y2 == 4) flags = F_PAWN2;   // Pawn double
                else if (to == state->en_passant) flags = F_EP; // En passant
                else if (y1 == 1)                               // Promotion
                {
                    if (move.size() < 5) return None;
                    flags = recognize_prom(move[4], cap != NOP);
                    if (flags == F_QUIET) return None;
                }
            }
        }
        else if (is_king(pie))
        {
            if      (to - from == 2) flags = F_KCASTLE;
            else if (from - to == 2) flags = F_QCASTLE;
        }

        if (flags == F_QUIET && cap != NOP) flags = F_CAP;

        return build_move(from, to, flags);
    }

    void Board::init_node(Move hash_move)
    {
        state->stage = Stage::Hash;
        state->ml.clear(hash_move);
    }

    Move Board::get_next_move()
    {
        while(true)
        {
            Move move = state->ml.get_next_move();
            if (move) return move;

            switch (state->stage)
            {
                case Stage::Hash:
                    generate<true>();
                    state->stage = Stage::Attacks;
                    break;

                case Stage::Attacks:
                    generate<false>();
                    state->stage = Stage::Quiet;
                    break;

                case Stage::Quiet:
                    state->stage = Stage::Ending;
                    break;

                case Stage::Ending:
                    return None;

                default:
                    throw exception("Stage generator is stalled");
            }
        }
        return None;
    }

    Move Board::get_next_move_qs()
    {
        while(true)
        {
            Move move = state->ml.get_next_move();
            if (move) return move;

            switch (state->stage)
            {
                case Stage::Hash:
                    generate<true>();
                    state->stage = Stage::Attacks;
                    break;

                case Stage::Attacks:
                    state->stage = Stage::Ending;
                    break;

                case Stage::Ending:
                    return None;

                default:
                    throw exception("Stage generator is stalled");
            }
        }
        return None;
    }

    bool Board::from_fen(string fen)
    {
        clear();

        stringstream ss(fen);
        string board, color, castling, ep;
        if (!(ss >> board >> color >> castling >> ep)) return false;

        parse_fen_board(board);
        parse_fen_color(color);
        parse_fen_castling(castling);
        parse_fen_ep(ep);

        calc_hash();

        int fifty;
        bool success = !!(ss >> fifty);
        state->fifty = success ? fifty : 0;
        return true;
    }

    void Board::set(const Board * board)
    {
        for (int i = 0; i < PIECE_N; i++)
            piece[i] = board->piece[i];

        for (int i = 0; i < SQUARE_N; i++)
            sq[i] = board->sq[i];

        occ[0] = board->occ[0];
        occ[1] = board->occ[1];

        wtm = board->wtm;

        state->en_passant = board->state->en_passant;
        state->castling = board->state->castling;
        state->fifty = board->state->fifty;
        state->hash = board->state->hash;
        state->cap = board->state->cap;
    }    

    void Board::parse_fen_board(string str)
    {
        SQ s = A8;
        for (char ch : str)
        {
            Piece p = NOP;
            switch(ch)
            {
                case 'p': p = BP; break;
                case 'P': p = WP; break;
                case 'n': p = BN; break;
                case 'N': p = WN; break;
                case 'b': p = BB; break;
                case 'B': p = WB; break;
                case 'r': p = BR; break;
                case 'R': p = WR; break;
                case 'q': p = BQ; break;
                case 'Q': p = WQ; break;
                case 'k': p = BK; break;
                case 'K': p = WK; break;
                case '/': continue;

                default:
                    if (ch > '0' && ch < '9')
                        s += ch - '0';
            }

            if (p < PIECE_N)
            {
                piece[p] |= BIT << s;
                sq[s] = p;
                ++s;
            }

            if (!(s & 7)) // End of row
            {
                if (s < 16) break;
                s -= 16;
            }
        }

        occ[0] = occ[1] = EMPTY;
        for (int i = BP; i < PIECE_N; i += 2)
        {
            occ[0] |= piece[i];
            occ[1] |= piece[i + 1];
        }
    }

    void Board::parse_fen_color(string str)
    {
        if (str.empty()) return;
        switch (str[0])
        {
            case 'w': case 'W': wtm = 1; break;
            case 'b': case 'B': wtm = 0; break;
        } 
    }

    void Board::parse_fen_castling(string str)
    {
        state->castling = static_cast<Castling>(0);
        for (char ch : str)
        {
            switch (ch)
            {
                case 'k': state->castling |= BOO;  break;
                case 'q': state->castling |= BOOO; break;
                case 'K': state->castling |= WOO;  break;
                case 'Q': state->castling |= WOOO; break;
            }
        }
    }

    void Board::parse_fen_ep(string str)
    {
        int x = 0, y = 0;
        for (char ch : str)
        {
            if (ch == '-') break;
            else if (ch >= 'a' && ch <= 'h') x = ch - 'a';
            else if (ch >= 'A' && ch <= 'H') x = ch - 'A';
            else if (ch == '3' || ch == '6') y = ch - '1';
        }
        state->en_passant = sq_(x, y);
    }

    void Board::calc_hash()
    {
        U64 key = EMPTY;
        for (int i = 0; i < PIECE_N; i++)
        {
            for (U64 bb = piece[i]; bb; bb = rlsb(bb))
            {
                SQ s = bitscan(bb);
                key ^= HT->hash_key[i][s];
            }
        }

        key ^= HT->hash_castle[state->castling];
        key ^= HT->hash_ep[state->en_passant];
        if (wtm) key ^= HT->hash_wtm;

        state->hash = key;
    }

    bool Board::check_consistency()
    {
        for (int i = 0; i < PIECE_N; i++)
        {
            for (U64 bb = piece[i]; bb; bb = rlsb(bb))
            {
                SQ s = bitscan(bb);
                if (sq[s] != i)
                {
                    cout << "check_consistency: sq <-> piece" << endl;
                    print();
                    print<true>();
                    //assert(false);
                    return false;
                }
            }
        }

        for (int s = 0; s < 64; s++)
        {
            int cnt = 0;
            for (int i = 0; i < PIECE_N; i++)
            {
                if (piece[i] & (1ull << s)) cnt++;
                if (cnt > 1)
                {
                    cout << "check_consistency: piece intersection" << endl;
                    cout << "sq " << static_cast<SQ>(s) << endl;
                    cout << "piece " << static_cast<Piece>(i) << endl;
                    print();
                    print<true>();
                    //assert(false);
                    return false;
                }
            }
        }
        return true;
    }

    void Board::restore_path(int ply)
    {
        for (int i = ply; state > 0 && i >= 0; state--, i--)
        {
            cout << state->curr << endl;
        }
    }

    U64 BoardInner::attack(Piece p, SQ sq) const
    {
        switch (p)
        {
            case BN: case WN: return attack<PieceType::KNIGHT>(sq);
            case BB: case WB: return attack<PieceType::BISHOP>(sq);
            case BR: case WR: return attack<PieceType::ROOK>(sq);
            case BQ: case WQ: return attack<PieceType::QUEEN>(sq);
            case BK: case WK: return attack<PieceType::KING>(sq);
        }
        return EMPTY;
    }
}
