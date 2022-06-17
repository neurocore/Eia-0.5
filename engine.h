#pragma once
#include <functional>
#include <queue>
#include "board.h"
#include "solver.h"

using std::function;
using std::queue;

namespace eia_v0_5
{
    enum GameType { Playing, Learning };

    struct Protocol;
    class Engine
    {
        queue<CommandGo> deferred_go;
        State states[2];
        Board * B;
        Player * S[2];
        Protocol * P;

    public:
        explicit Engine(GameType gt);
        ~Engine();
        void start();
        bool read_input();

        void defer_go(const CommandGo & go);
        void cmd_setpos(string fen);
        void cmd_setmove(string move);
        void cmd_stop();

        void cmd_newgame();
        void cmd_setoption(string name, int val);
        void cmd_setdebug(bool val);
        void cmd_perft(int depth);
        void cmd_quit();

        enum State { Waiting, Thinking, Error };

    private:
        void cmd_go(const CommandGo & go);
        void execute_for(function<void(Solver *)> f);

        State state = State::Waiting;
    };
}
