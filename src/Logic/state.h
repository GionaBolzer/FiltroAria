#ifndef STATE_H
#define STATE_H

#include "../Utility/button.h"

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
    int time = 0;
    State change(Pressed button);
};

#endif