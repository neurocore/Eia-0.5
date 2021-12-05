#include "magics.h"
#include "utils.h"
#include "types.h"
#include "bitboard.h"

namespace eia_v0_5
{
    inline U64 Magics::r_att(U64 occ, SQ sq)
    {
        U64 * ptr = rTable[sq].ptr;
        occ      |= rTable[sq].notmask;
        occ      *= rTable[sq].blackmagic;
        occ     >>= 64 - 12;
        return ptr[occ];
    }

    inline U64 Magics::b_att(U64 occ, SQ sq)
    {
        U64 * ptr = bTable[sq].ptr;
        occ      |= bTable[sq].notmask;
        occ      *= bTable[sq].blackmagic;
        occ     >>= 64 - 9;
        return ptr[occ];
    }

    U64 Magics::rand64_few()
    {
        return rand64() & rand64() & rand64();
    }

    U64 Magics::index_to_u64(int index, int bits, U64 mask)
    {
        U64 result = 0ull;
        for (int i = 0; i < bits; i++)
        {
            int j = bitscan(mask);
            if (index & (BIT << i)) result |= BIT << j;
            mask = rlsb(mask);
        }
        return result;
    }

    const U64 outer_mask = RANK_1 | RANK_8 | FILE_A | FILE_H;
    const int dirs[2][4][2] =
    {
        {{1, 0}, {0, 1}, {-1, 0}, {0, -1}},   // Rook
        {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}} // Bishop
    };

    U64 Magics::get_mask(SlidingPiece sp, SQ sq)
    {
        U64 result = 0ull;
        for (auto dir : dirs[+sp])
        {
            int x = x_(sq);
            int y = y_(sq);

            while (true)
            {
                x += dir[0];
                y += dir[1];
                U64 bit = BIT << sq_(x, y);
                if (bit & outer_mask) break;

                result |= bit;
            }
        }
        return ~result;
    }

    U64 Magics::get_att(SlidingPiece sp, SQ sq, U64 block)
    {
        U64 result = 0ull;
        for (auto dir : dirs[+sp])
        {
            int x = x_(sq);
            int y = y_(sq);

            while (true)
            {
                x += dir[0];
                y += dir[1];
                U64 bit = BIT << sq_(x, y);
                result |= bit;
                if (bit & block) break;
            }
        }
        return result;
    }

    Magics::Magics()
    {
        // TODO: init masks and attacks
    }
}