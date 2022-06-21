#include <sstream>
#include "moves.h"

using namespace std;

namespace eia_v0_5
{
    void History::clear()
    {
        for (int i = 0; i < PIECE_N; i++)
            for (int j = 0; j < SQUARE_N; j++)
                table[i][j] = 0;
    }

    void History::add(Piece p, SQ s, int depth)
    {
        table[p][s] += depth * depth;
        if (table[p][s] > 60000) shrink();
    }

    int History::get(Piece p, SQ s) const
    {
        return table[p][s];
    }

    void History::shrink()
    {
        for (int i = 0; i < PIECE_N; i++)
            for (int j = 0; j < SQUARE_N; j++)
                table[i][j] >>= 1;
    }

    ostream & operator << (ostream & os, const Move move)
    {
        os << get_from(move) << get_to(move);
        return os;
    }

    Flags recognize_prom(char ch, bool capture)
    {
        switch (ch)
        {
            case 'n': return capture ? F_NCAPPROM : F_NPROM;
            case 'b': return capture ? F_BCAPPROM : F_BPROM;
            case 'r': return capture ? F_RCAPPROM : F_RPROM;
            case 'q': return capture ? F_QCAPPROM : F_QPROM;
        }
        return F_QUIET;
    }
}
