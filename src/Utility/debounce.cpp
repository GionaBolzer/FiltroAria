#include "debounce.h"
#include "Arduino.h"

// mode low (false) or high (true)
Debounce::Debounce(int PinValue, bool mode)
{
    _pinValue = PinValue;
    _delayDebounce = 25;
    _state = true;
    pinMode(PinValue, INPUT_PULLUP);
    _mode = mode;
}

bool Debounce::scan()
{
    if (digitalRead(_pinValue) == _mode && _state)
    {
        timer = millis();
        _state = false;
        return false; // Not pressed (yet)
    }

    if (digitalRead(_pinValue) == _mode && !_state && int(millis()) >= timer + _delayDebounce)
    {
        _state = true;
        return true; // Pressed
    }

    return false; // Not pressed
}