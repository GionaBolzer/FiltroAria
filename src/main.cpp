#include "main.h"

Button button(BUTTON_PIN); // costruisco classe su bottone pin
Screen display;

void setup()
{
#ifdef SCREEN
    mydisplay();
#endif

#ifdef LOG
    Serial.begin(115200);
    Serial.println("Welcome home, Sir");
#endif

#ifdef PMSENSOR
    pmsSerial.begin(9600);
    pms.passiveMode();
#endif
#ifdef FAN
    pinMode(OC1A_PIN, OUTPUT);

    // Clear Timer1 control and count registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << CS10);
    ICR1 = TCNT1_TOP;
    setPwmDuty(fanPower);
#endif
}

void loop()
{
#ifdef PMSENSOR
    wakeUpSensor();
    readSensor();
#endif

#ifdef FAN
    power(PM_READ);
    setPwmDuty(fanPower); // Change this value 0-100 to adjust duty cycle
#endif

    Pressed b = button.scan();
    if (b == Pressed::SHORT)
    {
        display.testIter(1);
        // Serial.println("pulsante short");
        //  FORCE_POWER = 0;
    }
    else if (b == Pressed::LONG)
    {
        display.testIter(-1);
        // Serial.println("pulsante long");
        //  FORCE_POWER = millis();
    }
}

#ifdef PMSENSOR
void wakeUpSensor() // wake uo every 120s
{
    if (millis() > sensorTimer && !SENSOR_STATE)
    {
        pms.wakeUp();
        SENSOR_STATE = true;
#ifdef LOG
        Serial.println("Waking up, wait 30 seconds for stable readings...: ");
#endif
    }
}

void readSensor()
{
    if (millis() > (sensorTimer + WAIT_PM) && SENSOR_STATE) // 30s after wake up
    {
        pms.requestRead();
#ifdef LOG
        Serial.println("Send read request...");
        Serial.println("Reading data...");
#endif
        if (pms.readUntil(data))
        {
#ifdef LOG
            Serial.print("PM 1.0 (ug/m3): ");
            Serial.println(data.PM_AE_UG_1_0);
            Serial.print("PM 2.5 (ug/m3): ");
            Serial.println(data.PM_AE_UG_2_5);
            Serial.print("PM 10.0 (ug/m3): ");
            Serial.println(data.PM_AE_UG_10_0);
#endif
            PM_READ = data.PM_AE_UG_2_5;
        }
        else
        {
#ifdef LOG
            Serial.println("No data.");
#endif
        }
#ifdef LOG
        Serial.println("Going to sleep for 120 seconds.");
#endif
        pms.sleep();
        SENSOR_STATE = false;
        sensorTimer = millis() + GET_PM_READ_EVERY; // update timer
    }
}
#endif

#ifdef FAN
void setPwmDuty(byte duty)
{
    OCR1A = (word)(duty * TCNT1_TOP) / 100;
}

void power(uint32_t read)
{
    if (FORCE_POWER != 0)
    {
        if (millis() - FORCE_POWER < 30000 * 60)
        {
            fanPower = 100;
        }
        else
        {
            FORCE_POWER = 0;
            fanPower = 20;
        }
    }
    else
    {
        if (millis() > fanTimer)
        {
            float power = (float(maxPower - minPower) / float(maxPowerPM - minPowerPM)) * (float(read - minPowerPM)) + float(minPower); // retta passatper per (minPowerPM, minPower) e (maxPowerPM, maxPower)

            if (power > float(maxPower))
            {
                power = maxPower;
            }
            if (power < float(minPower))
            {
                power = minPower;
            }
            fanPower = int(power);
            fanTimer = millis() + CHANGE_FAN;

#ifdef LOG
            Serial.print("power: ");
            Serial.println(fanPower);
#endif
        }
    }
}
#endif

void mydisplay()
{
    //     // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    //     display.begin(SH1106_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    //     // init done

    //     // // Show image buffer on the display hardware.
    //     // // Since the buffer is intialized with an Adafruit splashscreen
    //     // // internally, this will display the splashscreen.
    //     display.display();
    //     delay(2000);

    //     // // Clear the buffer.
    //     display.clearDisplay();

    //     // // draw a single pixel
    //     // display.drawPixel(10, 10, WHITE);
    //     // // Show the display buffer on the hardware.
    //     // // NOTE: You _must_ call display after making any drawing commands
    //     // // to make them visible on the display hardware!
    //     // display.display();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // // draw many lines
    //     // testdrawline();
    //     // display.display();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // // draw rectangles
    //     // testdrawrect();
    //     // display.display();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // // draw multiple rectangles
    //     // testfillrect();
    //     // display.display();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // // draw mulitple circles
    //     // testdrawcircle();
    //     // display.display();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // // draw a white circle, 10 pixel radius
    //     // display.fillCircle(display.width() / 2, display.height() / 2, 10, WHITE);
    //     // display.display();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // testdrawroundrect();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // testfillroundrect();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // testdrawtriangle();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // testfilltriangle();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // // draw the first ~12 characters in the font
    //     // testdrawchar();
    //     // display.display();
    //     // delay(2000);
    //     // display.clearDisplay();

    //     // draw scrolling text
    //     /* testscrolltext();
    //      delay(2000);
    //      display.clearDisplay();*/

    //     // text display tests
    //     display.setTextSize(1);
    //     display.setTextColor(WHITE);
    //     display.setCursor(0, 0);
    //     display.println("Ciao Cire");
    //     delay(2000);
    //     display.setTextColor(BLACK, WHITE);
    //     delay(2000); // 'inverted' text
    //     display.println(3.141592);
    //     delay(2000);
    //     display.setTextSize(2);
    //     display.setTextColor(WHITE);
    //     display.display();
    //     delay(2000);

    //     // miniature bitmap display
    //     display.clearDisplay();
    //     display.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, 1);
    //     display.display();

    //     // invert the display
    //     display.invertDisplay(true);
    //     delay(1000);
    //     display.invertDisplay(false);
    //     // delay(1000);

    //     // draw a bitmap icon and 'animate' movement
    //     // testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
    //     display.clearDisplay();
    //     display.display();
}

