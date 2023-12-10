#include "main.h"

Button button(BUTTON_PIN); // costruisco classe su bottone pin
Screen displayClasse;
Logica state;

void setup()
{
    // state = Home();

#ifdef LOG
    Serial.begin(115200);
    Serial.println("Welcome home, Sir");
#endif

#ifdef SCREEN
    // display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    // display.display();
    // delay(2000);
    displayClasse.begin();
    delay(100);
    displayClasse.home(10,10,10,0);
    // displayClasse.home(10, 15, 30);
    // displayClasse.testIter(0);
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
    switch (state.change(b))
    {
    case State::HOME:
        displayClasse.home(10, 10, 10,0);
        break;
    case State::HOMETIMER:
        displayClasse.home(10, 10, 10, state.time);
    break;
    case State::MIN:
        displayClasse.paginaMin(state.time);
        break;
    case State::MAX:
        displayClasse.paginaMax(state.time);
        break;
    case State::MINSEL:
        displayClasse.minsel(state.time);
        break;
    case State::MAXSEL:
        displayClasse.maxsel(state.time);
        break;
    }

    // if (b == Pressed::SHORT)
    // {
    //     displayClasse.testIter(1);
    //     Serial.println("pulsante short");
    //     //  FORCE_POWER = 0;
    // }
    // else if (b == Pressed::LONG)
    // {
    //     displayClasse.testIter(-1);
    //     Serial.println("pulsante long");
    //     //  FORCE_POWER = millis();
    // }
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
