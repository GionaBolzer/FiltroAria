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

class Logica
{
    State schermo = State::HOME;

public:
    int timeSchermo = 5;
    int timeHome = 0;
    State change(Pressed button);
};

#endif