#include <iostream>
#include <cstdlib>
#include "bitboard.h"
#include "piece.h"
#include "magics.h"
#include "engine.h"

using namespace eia_v0_5;
using namespace std;

PieceTables * eia_v0_5::PT;
BBTables * eia_v0_5::BBT;
Magics * eia_v0_5::M;

int main()
{
    PT = new PieceTables;
    BBT = new BBTables;
    M = new Magics;
    cout << ENG_NAME << " v" << ENG_VERS << " by " << ENG_AUTH << " (c) 2021\n";

    U64 att = r_att((BIT << B4) | (BIT << E7), E4);
    cout << BitBoard{att};

    cout << BitBoard{between(B4, E7)};

    Engine * engine = new Engine(GameType::Playing);
    engine->start();

    delete engine;
    delete M;
    delete ARR;
    delete BBT;
    delete PT;

    _CrtDumpMemoryLeaks();
    return 0;
}