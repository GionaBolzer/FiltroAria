class Debounce {        
  public:
    int _pinValue;
    int _delayDebounce;
    bool _state; // set to false if button pressed
    int timer; 
    Debounce(int pinValue);
    int scan(); // return 1 if button pass debouce
};