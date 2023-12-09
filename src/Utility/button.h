#ifndef BUTTON_H
#define BUTTON_H
#include "debounce.h"

enum Pressed
{
    NO,
    SHORT,
    LONG
};

class Button
{
    int pinValue;
    Debounce debLow;
    Debounce debHigh;
    unsigned long timer;
    bool returned = false;

public:
    Button(int _pinValue);
    Pressed scan();
};

#endif