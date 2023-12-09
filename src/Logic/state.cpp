#include "state.h"

State Logica::change(Pressed button)
{
    if (button == Pressed::SHORT)
    {
        if (schermo == State::HOME)
        {
            schermo = State::MIN;
        }
        else if (schermo == State::MIN)
        {
            schermo = State::MAX;
        }
        else if (schermo == State::MINSEL)
        {
            time += 5;
            schermo = State::MINSEL;
        }
        else if (schermo == State::MAX)
        {
            schermo = State::HOME;
        }
        else if (schermo == State::MAXSEL)
        {
            time += 5;
            schermo = State::MAXSEL;
        }
        return schermo;
    }

    if (button == Pressed::LONG)
    {
        if (schermo == State::HOME)
        {
            schermo = State::HOME;
            return schermo;
        }
        if (schermo == State::MIN)
        {
            schermo = State::MINSEL;
            return schermo;
        }
        if (schermo == State::MINSEL)
        {
            schermo = State::HOME;
            return schermo;
        }
        if (schermo == State::MAX)
        {
            schermo = State::MAXSEL;
            return schermo;
        }
        if (schermo == State::MAXSEL)
        {
            schermo = State::HOME;
            return schermo;
        }
    }
}
