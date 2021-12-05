#include <exception>
#include "command.h"
#include "engine.h"

using namespace std;

namespace eia_v0_5
{
    Cmd CommandStream::read_cmd()
    {
        if (stream.empty()) return Cmd::None;
        Cmd cmd = static_cast<Cmd>(stream.front());
        stream.pop();
        return cmd;
    }

    int CommandStream::read_int()
    {
        if (stream.empty()) throw exception("Expected int value in bytecode");
        int val = stream.front();
        stream.pop();

        return val;
    }

    string CommandStream::read_string()
    {
        string str;
        while (true)
        {
            if (stream.empty()) throw exception("Expected string value in bytecode");
            char ch = +stream.front();
            stream.pop();
            if (ch == 0) break;

            str += ch;
        }
        return str;
    }

    Move CommandStream::read_move()
    {
        if (stream.empty()) throw exception("Expected move value in bytecode");
        int val = stream.front();
        Move move = Move::unpack(val);
        stream.pop();
        return move;
    }

    void CommandStream::write_cmd(Cmd cmd)
    {
        stream.push(static_cast<U8>(cmd));
    }

    void CommandStream::write_int(int val)
    {
        stream.push(val);
    }

    void CommandStream::write_string(std::string str)
    {
        for (char ch : str)
            stream.push(static_cast<int>(ch));
        stream.push(0);
    }

    void CommandStream::write_move(Move move)
    {
        int val = move.pack();
        stream.push(val);
    }
}