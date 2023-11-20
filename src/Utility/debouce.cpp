#include "debouce.h"
#include "Arduino.h"

Debounce::Debounce(int PinValue){
    _pinValue = PinValue;
    _delayDebounce = 50 ;
    _state = true;
    pinMode(PinValue, INPUT_PULLUP);
}

Debounce::scan(){
    if(digitalRead(_pinValue) == LOW) //
    {
        timer = millis();
        _state = false;
    }
    if(digitalRead(_pinValue) == LOW && !_state && int(millis())>= timer + _delayDebounce){
        _state = true;
        return 0;
    }
    return 1;
}