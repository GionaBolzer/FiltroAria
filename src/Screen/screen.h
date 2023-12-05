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
    int iter;
    Adafruit_SH1106 display;

public:
    Screen();
    void home(int i);
};