#include "solver_pvs.h"
#include "tests.h"

void Tests::register_tests()
{
    TEST("MatedTest")
    {
        State S[2];
        Board B(S);

        B.from_fen("1r3bkr/ppp1p1pp/4Q3/2P1P1B1/1P5N/P7/5RPP/1N1R2K1 b - - 0 27");
        
        SolverPVS solver;
        solver.set(&B);
        U64 cnt = solver.perft_inner(1);

        return B.in_check() && cnt == 0;
    };
}
