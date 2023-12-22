#include <Arduino.h>
#include <SoftwareSerial.h>
#include "PMS.h"
#include "pindef.h"
#include "./Utility/button.h"
#include "./Screen/screen.h"
#include "./Logic/state.h"
///////////////////////////////////// SENSOR ////////////////////////////////////////////
#ifdef SCREEN
#define REFRESH_HOME 1000        // refresh screen home every second
unsigned long homeTimer = 0;
#endif
uint32_t PM_READ = 0;
#ifdef PMSENSOR
SoftwareSerial pmsSerial(RxPin, TxPin);
PMS pms(pmsSerial);
PMS::DATA data;
#define GET_PM_READ_EVERY 120000 // wake sensore every 60s
#define WAIT_PM 30000            // wait 30s for sensor to stabilize
unsigned long sensorTimer = 0;
bool SENSOR_STATE = false; // if false sensor sleep

#endif

uint16_t fanPower = 20;

#ifdef FAN
#define CHANGE_FAN 60000 // wait 30s for sensor to stabilize
unsigned long fanTimer = 0;
const byte OC1A_PIN = 9;
const byte OC1B_PIN = 10;

const word PWM_FREQ_HZ = 25000; // Adjust this value to adjust the frequency
const word TCNT1_TOP = 16000000 / (2 * PWM_FREQ_HZ);

   // default power is 20 %
uint16_t minPower = 20;   // min power fan at 20%
uint16_t maxPower = 100;  // max power fan at 100%
uint32_t minPowerPM = 0;  // set pm read at wich the fan is at min value
uint32_t maxPowerPM = 10; // set pm read at wich the fan is at max value
#endif

unsigned long FORCE_POWER = 0;

void wakeUpSensor(); // wake uo every 120s

void readSensor();

void setPwmDuty(byte duty);

void power(uint32_t read);

void mydisplay();
void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h);
void testdrawchar(void);
void testdrawcircle(void);
void testfillrect(void);
void testdrawtriangle(void);
void testfilltriangle(void);
void testdrawroundrect(void);
void testfillroundrect(void);
void testdrawrect(void);
void testdrawline();
void refreshHome();
void PwmArd();