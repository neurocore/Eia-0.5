#include <cassert>
#include <iostream>
#include <sstream>
#include "piece.h"
#include "board.h"
#include "array.h"

using namespace std;

namespace eia_v0_5
{
    void Board::print() const
    {
        const string pieces = "pPnNbBrRqQkK..";
        const string side = wtm ? "<W>" : "<B>";
        U64 occupied = occ[0] | occ[1];

        for (int y = 7; y >= 0; y--)
        {
            cout << RANKS[y] << "|";

            for (int x = 0; x < 8; x++)
            {
                SQ s = sq_(x, y);
                U64 bit = BIT << s;
                cout << ((bit & occupied) ? pieces[sq[s]] : '.');
            }

            if (y == 0) cout << " " << side;
            cout << "\n";
        }
        cout << " +--------\n  " << FILES << "\n\n";
    }

    void Board::clear()
    {
        wtm = 1;
        for (int i = 0; i < PIECE_N;  i++) piece[i] = EMPTY;
        for (int i = 0; i < COLOR_N;  i++) occ[i] = EMPTY;
        for (int i = 0; i < SQUARE_N; i++) sq[i] = NOP;
    }

    void Board::reset()
    {
        from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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
        return 0; // E->eval(inner);
    }

    bool Board::is_attacked(SQ king, U64 occupied, int opposite)
    {
        int col = wtm ^ opposite;
        if (moves(WN ^ col, king) & piece[BN ^ col]) return true; // Knights
        if (moves(WP ^ col, king) & piece[BP ^ col]) return true; // Pawns
        if (moves(WK ^ col, king) & piece[BK ^ col]) return true; // King

        if (b_att(occupied, king) & (piece[BB ^ col] | piece[BQ ^ col])) return true; // Bishops & queens
        if (r_att(occupied, king) & (piece[BR ^ col] | piece[BQ ^ col])) return true; // Rooks & queens

        return false;
    }

    bool Board::make(const Move & move)
    {
        const SQ from = get_from(move);
        const SQ to = get_to(move);

        state++;
        state->castling = (state - 1)->castling - from - to;
        state->hash = (state - 1)->hash; // ^ hash_ep[en_passant] ^ hash_castle[castling];
        state->cap = static_cast<Piece>(sq[to]);
        state->en_passant = A1;
        state->fifty = 0;

        state->best = Empty;
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

                if (!is_pawn(p)) state->fifty = (state - 1)->fifty + 1;
            }
        }

        //state->hash ^= hash_ep[state->ep] ^ hash_castle[state->castling] ^ hash_wtm;
        wtm ^= 1;

        const U64 o = occ[0] | occ[1];
        if (is_attacked(bitscan(piece[BK + wtm ^ 1]), o))
        {
            unmake(move);
            return false;
        }

        switch (flags)
        {
            case F_KCASTLE: case F_QCASTLE:
            {
                if (is_attacked(to, o) || is_attacked(static_cast<SQ>((from + to) / 2), o))
                {
                    unmake(move);
                    return false;
                }
            }
        }
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

        int val;
        bool success = !!(ss >> val);
        state->fifty = success ? val : 0;
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

            if (p < NOP)
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

    U64 Board::attack(Piece p, SQ sq)
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
