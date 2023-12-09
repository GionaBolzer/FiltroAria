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
    iter += change;
    if (millis() - timer > delayRefresh)
    {
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

    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Pm 2.5: ");
        display.print(pm2_5);
        display.println(" u/m3");

        // display.setCursor(0, 15);
        display.print("Pm 10: ");
        display.print(pm10);
        display.println(" u/m3");

        // display.setCursor(0, 30);
        display.print("Power: ");
        display.print(power);
        display.print(" %");
        display.display();
    }
}

void Screen::paginaMin()
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Modalita min");
        display.display();
        timer = millis();
    }
}
void Screen::paginaMax()
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Modalita max");
        display.display();
        timer = millis();
    }
}
void Screen::minsel(int timer)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Modalita min sel");
        display.print("timer ;");
        display.print(timer);
        display.display();
        timer = millis();
    }
}
void Screen::maxsel(int timer)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Modalita max sel");
        display.print("timer ;");
        display.print(timer);
        display.display();
        timer = millis();
    }
}