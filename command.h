#pragma once
#include <string>
#include <queue>
#include "moves.h"

using std::string;
using std::queue;

namespace eia_v0_5
{
    enum Cmd : int
    {
        None, Quit,
        NewGame, Stop,
        SetPosition, SetMove,
        SetOption,
        Go, Perft
    };

    class CommandStream
    {
        queue<int> stream;

    public:
        Cmd read_cmd();
        int read_int();
        string read_string();
        Move read_move();

        void write_cmd(Cmd cmd);
        void write_int(int val);
        void write_string(string str);
        void write_move(Move move);
    };
}