#include <iostream>
#include "magics.h"
#include "utils.h"
#include "types.h"
#include "bitboard.h"

using namespace std;

namespace eia_v0_5
{
    U64 r_att(U64 occ, SQ sq)
    {
        U64 * ptr = M->rTable[sq].ptr;
        occ      |= M->rTable[sq].notmask;
        occ      *= M->rTable[sq].blackmagic;
        occ     >>= 64 - 12;
        return ptr[occ];
    }

    U64 b_att(U64 occ, SQ sq)
    {
        U64 * ptr = M->bTable[sq].ptr;
        occ      |= M->bTable[sq].notmask;
        occ      *= M->bTable[sq].blackmagic;
        occ     >>= 64 - 9;
        return ptr[occ];
    }

    U64 Magics::index_to_u64(int index, int bits, U64 mask)
    {
        U64 result = ~mask;
        for (int i = 0; i < bits; i++)
        {
            int j = bitscan(mask);
            if (index & (BIT << i)) result |= BIT << j;
            mask = rlsb(mask);
        }
        return result;
    }

    Dir dirs[2][4] =
    {
        {Dir::U, Dir::D, Dir::L, Dir::R},   // Rook
        {Dir::DL, Dir::DR, Dir::UL, Dir::UR} // Bishop
    };

    U64 Magics::get_mask(bool bishop, SQ sq)
    {
        U64 result = 0ull;
        for (auto dir : dirs[+bishop])
        {
            U64 pre = 0ull;
            U64 bit = BIT << sq;

            while (true)
            {
                bit = shift(bit, dir);
                if (!bit) break;

                result |= pre;
                pre = bit;
            }
        }
        return result;
    }

    U64 Magics::get_att(bool bishop, SQ sq, U64 blocks)
    {
        U64 result = 0ull;
        for (auto dir : dirs[+bishop])
        {
            U64 bit = BIT << sq;
            while (true)
            {
                bit = shift(bit, dir);
                result |= bit;
                if (!bit || bit & blocks) break;
            }
        }
        return result;
    }

    inline int Magics::transform(U64 blocks, U64 magic, int bits)
    {
        return static_cast<int>((blocks * magic) >> (64 - bits));
    }

    void Magics::build_magics()
    {
        for (int sq = 0; sq < 64; sq++) // Rooks
        {
            U64 mask = get_mask(0, SQ(sq));
            U64 magic = rTable[sq].blackmagic;
            int bits = popcnt(mask);

            for (int i = 0; i < (1 << bits); i++)
            {
                U64 blocks = index_to_u64(i, bits, mask);
                int offset = transform(blocks, magic, 12);
                U64 * ptr  = rTable[sq].ptr;

                ptr[offset] = get_att(0, SQ(sq), blocks);
            };

            rTable[sq].notmask = ~mask;
            rTable[sq].blackmagic = magic;
        }

        for (int sq = 0; sq < 64; sq++) // Bishops
        {
            U64 mask = get_mask(1, SQ(sq));
            U64 magic = bTable[sq].blackmagic;
            int bits = popcnt(mask);

            for (int i = 0; i < (1 << bits); i++)
            {
                U64 blocks = index_to_u64(i, bits, mask);
                int offset = transform(blocks, magic, 9);
                U64 * ptr  = bTable[sq].ptr;

                ptr[offset] = get_att(1, SQ(sq), blocks);
            };

            bTable[sq].notmask = mask;
            bTable[sq].blackmagic = magic;
        }
        cout << endl;
    }

    Magics::Magics()
    {
        build_magics();
    }
}
