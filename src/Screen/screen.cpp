#include "screen.h"

Screen::Screen(): display(Adafruit_SH1106(OLED_RESET))
{
}

void Screen::testIter(int change)
{
    iter += change;
    display.clearDisplay();
    // display.setTextSize(1);
    display.setCursor(63, 32);
    display.println(iter);
    display.display();
}

void Screen::home(int pm2_5, int pm10, int power)
{
    if(millis() - timer > delayRefresh){

        display.clearDisplay();
        display.setCursor(10,10);
        display.print("Pm 2.5" + char(pm2_5));
        display.setCursor(30,10);
        display.print("Pm 10" + char(pm10));
        display.setCursor(40,10);
        display.print("Power" + char(power));
        display.display();
        timer = millis();
    }
}