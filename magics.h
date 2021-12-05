#pragma once
#include "types.h"

namespace eia_v0_5
{
    class Magics // Black Magics were discovered by Volker Annuss
    {
		enum SlidingPiece { Rook, Bishop };
        struct Entry
        {
            U64 * ptr;
            U64 notmask;
            U64 blackmagic;
        };

		U64 rand64_few();
		U64 index_to_u64(int index, int bits, U64 mask);
		U64 get_mask(SlidingPiece sp, SQ sq);
		U64 get_att(SlidingPiece sp, SQ sq, U64 block);

	public:
		Magics();
        inline U64 r_att(U64 occ, SQ sq);
        inline U64 b_att(U64 occ, SQ sq);

	private:
        U64 att_table[88507];
        Entry bTable[64] =
		{
			{ att_table + 66157, 0ull, 0x107ac08050500bffull },
			{ att_table + 71730, 0ull, 0x7fffdfdfd823fffdull },
			{ att_table + 37781, 0ull, 0x0400c00fe8000200ull },
			{ att_table + 21015, 0ull, 0x103f802004000000ull },
			{ att_table + 47590, 0ull, 0xc03fe00100000000ull },
			{ att_table +   835, 0ull, 0x24c00bffff400000ull },
			{ att_table + 23592, 0ull, 0x0808101f40007f04ull },
			{ att_table + 30599, 0ull, 0x100808201ec00080ull },
			{ att_table + 68776, 0ull, 0xffa2feffbfefb7ffull },
			{ att_table + 19959, 0ull, 0x083e3ee040080801ull },
			{ att_table + 21783, 0ull, 0x040180bff7e80080ull },
			{ att_table + 64836, 0ull, 0x0440007fe0031000ull },
			{ att_table + 23417, 0ull, 0x2010007ffc000000ull },
			{ att_table + 66724, 0ull, 0x1079ffe000ff8000ull },
			{ att_table + 74542, 0ull, 0x7f83ffdfc03fff80ull },
			{ att_table + 67266, 0ull, 0x080614080fa00040ull },
			{ att_table + 26575, 0ull, 0x7ffe7fff817fcff9ull },
			{ att_table + 67543, 0ull, 0x7ffebfffa01027fdull },
			{ att_table + 24409, 0ull, 0x20018000c00f3c01ull },
			{ att_table + 30779, 0ull, 0x407e0001000ffb8aull },
			{ att_table + 17384, 0ull, 0x201fe000fff80010ull },
			{ att_table + 18778, 0ull, 0xffdfefffde39ffefull },
			{ att_table + 65109, 0ull, 0x7ffff800203fbfffull },
			{ att_table + 20184, 0ull, 0x7ff7fbfff8203fffull },
			{ att_table + 38240, 0ull, 0x000000fe04004070ull },
			{ att_table + 16459, 0ull, 0x7fff7f9fffc0eff9ull },
			{ att_table + 17432, 0ull, 0x7ffeff7f7f01f7fdull },
			{ att_table + 81040, 0ull, 0x3f6efbbf9efbffffull },
			{ att_table + 84946, 0ull, 0x0410008f01003ffdull },
			{ att_table + 18276, 0ull, 0x20002038001c8010ull },
			{ att_table +  8512, 0ull, 0x087ff038000fc001ull },
			{ att_table + 78544, 0ull, 0x00080c0c00083007ull },
			{ att_table + 19974, 0ull, 0x00000080fc82c040ull },
			{ att_table + 23850, 0ull, 0x000000407e416020ull },
			{ att_table + 11056, 0ull, 0x00600203f8008020ull },
			{ att_table + 68019, 0ull, 0xd003fefe04404080ull },
			{ att_table + 85965, 0ull, 0x100020801800304aull },
			{ att_table + 80524, 0ull, 0x7fbffe700bffe800ull },
			{ att_table + 38221, 0ull, 0x107ff00fe4000f90ull },
			{ att_table + 64647, 0ull, 0x7f8fffcff1d007f8ull },
			{ att_table + 61320, 0ull, 0x0000004100f88080ull },
			{ att_table + 67281, 0ull, 0x00000020807c4040ull },
			{ att_table + 79076, 0ull, 0x00000041018700c0ull },
			{ att_table + 17115, 0ull, 0x0010000080fc4080ull },
			{ att_table + 50718, 0ull, 0x1000003c80180030ull },
			{ att_table + 24659, 0ull, 0x2006001cf00c0018ull },
			{ att_table + 38291, 0ull, 0xffffffbfeff80fdcull },
			{ att_table + 30605, 0ull, 0x000000101003f812ull },
			{ att_table + 37759, 0ull, 0x0800001f40808200ull },
			{ att_table +  4639, 0ull, 0x084000101f3fd208ull },
			{ att_table + 21759, 0ull, 0x080000000f808081ull },
			{ att_table + 67799, 0ull, 0x0004000008003f80ull },
			{ att_table + 22841, 0ull, 0x08000001001fe040ull },
			{ att_table + 66689, 0ull, 0x085f7d8000200a00ull },
			{ att_table + 62548, 0ull, 0xfffffeffbfeff81dull },
			{ att_table + 66597, 0ull, 0xffbfffefefdff70full },
			{ att_table + 86749, 0ull, 0x100000101ec10082ull },
			{ att_table + 69558, 0ull, 0x7fbaffffefe0c02full },
			{ att_table + 61589, 0ull, 0x7f83fffffff07f7full },
			{ att_table + 62533, 0ull, 0xfff1fffffff7ffc1ull },
			{ att_table + 64387, 0ull, 0x0878040000ffe01full },
			{ att_table + 26581, 0ull, 0x005d00000120200aull },
			{ att_table + 76355, 0ull, 0x0840800080200fdaull },
			{ att_table + 11140, 0ull, 0x100000c05f582008ull }
		};

