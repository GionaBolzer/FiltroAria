#include "button.h"

Button::Button(int _pinValue)
{
    debounce = Debounce(_pinValue);
    pinValue = _pinValue
}

Pressed Button::scan()
{
    if (!is_pressing())
    {
        if (debounce.scan())
        {
            timer = millis();
            return Pressed::NO;
        }

        return Pressed::NO;
    }

    if (millis() >= 2000 + timer)
    {
        return Pressed::LONG;
    }

    if (digitalRead(pinValue) == HIGH)
    {
        timer = 0;
    }
    return Pressed::SHORT
}

bool Button::is_pressing()
{
    return timer > 0;
}