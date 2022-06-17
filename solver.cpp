#include <windows.h>
#include <conio.h>
#include "solver.h"

namespace eia_v0_5
{
    bool Solver::input_available() const
    {
        DWORD mode;
        static HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        static BOOL console = GetConsoleMode(hInput, &mode);

        if (!console)
        {
            DWORD totalBytesAvail;
            if (!PeekNamedPipe(hInput, 0, 0, 0, &totalBytesAvail, 0)) return true;
            return totalBytesAvail;
        }
        else return _kbhit();
    }
}