// void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h)
// {
//     uint8_t icons[NUMFLAKES][3];

//     // initialize
//     for (uint8_t f = 0; f < NUMFLAKES; f++)
//     {
//         icons[f][XPOS] = random(display.width());
//         icons[f][YPOS] = 0;
//         icons[f][DELTAY] = random(5) + 1;

//         Serial.print("x: ");
//         Serial.print(icons[f][XPOS], DEC);
//         Serial.print(" y: ");
//         Serial.print(icons[f][YPOS], DEC);
//         Serial.print(" dy: ");
//         Serial.println(icons[f][DELTAY], DEC);
//     }

//     while (1)
//     {
//         // draw each icon
//         for (uint8_t f = 0; f < NUMFLAKES; f++)
//         {
//             display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
//         }
//         display.display();
//         delay(200);

//         // then erase it + move it
//         for (uint8_t f = 0; f < NUMFLAKES; f++)
//         {
//             display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
//             // move it
//             icons[f][YPOS] += icons[f][DELTAY];
//             // if its gone, reinit
//             if (icons[f][YPOS] > display.height())
//             {
//                 icons[f][XPOS] = random(display.width());
//                 icons[f][YPOS] = 0;
//                 icons[f][DELTAY] = random(5) + 1;
//             }
//         }
//     }
// }

// void testdrawchar(void)
// {
//     display.setTextSize(1);
//     display.setTextColor(WHITE);
//     display.setCursor(0, 0);

//     for (uint8_t i = 0; i < 168; i++)
//     {
//         if (i == '\n')
//             continue;
//         display.write(i);
//         if ((i > 0) && (i % 21 == 0))
//             display.println();
//     }
//     display.display();
// }

// void testdrawcircle(void)
// {
//     for (int16_t i = 0; i < display.height(); i += 2)
//     {
//         display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
//         display.display();
//     }
// }

// void testfillrect(void)
// {
//     uint8_t color = 1;
//     for (int16_t i = 0; i < display.height() / 2; i += 3)
//     {
//         // alternate colors
//         display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
//         display.display();
//         color++;
//     }
// }

// void testdrawtriangle(void)
// {
//     for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5)
//     {
//         display.drawTriangle(display.width() / 2, display.height() / 2 - i,
//                              display.width() / 2 - i, display.height() / 2 + i,
//                              display.width() / 2 + i, display.height() / 2 + i, WHITE);
//         display.display();
//     }
// }

// void testfilltriangle(void)
// {
//     uint8_t color = WHITE;
//     for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5)
//     {
//         display.fillTriangle(display.width() / 2, display.height() / 2 - i,
//                              display.width() / 2 - i, display.height() / 2 + i,
//                              display.width() / 2 + i, display.height() / 2 + i, WHITE);
//         if (color == WHITE)
//             color = BLACK;
//         else
//             color = WHITE;
//         display.display();
//     }
// }

// void testdrawroundrect(void)
// {
//     for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
//     {
//         display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, WHITE);
//         display.display();
//     }
// }

// void testfillroundrect(void)
// {
//     uint8_t color = WHITE;
//     for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
//     {
//         display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
//         if (color == WHITE)
//             color = BLACK;
//         else
//             color = WHITE;
//         display.display();
//     }
// }

// void testdrawrect(void)
// {
//     for (int16_t i = 0; i < display.height() / 2; i += 2)
//     {
//         display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
//         display.display();
//     }
// }

// void testdrawline()
// {
//     for (int16_t i = 0; i < display.width(); i += 4)
//     {
//         display.drawLine(0, 0, i, display.height() - 1, WHITE);
//         display.display();
//     }
//     for (int16_t i = 0; i < display.height(); i += 4)
//     {
//         display.drawLine(0, 0, display.width() - 1, i, WHITE);
//         display.display();
//     }
//     delay(250);

//     display.clearDisplay();
//     for (int16_t i = 0; i < display.width(); i += 4)
//     {
//         display.drawLine(0, display.height() - 1, i, 0, WHITE);
//         display.display();
//     }
//     for (int16_t i = display.height() - 1; i >= 0; i -= 4)
//     {
//         display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
//         display.display();
//     }
//     delay(250);

//     display.clearDisplay();
//     for (int16_t i = display.width() - 1; i >= 0; i -= 4)
//     {
//         display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
//         display.display();
//     }
//     for (int16_t i = display.height() - 1; i >= 0; i -= 4)
//     {
//         display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
//         display.display();
//     }
//     delay(250);

//     display.clearDisplay();
//     for (int16_t i = 0; i < display.height(); i += 4)
//     {
//         display.drawLine(display.width() - 1, 0, 0, i, WHITE);
//         display.display();
//     }
//     for (int16_t i = 0; i < display.width(); i += 4)
//     {
//         display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
//         display.display();
//     }
//     delay(250);
// }

// /*void testscrolltext(void) {
//   display.setTextSize(2);
//   display.setTextColor(WHITE);
//   display.setCursor(10,0);
//   display.clearDisplay();
//   display.println("scroll");
//   display.display();

//   display.startscrollright(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrollleft(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrolldiagright(0x00, 0x07);
//   delay(2000);
//   display.startscrolldiagleft(0x00, 0x07);
//   delay(2000);
//   display.stopscroll();
// }*/