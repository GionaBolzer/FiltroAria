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

    // button still pressed
    if (digitalRead(pinValue) == LOW)
    {
        return Pressed::NO;
    }

    if (millis() - timer < 200)
    {
        return Pressed::SHORT;
    }

    return Pressed::LONG;
}

bool Button::is_pressing()
{
    return timer > 0;
}