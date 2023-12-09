#include "button.h"
#include "Arduino.h"

Button::Button(int _pinValue) : pinValue(_pinValue), debLow(Debounce(_pinValue, false)), debHigh(_pinValue, true)
{
}

Pressed Button::scan()
{
    if (timer <= 0)
    {
        if (debLow.scan())
        {
            
            timer = millis();
            return Pressed::NO;
        }

        return Pressed::NO;
    }

    if (!returned && millis() >= 500 + timer)
    {
        returned = true;
        return Pressed::LONG;
    }

    if (debHigh.scan()) // button not pressed anymore
    {
        if (returned)
        {
            timer = 0;
            returned = false;
            return Pressed::NO;
        }

        timer = 0;
        return Pressed::SHORT;
    }

    // button still pressed, but from less than 2000ms
    return Pressed::NO;
};
