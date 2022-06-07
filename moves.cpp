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
}
