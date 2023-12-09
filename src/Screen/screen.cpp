#include "screen.h"

Screen::Screen() : display(Adafruit_SH1106(OLED_RESET))
{
}

void Screen::begin()
{
    Serial.println("Schermo init");
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
}

void Screen::testIter(int change)
{
    if (millis() - timer > delayRefresh)
    {
        iter += change;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Iter: ");
        display.println(iter);
        display.display();
        timer = millis();
    }
}

void Screen::home(int pm2_5, int pm10, int power)
{
    display.clearDisplay();
    display.print("Pm 10: ");
    display.print(pm2_5);
    display.print(" u/m3");
    display.setCursor(0, 15);
    display.print("Pm 10: ");
    display.print(pm10);
    display.print(" u/m3");
    display.setCursor(0, 30);
    display.print("Power: ");
    display.print(power);
    display.print(" %");
    display.display();
    delay(2000);
    //     if (millis() - timer > delayRefresh)
    //     {

    //         display.clearDisplay();
    //         display.setCursor(10, 10);
    //         display.print("Pm 2.5" + char(pm2_5));
    //         display.setCursor(30, 10);
    //         display.print("Pm 10" + char(pm10));
    //         display.setCursor(40, 10);
    //         display.print("Power" + char(power));
    //         display.display();
    //         timer = millis();
    //     }
}