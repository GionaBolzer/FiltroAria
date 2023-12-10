#ifndef STATE_H
#define STATE_H

#include "../Utility/button.h"
#include "../Screen/screen.h"
enum State
{
    HOME,
    HOMETIMER,
    MIN,
    MINSEL,
    MAX,
    MAXSEL,
};

class Logica
{
    State schermo = State::HOME;

public:
    int time = 5;
    State change(Pressed button);
};

#endif