#include <iostream>
#include <cstdlib>
#include "bitboard.h"
#include "magics.h"
#include "engine.h"

using namespace eia_v0_5;
using namespace std;

BBTables * eia_v0_5::BBT;
Magics * eia_v0_5::M;

int main()
{
    BBT = new BBTables;
    M = new Magics;
    cout << ENG_NAME << " v" << ENG_VERS << " by " << ENG_AUTH << " (c) 2021\n";

    Engine * engine = new Engine(GameType::Playing);
    engine->start();

    system("pause");
    return 0;
}