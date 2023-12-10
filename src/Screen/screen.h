#ifndef SCREEN_H
#define SCREEN_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define OLED_RESET 4

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

class Screen
{
    unsigned delayRefresh = 200;
    unsigned long timer = 0;
    Adafruit_SH1106 display;

public:
    int time = 0;
    Screen();
    void begin();
    void home(int pm2_5, int pm10, int power, int Timer);
    void paginaMin(int timer);
    void paginaMax(int timer);
    void minsel(int timer);
    void maxsel(int timer);
};
#endif