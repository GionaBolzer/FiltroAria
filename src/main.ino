#include <Arduino.h>
#include <SoftwareSerial.h>
#include "PMS.h"

///////////////////////////////////// SENSOR ////////////////////////////////////////////
#ifdef PMSENSOR
SoftwareSerial pmsSerial(2, 3);
PMS pms(pmsSerial);
PMS::DATA data;
#define GET_PM_READ_EVERY 120000 // wake sensore every 60s
#define WAIT_PM 30000            // wait 30s for sensor to stabilize
unsigned long sensorTimer = 0;
bool SENSOR_STATE = false; // if false sensor sleep
uint32_t PM_READ = 0;
#endif

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

///////////////////////////////////// FAN ////////////////////////////////////////////

#ifdef FAN
#define CHANGE_FAN 60000 // wait 30s for sensor to stabilize
unsigned long fanTimer = 0;
const byte OC1A_PIN = 9;
const byte OC1B_PIN = 10;

const word PWM_FREQ_HZ = 25000; // Adjust this value to adjust the frequency
const word TCNT1_TOP = 16000000 / (2 * PWM_FREQ_HZ);

uint16_t fanPower = 20; // default power is 20 %

void setPwmDuty(byte duty)
{
    OCR1A = (word)(duty * TCNT1_TOP) / 100;
}

void power(uint32_t read)
{
    if (millis() > fanTimer)
    {
        float power = 0.88f * float(read) + 11.1f;
        if (power > 100.f)
        {
            power = 100.f;
        }
        if (power < 20.f)
        {

            power = 20.f;
        }
        fanPower = int(power);
#ifdef LOG
        Serial.print("power: ");
        Serial.println(fanPower);
#endif
        fanTimer = millis() + CHANGE_FAN;
    }
}
#endif

void setup()
{
#ifdef LOG
    Serial.begin(115200);
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
}
