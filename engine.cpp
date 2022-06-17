#include <exception>
#include "array.h"
#include "engine.h"
#include "protocol.h"
#include "solver_pvs.h"

using namespace std;

namespace eia_v0_5
{
    Engine::Engine(GameType gt)
    {
        PT = new PieceTables;
        BBT = new BBTables;
        ARR = new Array;
        M = new Magics;
        B = new Board(states);

        switch (gt)
        {
            case Playing:
                S[0] = new SolverPVS();
                S[1] = new Reader();
                break;

            case Learning:
                S[0] = new SolverPVS();
                S[1] = new SolverPVS();
                break;

            default:
                throw logic_error("Game type isn't implemented in engine class");
        }
    }

    Engine::~Engine()
    {
        delete S[1];
        delete S[0];
        delete B;
        delete M;
        delete ARR;
        delete BBT;
        delete PT;
    }

    void Engine::start()
    {
        B->reset();
        B->print();

        /*P = Protocol::detect();
        if (P == nullptr)
        {
            cout << "have a nice day" << endl;
            state = State::Error;
            return;
        }
        P->greet();*/

        P = new UCI();
        while (P->parse(this));
        delete P;
    }

    void Engine::cmd_go(MS wtime, MS btime, MS winc, MS binc, bool infinite)
    {
        MS time = B->to_move() ? wtime + winc : btime + binc;
        cout << "go " << static_cast<int>(time)
             << "ms" << endl;

        execute_for([&](Solver * solver)
        {
            solver->set(B);
            solver->analysis(infinite);
            solver->get_move(time);
        });
    }

    void Engine::cmd_setpos(string fen)
    {
        B->from_fen(fen);
    }

    void Engine::cmd_setmove(string move)
    {
        Move mv = B->recognize(move);
        if (mv == Empty) return;

        B->make(mv, true);
        B->print();
    }

    void Engine::cmd_stop()
    {
        execute_for([](Solver * solver) { solver->stop(); });
    }

    void Engine::cmd_newgame()
    {
        cout << "newgame" << endl;
    }

    void Engine::cmd_setoption(string name, int val)
    {
        cout << "set [" << name << " = " << val << "]" << endl;
    }

    void Engine::cmd_setdebug(bool val)
    {
        execute_for([val](Solver * solver) { solver->debug(val); });
    }

    void Engine::cmd_perft(int depth)
    {
        S[0]->set(B);
        S[0]->perft(depth);
    }

    void Engine::cmd_quit()
    {
        cout << "glad to see you again" << endl;
    }

    void Engine::execute_for(function<void(Solver *)> f)
    {
        for (int i = 0; i < 2; i++)
        {
            Solver * solver = dynamic_cast<Solver *>(S[i]);
            if (solver == nullptr) continue;

            f(solver);
        }
    }
}
