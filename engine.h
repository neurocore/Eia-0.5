#pragma once
#include "board.h"
#include "player.h"
#include "protocol.h"
#include "command.h"

namespace eia_v0_5
{
    enum GameType { Playing, Learning };

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
        bool execute();

        enum State { Waiting, Thinking, Error };

    private:
        State state = State::Waiting;
    };
}