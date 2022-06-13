#pragma once
#include <functional>
#include "board.h"
#include "player.h"

using std::function;

namespace eia_v0_5
{
    enum GameType { Playing, Learning };

    struct Protocol;
    class Engine
    {
        State states[2];
        Board * B;
        Player * S[2];
        Protocol * P;

    public:
        explicit Engine(GameType gt);
        ~Engine();
        void start();

        void cmd_go(MS wtime, MS btime, MS winc, MS binc, bool infinite = false);
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
        void execute_for(function<void(Solver *)> f);

        State state = State::Waiting;
    };
}
