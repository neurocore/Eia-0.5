#pragma once
#include "types.h"

namespace eia_v0_5
{
    struct Array
    {
        Castling uncastle[64];

        constexpr Array() : uncastle()
        {
            for (int i = 0; i < 64; i++)
                uncastle[i] = CALL;

	        uncastle[A1] = static_cast<Castling>(CALL ^ (WOOO));
	        uncastle[E1] = static_cast<Castling>(CALL ^ (WOOO | WOO));
	        uncastle[H1] = static_cast<Castling>(CALL ^ (WOO));

	        uncastle[A8] = static_cast<Castling>(CALL ^ (BOOO));
	        uncastle[E8] = static_cast<Castling>(CALL ^ (BOOO | BOO));
	        uncastle[H8] = static_cast<Castling>(CALL ^ (BOO));
        }
    };

    extern Array * ARR;
}
