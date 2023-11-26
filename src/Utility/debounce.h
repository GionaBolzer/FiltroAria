class Debounce
{
    int _pinValue;
    int _delayDebounce;
    bool _state; // set to false if button pressed
    int timer;
    bool _mode;

public:
    Debounce(int pinValue, bool mode);
    bool scan(); // return true if button is in `mode`
};