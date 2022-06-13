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

    bool UCI::parse(Engine * E)
    {
        string input, word;
        getline(cin, input);

        stringstream ss(input + " 0");
        ss >> word;

        if (word == "go")
        {
            bool infinite = false;
            int wtime = TIME_DEFAULT;
            int btime = TIME_DEFAULT;
            int winc = INC_DEFAULT;
            int binc = INC_DEFAULT;

            string arg;
            int val;
            while (ss >> arg >> val)
            {
                if      (arg == "wtime") wtime = val;
                else if (arg == "btime") btime = val;
                else if (arg == "winc") winc = val;
                else if (arg == "binc") binc = val;
                else if (arg == "infinite") infinite = true;
            }

            E->cmd_go(wtime, btime, winc, binc, infinite);
        }
        else if (word == "position")
        {
            // [fen #fenstring | startpos ] moves #move1 .... #movei
            string arg;
            ss >> arg;

            if (arg == "startpos")
            {
                E->cmd_setpos(STARTPOS);
            }
            else if (arg == "fen")
            {
                string fen;
                getline(ss, fen, 'm');

                if (!fen.empty())
                {
                    E->cmd_setpos(fen);
                }
            }

            ss >> arg;
            if (arg == "moves" || arg == "oves")
            {
                string move;
                while (ss >> move)
                {
                    E->cmd_setmove(move);
                }
            }
        }
        else if (word == "fen")
        {
            string fen;
            getline(ss, fen);
            if (!fen.empty())
            {
                E->cmd_setpos(fen);
            }
        }
        else if (word == "isready")
        {
            cout << "readyok" << endl;
        }
        else if (word == "stop")
        {
            E->cmd_stop();
        }
        else if (word == "ucinewgame")
        {
            E->cmd_newgame();
        }
        else if (word == "setoption")
        {
            string arg, name;
            if (ss >> arg >> name && arg == "name")
            {
                int value;
                if (ss >> arg >> value && arg == "value")
                {
                    E->cmd_setoption(name, value);
                }
            }
        }
        else if (word == "debug")
        {
            string arg;
            if (ss >> arg)
            {
                E->cmd_setdebug(arg == "on" ? 1 : 0);
            }
        }
        else if (word == "perft")
        {
            int depth = 1;
            ss >> depth;

            E->cmd_perft(depth);
        }
        else if (word == "quit")
        {
            E->cmd_quit();
            return false;
        }
        else
        {
            cout << "Can't recognize command \"" << word << "\"" << endl;
        }
        return true;
    }
}
