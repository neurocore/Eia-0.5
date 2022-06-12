#include <sstream>
#include "moves.h"

using namespace std;

namespace eia_v0_5
{
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
