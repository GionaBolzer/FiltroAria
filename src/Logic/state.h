#ifndef STATE_H
#define STATE_H

#include "../Utility/button.h"
#include "../Screen/screen.h"
enum State
{
    HOME,
    MIN,
    MINSEL,
    MAX,
    MAXSEL,
};

enum TimerMode
{
    MINIMO,
    MASSIMO,
};

class Logica
{
    

public:
    TimerMode timerMode;
    State schermo = State::HOME;
    int timeSchermo = 5;
    int timeHome = 0;
    unsigned long timerHome = 0;
    State change(Pressed button);
};

#endif