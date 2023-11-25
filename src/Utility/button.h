#include "debounce.h";

class Button
{
    int pinValue;
    Debounce debounce;
    int timer;

    bool is_pressing();

public:
    Button(int pinValue);
    Pressed scan();
}

enum Pressed {
    NO,
    SHORT,
    LONG,
}