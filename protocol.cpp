#include <iostream>
#include <sstream>
#include <string>
#include "consts.h"
#include "protocol.h"

using namespace std;

namespace eia_v0_5
{
    Protocol * Protocol::detect()
    {
        string input;
        while(true)
        {
            getline(cin, input);
            if (input == "uci") return new UCI();
            if (input == "quit") return nullptr;
            if (input == "exit") return nullptr;
            cout << "Eia supports protocols: UCI" << endl;
        }
        return nullptr;
    }

    void UCI::greet()
    {
        cout << "id name " << ENG_NAME << " v" << ENG_VERS << endl;
        cout << "id author " << ENG_AUTH << endl;
        cout << "option name Hash type spin default 4 min 1 max 128" << endl;
        cout << "option name Nullmove type check default true" << endl;
        cout << "uciok" << endl;
    }

    void UCI::read()
    {
        string input, word;
        getline(cin, input);

        stringstream ss(input);
        ss >> word;

        if (word == "quit")
        {
            commands.write_cmd(Cmd::Quit);
        }
        else if (word == "go")
        {
            int time;
            if (ss >> time)
            {
                commands.write_cmd(Cmd::Go);
                commands.write_int(time);
            }
        }
        else if (word == "perft")
        {
            int depth;
            if (!(ss >> depth)) depth = 1;

            commands.write_cmd(Cmd::Perft);
            commands.write_int(depth);
        }
        else
        {

        }
    }
}
