#include "screen.h"

Screen::Screen(): display(Adafruit_SH1106(OLED_RESET))
{
}

void Screen::home(int change)
{
    iter += change;
    display.clearDisplay();
    // display.setTextSize(1);
    display.setCursor(63, 32);
    display.println(iter);
    display.display();
}