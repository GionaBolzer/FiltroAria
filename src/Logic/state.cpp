#include "state.h"

State Logica::change(Pressed button)
{
    if (button == Pressed::SHORT)
    {
        if (schermo == State::HOME)
        {
            time = 5;
            schermo = State::MIN;
        }
        else if (schermo == State::MIN)
        {
            time = 5;
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
        else if (schermo == State::MAX)
        {
            schermo = State::HOMETIMER;
        }
        else if (schermo == State::MAXSEL)
        {
            time += 5;
            schermo = State::MAXSEL;
        }
        if (schermo == State::HOMETIMER)
        {
            time = 5;
            schermo = State::MIN;
            return schermo;
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
        if (schermo == State::HOMETIMER)
        {
            time = 5;
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
            schermo = State::HOMETIMER;
            return schermo;
        }
        if (schermo == State::MAX)
        {
            schermo = State::MAXSEL;
            return schermo;
        }
        if (schermo == State::MAXSEL)
        {
            schermo = State::HOMETIMER;
            return schermo;
        }
    }
}
