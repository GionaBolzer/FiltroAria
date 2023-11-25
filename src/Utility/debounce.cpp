#include "debounce.h"
#include "Arduino.h"

Debounce::Debounce(int PinValue)
{
    _pinValue = PinValue;
    _delayDebounce = 150;
    _state = true;
    pinMode(PinValue, INPUT_PULLUP);
}

bool Debounce::scan()
{
    if (digitalRead(_pinValue) == LOW && _state)
    {
        timer = millis();
        _state = false;
        return false; // Not pressed (yet)
    }

    if (digitalRead(_pinValue) == LOW && !_state && int(millis()) >= timer + _delayDebounce)
    {
        _state = true;
        return true; // Pressed
    }

    return false; // Not pressed
}