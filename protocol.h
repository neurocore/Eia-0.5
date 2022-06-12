#pragma once
#include <stack>
#include <iostream>
#include <sstream>
#include "engine.h"

namespace eia_v0_5
{
    struct Protocol
    {
        static Protocol * detect();
        virtual void greet() = 0;
        virtual bool parse(Engine * E) = 0;
    };

    struct UCI : public Protocol
    {
        virtual void greet() override;
        virtual bool parse(Engine * E) override;
    };
}
