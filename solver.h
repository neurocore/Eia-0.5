#pragma once
#include "player.h"

namespace eia_v0_5
{
    class Engine;

    // Solver is the player that performs calculations
    //  to make a decision about its next move
    class Solver : public Player
    {
    protected:
        Engine * engine = nullptr;
        mutable MS last_response = 0;
        mutable bool thinking  = false;
        mutable bool debugging = false;
        mutable bool infinite  = false;

    public:
        Solver(Engine * engine) : engine(engine), Player() {}
        void stop() { thinking = false; }
        void debug(bool val) { debugging = val; }
        void analysis(bool val) { infinite = val; }
        bool input_available() const;
    };

    class CommandGo
    {
        MS wtime_ = TIME_DEFAULT;
        MS btime_ = TIME_DEFAULT;
        MS winc_ = INC_DEFAULT;
        MS binc_ = INC_DEFAULT;
        bool infinite_ = false;

    public:
        CommandGo(MS wtime, MS btime, MS winc, MS binc, bool infinite = false)
            : wtime_(wtime), btime_(btime), winc_(winc), binc_(binc), infinite_(infinite)
        {}

        MS wtime() const { return wtime_; }
        MS btime() const { return btime_; }
        MS winc() const { return winc_; }
        MS binc() const { return binc_; }
        bool infinite() const { return infinite_; }
    };
}
