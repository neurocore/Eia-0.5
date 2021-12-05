#pragma once
#include <stack>
#include <iostream>
#include <sstream>
#include "command.h"

namespace eia_v0_5
{
    struct Protocol
    {
        CommandStream commands;

        static Protocol * detect();
        virtual void greet() = 0;
        virtual void read() = 0;
    };

    struct UCI : public Protocol
    {
        virtual void greet();
        virtual void read();
    };

}