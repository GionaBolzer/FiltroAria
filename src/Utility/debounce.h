class Debounce
{
    int _pinValue;
    int _delayDebounce;
    bool _state; // set to false if button pressed
    int timer;

public:
    Debounce(int pinValue);
    bool scan(); // return 1 if button pass debouce
};