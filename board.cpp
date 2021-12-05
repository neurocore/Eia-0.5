#include <iostream>
#include <sstream>
#include "board.h"

using namespace std;

namespace eia_v0_5
{
    void Board::print() const
    {
        const string pieces = "pPnNbBrRqQkK..";
        const string side = inner.wtm ? "<W>" : "<B>";
        U64 occupied = inner.occ[0] | inner.occ[1];

        for (int y = 7; y >= 0; y--)
        {
            cout << RANKS[y] << "|";

            for (int x = 0; x < 8; x++)
            {
                SQ s = sq_(x, y);
                U64 bit = BIT << s;
                cout << (bit & occupied ? pieces[inner.sq[s]] : '.');
            }

            if (y == 0) cout << " " << side;
            cout << "\n";
        }
        cout << " +--------\n  " << FILES << "\n\n";
    }

    void Board::clear()
    {
        inner.wtm = 1;
        for (int i = 0; i < PIECE_N;  i++) inner.piece[i] = EMPTY;
        for (int i = 0; i < COLOR_N;  i++) inner.occ[i] = EMPTY;
        for (int i = 0; i < SQUARE_N; i++) inner.sq[i] = NOP;
    }

    void Board::reset()
    {
        from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    string Board::make_fen()
    {
        return string();
    }

    string Board::prettify(const Move & move)
    {
        stringstream ss;
        int piece = inner.sq[move.from()];
        ss << "pPnNbBrRqQkK?"[piece];
        ss << move.from();
        ss << move.to();
        return ss.str();
    }

    int Board::eval(const Eval * E) const
    {
        return E->eval(inner);
    }

    bool Board::make(const Move & move)
    {

        return false;
    }

    void Board::unmake(const Move & move)
    {
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
        inner.fifty = success ? val : 0;
        return true;
    }

    void Board::parse_fen_board(string str)
    {
        SQ sq = A8;
        for (char ch : str)
        {
            Piece piece = NOP;
            switch(ch)
            {
                case 'p': piece = BP; break;
                case 'P': piece = WP; break;
                case 'n': piece = BN; break;
                case 'N': piece = WN; break;
                case 'b': piece = BB; break;
                case 'B': piece = WB; break;
                case 'r': piece = BR; break;
                case 'R': piece = WR; break;
                case 'q': piece = BQ; break;
                case 'Q': piece = WQ; break;
                case 'k': piece = BK; break;
                case 'K': piece = WK; break;
                case '/': continue;

                default:
                    if (ch > '0' && ch < '9')
                        sq += ch - '0';
            }

            if (piece < NOP)
            {
                inner.piece[piece] |= BIT << sq;
                inner.sq[sq] = piece;
                sq++;
            }

            if (!(sq & 7)) // End of row
            {
                if (sq < 16) break;
                sq -= 16;
            }
        }

        inner.occ[0] = inner.occ[1] = EMPTY;
        for (int i = BP; i < PIECE_N; i += 2)
        {
            inner.occ[0] |= inner.piece[i];
            inner.occ[1] |= inner.piece[i + 1];
        }
    }

    void Board::parse_fen_color(string str)
    {
        if (str.empty()) return;
        switch (str[0])
        {
            case 'w': case 'W': inner.wtm = 1; break;
            case 'b': case 'B': inner.wtm = 0; break;
        } 
    }

    void Board::parse_fen_castling(string str)
    {
        inner.castling = static_cast<Castling>(0);
        for (char ch : str)
        {
            switch (ch)
            {
                case 'k': inner.castling |= BOO;  break;
                case 'q': inner.castling |= BOOO; break;
                case 'K': inner.castling |= WOO;  break;
                case 'Q': inner.castling |= WOOO; break;
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
        inner.en_passant = sq_(x, y);
    }
}