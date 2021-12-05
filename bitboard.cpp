#include <intrin.h>
#include "consts.h"
#include "bitboard.h"

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

    inline int bitscan(U64 bb)
    {
        unsigned long btscn_indx;
        _BitScanForward64(&btscn_indx, bb);
        return btscn_indx;
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

    inline int bitscan(U64 bb)
    {
        return BBT->btscn[(lsb(bb) * BBT->DEBRUIJN) >> 58];
    }
#endif
}
