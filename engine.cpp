#include <exception>
#include "engine.h"
#include "protocol.h"
#include "solver_pvs.h"

using namespace std;

namespace eia_v0_5
{
    Engine::Engine(GameType gt)
    {
        B = new Board;

        switch (gt)
        {
            case Playing:
                S[0] = new SolverPVS(B);
                S[1] = new Reader(B);
                break;

            case Learning:
                S[0] = new SolverPVS(B);
                S[1] = new SolverPVS(B);
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
    }

    void Engine::start()
    {
        B->reset();
        B->print();
        P = Protocol::detect();
        if (P == nullptr)
        {
            cout << "have a nice day" << endl;
            state = State::Error;
            return;
        }

        P->greet();
        while (true)
        {
            P->read();
            if (!execute()) break;
        }
    }

    bool Engine::execute()
    {
        Cmd cmd = P->commands.read_cmd();
        switch (cmd)
        {
            case NewGame:
            {
                break;
            }
            case Stop:
            {
                break;
            }
            case Quit:
            {
                cout << "glad to see you again" << endl;
                return false;
            }
            case SetPosition:
            {
                break;
            }
            case SetMove:
            {
                break;
            }
            case SetOption:
            {
                break;
            }
            case Go:
            {
                int time = P->commands.read_int();
                cout << "go " << time << "ms" << endl;
                break;
            }
            case Perft:
            {
                int depth = P->commands.read_int();
                cout << "perft " << depth << " ply" << endl;
                break;
            }
        }
        return true;
    }
}