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
    display.clearDisplay();
}

void Screen::home(int pm2_5, int pm10, int power, int Timer)
{
    if (millis() - timer > delayRefresh || timer == 0)
    {
        display.clearDisplay();
        display.setCursor(5, 10);
        display.print("Pm 2.5: ");
        display.print(pm2_5);
        display.println(" u/m3");
        display.setCursor(5, 25);
        display.print("Pm 10: ");
        display.print(pm10);
        display.println(" u/m3");
        display.setCursor(5, 40);
        display.print("Power: ");
        display.print(power);
        display.println(" %");
        if (Timer != 0)
        {
            // utente ha seleziona un temo dalla modalità
            display.setCursor(5, 55);
            display.print("Timer: ");
            display.print(Timer);
        }
        timer = millis();
        display.display();
    }
}

void Screen::paginaMin(int Timer)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(35, 0);
        display.setTextSize(1);
        display.println("MIN MODE");
        display.setTextSize(1);
        display.setCursor(35, 35);
        display.print("Timer: ");
        display.print(Timer);
        display.display();
        timer = millis();
    }
}
void Screen::paginaMax(int Timer)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(35, 0);
        display.setTextSize(1);
        display.println("MAX MODE");
        display.setTextSize(1);
        display.setCursor(35, 35);
        display.print("Timer: ");
        display.print(Timer);
        display.display();
        timer = millis();
    }
}
void Screen::minsel(int Timer)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(35, 0);
        display.setTextSize(1);
        display.println("MIN MODE");
        display.setTextSize(1);
        display.setCursor(35, 35);
        display.print("Timer: ");
        display.print(Timer);
        display.drawRoundRect(30, 32, 70, 13, 3, WHITE);
        display.display();
        timer = millis();
    }
}
void Screen::maxsel(int Timer)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(35, 0);
        display.setTextSize(1);
        display.println("MAX MODE");
        display.setTextSize(1);
        display.setCursor(35, 35);
        display.print("Timer: ");
        display.print(Timer);
        display.drawRoundRect(30, 32, 70, 13, 3, WHITE);
        display.display();
        timer = millis();
    }
}