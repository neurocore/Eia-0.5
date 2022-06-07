#include <intrin.h>
#include "utils.h"
#include "consts.h"
#include "bitboard.h"

using namespace std;

namespace eia_v0_5
{
    BBTables::BBTables()
    {
        for (int i = 0; i < 65536; i++)
        {
            int cnt = 0;
            for (U64 j = +i; j; j = rlsb(j)) cnt++;
            lut[i] = cnt;
        }
    }

    ostream & operator << (ostream & os, const BitBoard & bb)
    {
        for (int y = 7; y >= 0; y--)
        {
            os << "  ";
            for (int x = 0; x < 8; x++)
            {
                SQ sq = sq_(x, y);
                U64 bit = BIT << sq;
                char ch = (bb.val & bit) ? '*' : '.';
                os << ch;
            }
            os << endl;
        }
        os << endl;
        return os;
    }

    inline U64 lsb(U64 bb)
    {
        return bb & (EMPTY - bb);
    }

    inline U64 rlsb(U64 bb)
    {
        return bb & (bb - BIT);
    }

#ifdef SSE
    inline int popcnt(U64 bb)
    {
        return __popcnt64(bb);
    }

    inline SQ bitscan(U64 bb)
    {
        unsigned long btscn_indx;
        _BitScanForward64(&btscn_indx, bb);
        return static_cast<SQ>(btscn_indx);
    }
#else
    inline int popcnt(U64 bb)
    {
        int a =  bb >> 48;
        int b = (bb >> 32) & 0xFFFF;
        int c = (bb >> 16) & 0xFFFF;
        int d =  bb        & 0xFFFF;
        return BBT->lut[a] + BBT->lut[b] + BBT->lut[c] + BBT->lut[d];
    }

    inline SQ bitscan(U64 bb)
    {
        return static_cast<SQ>(BBT->btscn[(lsb(bb) * BBT->DEBRUIJN) >> 58]);
    }
#endif
}
