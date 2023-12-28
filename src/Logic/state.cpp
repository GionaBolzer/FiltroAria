#include "state.h"

State Logica::change(Pressed button)
{
    if(button == Pressed::NO){
        return schermo;
    }
    if (button == Pressed::SHORT)
    {
        if (schermo == State::HOME)
        {
            timeSchermo = 5;
            powerSchermo = 0;
            schermo = State::MODE;
        }
        else if (schermo == State::MODE)
        {
            schermo = State::HOME;
        }
        else if (schermo == State::MODESELTIMER)
        {
            timeSchermo += 5;
            schermo = State::MODESELTIMER;
        }
        else if (schermo == State::MODESELPOWER)
        {
            powerSchermo += 10;
            if(powerSchermo > 100){
                powerSchermo = 0;
            }
            schermo = State::MODESELPOWER;
        }
        return schermo;
    }

    if (button == Pressed::LONG)
    {
        if (schermo == State::HOME)
        {
            timeSchermo = 5;
            powerSchermo = 0;
            powerHome = 0;
            timeHome = 0;
            timerHome = 0;
            schermo = State::HOME;
            return schermo;
        }
        if (schermo == State::MODE)
        {
            schermo = State::MODESELTIMER;
            return schermo;
        }
        if (schermo == State::MODESELTIMER)
        {
            schermo = State::MODESELPOWER;
            return schermo;
        }
        if (schermo == State::MODESELPOWER)
        {
            schermo = State::HOME;
            timeHome = timeSchermo;
            powerHome = powerSchermo;
            timerHome = millis();
            return schermo;
        }
    }
}