        Entry rTable[64] =
		{
			{ att_table + 10890, 0ull, 0x80280013ff84ffffull },
			{ att_table + 56054, 0ull, 0x5ffbfefdfef67fffull },
			{ att_table + 67495, 0ull, 0xffeffaffeffdffffull },
			{ att_table + 72797, 0ull, 0x003000900300008aull },
			{ att_table + 17179, 0ull, 0x0030018003500030ull },
			{ att_table + 63978, 0ull, 0x0020012120a00020ull },
			{ att_table + 56650, 0ull, 0x0030006000c00030ull },
			{ att_table + 15929, 0ull, 0xffa8008dff09fff8ull },
			{ att_table + 55905, 0ull, 0x7fbff7fbfbeafffcull },
			{ att_table + 26301, 0ull, 0x0000140081050002ull },
			{ att_table + 78100, 0ull, 0x0000180043800048ull },
			{ att_table + 86245, 0ull, 0x7fffe800021fffb8ull },
			{ att_table + 75228, 0ull, 0xffffcffe7fcfffafull },
			{ att_table + 31661, 0ull, 0x00001800c0180060ull },
			{ att_table + 38053, 0ull, 0xffffe7ff8fbfffe8ull },
			{ att_table + 37433, 0ull, 0x0000180030620018ull },
			{ att_table + 74747, 0ull, 0x00300018010c0003ull },
			{ att_table + 53847, 0ull, 0x0003000c0085ffffull },
			{ att_table + 70952, 0ull, 0xfffdfff7fbfefff7ull },
			{ att_table + 49447, 0ull, 0x7fc1ffdffc001fffull },
			{ att_table + 62629, 0ull, 0xfffeffdffdffdfffull },
			{ att_table + 58996, 0ull, 0x7c108007befff81full },
			{ att_table + 36009, 0ull, 0x20408007bfe00810ull },
			{ att_table + 21230, 0ull, 0x0400800558604100ull },
			{ att_table + 51882, 0ull, 0x0040200010080008ull },
			{ att_table + 11841, 0ull, 0x0010020008040004ull },
			{ att_table + 25794, 0ull, 0xfffdfefff7fbfff7ull },
			{ att_table + 49689, 0ull, 0xfebf7dfff8fefff9ull },
			{ att_table + 63400, 0ull, 0xc00000ffe001ffe0ull },
			{ att_table + 33958, 0ull, 0x2008208007004007ull },
			{ att_table + 21991, 0ull, 0xbffbfafffb683f7full },
			{ att_table + 45618, 0ull, 0x0807f67ffa102040ull },
			{ att_table + 70134, 0ull, 0x200008e800300030ull },
			{ att_table + 75944, 0ull, 0x0000008780180018ull },
			{ att_table + 68392, 0ull, 0x0000010300180018ull },
			{ att_table + 66472, 0ull, 0x4000008180180018ull },
			{ att_table + 23236, 0ull, 0x008080310005fffaull },
			{ att_table + 19067, 0ull, 0x4000188100060006ull },
			{ att_table +     0, 0ull, 0xffffff7fffbfbfffull },
			{ att_table + 43566, 0ull, 0x0000802000200040ull },
			{ att_table + 29810, 0ull, 0x20000202ec002800ull },
			{ att_table + 65558, 0ull, 0xfffff9ff7cfff3ffull },
			{ att_table + 77684, 0ull, 0x000000404b801800ull },
			{ att_table + 73350, 0ull, 0x2000002fe03fd000ull },
			{ att_table + 61765, 0ull, 0xffffff6ffe7fcffdull },
			{ att_table + 49282, 0ull, 0xbff7efffbfc00fffull },
			{ att_table + 78840, 0ull, 0x000000100800a804ull },
			{ att_table + 82904, 0ull, 0xfffbffefa7ffa7feull },
			{ att_table + 24594, 0ull, 0x0000052800140028ull },
			{ att_table +  9513, 0ull, 0x00000085008a0014ull },
			{ att_table + 29012, 0ull, 0x8000002b00408028ull },
			{ att_table + 27684, 0ull, 0x4000002040790028ull },
			{ att_table + 27901, 0ull, 0x7800002010288028ull },
			{ att_table + 61477, 0ull, 0x0000001800e08018ull },
			{ att_table + 25719, 0ull, 0x1890000810580050ull },
			{ att_table + 50020, 0ull, 0x2003d80000500028ull },
			{ att_table + 41547, 0ull, 0xfffff37eefefdfbeull },
			{ att_table +  4750, 0ull, 0x40000280090013c1ull },
			{ att_table +  6014, 0ull, 0xbf7ffeffbffaf71full },
			{ att_table + 41529, 0ull, 0xfffdffff777b7d6eull },
			{ att_table + 84192, 0ull, 0xeeffffeff0080bfeull },
			{ att_table + 33433, 0ull, 0xafe0000fff780402ull },
			{ att_table +  8555, 0ull, 0xee73fffbffbb77feull },
			{ att_table +  1009, 0ull, 0x0002000308482882ull }
		};
    };
	extern Magics * M;
}
