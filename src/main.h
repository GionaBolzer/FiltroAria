#include <Arduino.h>
#include <SoftwareSerial.h>
#include "PMS.h"
#include "pindef.h"
#include "./Utility/debounce.h"

///////////////////////////////////// SENSOR ////////////////////////////////////////////
#ifdef PMSENSOR
SoftwareSerial pmsSerial(RxPin, TxPin);
PMS pms(pmsSerial);
PMS::DATA data;
#define GET_PM_READ_EVERY 120000 // wake sensore every 60s
#define WAIT_PM 30000            // wait 30s for sensor to stabilize
unsigned long sensorTimer = 0;
bool SENSOR_STATE = false; // if false sensor sleep
uint32_t PM_READ = 0;
#endif

#ifdef FAN
#define CHANGE_FAN 60000 // wait 30s for sensor to stabilize
unsigned long fanTimer = 0;
const byte OC1A_PIN = 9;
const byte OC1B_PIN = 10;

const word PWM_FREQ_HZ = 25000; // Adjust this value to adjust the frequency
const word TCNT1_TOP = 16000000 / (2 * PWM_FREQ_HZ);

uint16_t fanPower = 20;   // default power is 20 %
uint16_t minPower = 20;   // min power fan at 20%
uint16_t maxPower = 100;  // max power fan at 100%
uint32_t minPowerPM = 0;  // set pm read at wich the fan is at min value
uint32_t maxPowerPM = 38; // set pm read at wich the fan is at max value
#endif

void wakeUpSensor(); // wake uo every 120s

void readSensor();

void setPwmDuty(byte duty);

void power(uint32_t read);