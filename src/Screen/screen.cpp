#include "screen.h"

Screen::Screen() : display(Adafruit_SH1106(OLED_RESET))
{
}

void Screen::begin()
{

    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.clearDisplay();
}

void Screen::home(int pm2_5, int pm10, int power, int Timer, bool error)
{
    if (millis() - timer > delayRefresh || timer == 0)
    {
        display.clearDisplay();
        if (error)
        {
            display.setCursor(5, 10);
            display.print("Pm 2.5: ");
            display.print("No data");
            display.println(" u/m3");
            display.setCursor(5, 25);
            display.print("Pm 10: ");
            display.print("No data");
            display.println(" u/m3");
        }
        else
        {
            display.setCursor(5, 10);
            display.print("Pm 2.5: ");
            display.print(pm2_5);
            display.println(" u/m3");
            display.setCursor(5, 25);
            display.print("Pm 10: ");
            display.print(pm10);
            display.println(" u/m3");
        }

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

void Screen::paginaMode(int Timer, int power)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(50, 0);
        display.setTextSize(1);
        display.println("MODE");
        display.setTextSize(1);
        display.setCursor(35, 25);
        display.print("Timer: ");
        display.print(Timer);
        display.setCursor(35, 40);
        display.print("Power: ");
        display.print(power);
        display.display();
        timer = millis();
    }
}
void Screen::paginaModeSelTimer(int Timer, int power)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(50, 0);
        display.setTextSize(1);
        display.println("MODE");
        display.setTextSize(1);
        display.setCursor(35, 25);
        display.print("Timer: ");
        display.print(Timer);
        display.drawRoundRect(30, 22, 70, 13, 3, WHITE);
        display.setCursor(35, 40);
        display.print("Power: ");
        display.print(power);
        display.display();
        timer = millis();
    }
}
void Screen::paginaModeSelPower(int Timer, int power)
{
    if (millis() - timer > delayRefresh)
    {
        display.clearDisplay();
        display.setCursor(50, 0);
        display.setTextSize(1);
        display.println("MODE");
        display.setTextSize(1);
        display.setCursor(35, 25);
        display.print("Timer: ");
        display.print(Timer);
        display.setCursor(35, 40);
        display.print("Power: ");
        display.print(power);
        display.drawRoundRect(30, 37, 70, 13, 3, WHITE);
        display.display();
        timer = millis();
    }
}
