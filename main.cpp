#include <iostream>
#include "bitboard.h"
#include "array.h"
#include "piece.h"
#include "magics.h"
#include "engine.h"
#include "tests.h"
#include "hash.h"

using namespace eia_v0_5;
using namespace std;

PieceTables * eia_v0_5::PT;
HashTables * eia_v0_5::HT;
BBTables * eia_v0_5::BBT;
Magics * eia_v0_5::M;
Array * eia_v0_5::ARR;

int main()
{
    cout << ENG_NAME << " v" << ENG_VERS << " by " << ENG_AUTH << " (c) 2021-2022\n";

    Engine * engine = new Engine(GameType::Playing);

    U64 att = r_att((BIT << B4) | (BIT << E7), E4);
    cout << BitBoard{att};
    cout << BitBoard{between(B4, E7)};

#ifdef _DEBUG
    Tests tests(engine);
    if (!tests.run()) return 1;
#endif

    engine->start();
    delete engine;

    _CrtDumpMemoryLeaks();
    return 0;
}
