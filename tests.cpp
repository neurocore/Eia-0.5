#include "solver_pvs.h"
#include "eval_basic.h"
#include "tests.h"

using namespace std;

void Tests::register_tests()
{
    TEST("MatedTest")
    {
        Engine * engine = new Engine(GameType::Playing);
        State S[2];
        Board B(S);

        B.from_fen("1r3bkr/ppp1p1pp/4Q3/2P1P1B1/1P5N/P7/5RPP/1N1R2K1 b - - 0 27");
        
        SolverPVS solver(engine);
        solver.set(&B);
        U64 cnt = solver.perft_inner(1);

        return B.in_check() && cnt == 0;
    };

    TEST("PasserTest")
    {
        Engine * engine = new Engine(GameType::Playing);
        State S[2];
        Board B(S);

        B.from_fen("8/1p1p4/8/1PPP4/8/7k/8/7K w - - 0 1");

        Eval * E = new EvalBasic;
        int val = B.eval(E);

        return val > 105;
    };
}
