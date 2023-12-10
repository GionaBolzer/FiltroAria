#include "state.h"

State Logica::change(Pressed button)
{
    if (button == Pressed::SHORT)
    {
        if (schermo == State::HOME)
        {
            timeSchermo = 5;
            schermo = State::MIN;
        }
        else if (schermo == State::MIN)
        {
            timeSchermo = 5;
            schermo = State::MAX;
        }
        else if (schermo == State::MINSEL)
        {
            timeSchermo += 5;
            schermo = State::MINSEL;
        }
        else if (schermo == State::MAX)
        {
            schermo = State::HOME;
        }
        else if (schermo == State::MAXSEL)
        {
            timeSchermo += 5;
            schermo = State::MAXSEL;
        }
        return schermo;
    }

    if (button == Pressed::LONG)
    {
        if (schermo == State::HOME)
        {
            timeSchermo = 5;
            timeHome = 0;
            timerHome = 0;
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
            timeHome = timeSchermo;
            timerMode = TimerMode::MINIMO;
            timerHome = millis();
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
            timeHome = timeSchermo;
            timerMode = TimerMode::MASSIMO;
            timerHome = millis();
            return schermo;
        }
    }
}
