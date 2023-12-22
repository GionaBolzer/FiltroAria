#include "main.h"

Button button(BUTTON_PIN); // costruisco classe su bottone pin
Screen displayClasse;
Logica state;

void setup()
{

#ifdef LOG
    Serial.begin(115200);
    Serial.println("Welcome home, Sir");
#endif

#ifdef SCREEN

    displayClasse.begin();
    delay(100);
    displayClasse.home(10, 10, fanPower, 0);
    // displayClasse.home(10, 15, 30);
    // displayClasse.testIter(0);
#endif

#ifdef PMSENSOR
    pmsSerial.begin(9600);
    pms.passiveMode();
#endif

#ifdef FAN
#ifdef ARDUINO_AVR_NANO
    PwmArd();
#endif
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
    refreshHome();
    Pressed b = button.scan();
    switch (state.change(b))
    {
    case State::HOME:
        displayClasse.home(10, 10, fanPower, state.timeHome);
        break;
    case State::MIN:
        displayClasse.paginaMin(state.timeSchermo);

        break;
    case State::MAX:
        displayClasse.paginaMax(state.timeSchermo);

        break;
    case State::MINSEL:
        displayClasse.minsel(state.timeSchermo);

        break;
    case State::MAXSEL:
        displayClasse.maxsel(state.timeSchermo);

        break;
    }
}

void refreshHome()
{
    if (state.schermo == State::HOME)
    {
        if (state.timeHome > 0 && FORCE_POWER == 0)
        {
            FORCE_POWER = 1;
        }
        if (state.timeHome <= 0 && FORCE_POWER == 1)
        {
            state.timeHome = 0;
            FORCE_POWER = 0;
            fanPower = 20;
        }
        if (millis() > homeTimer + REFRESH_HOME)
        {
            if (state.timeHome > 0 && int((millis() - state.timerHome) / 60000) >= 1)
            {
                state.timeHome -= int((millis() - state.timerHome) / 60000);
                state.timerHome = millis();
            }

            displayClasse.home(10, 10, fanPower, state.timeHome);
            homeTimer = millis();
        }
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

void setPwmDuty(byte duty)
{
    // TO DO: aggiungere il pwm per la black pill
#ifdef ARDUINO_AVR_NANO
    OCR1A = (word)(duty * TCNT1_TOP) / 100;
#endif
}

void power(uint32_t read)
{
    if (FORCE_POWER == 1)
    {
        if (state.timerMode == TimerMode::MASSIMO)
        {
            fanPower = 100;
        }
        if (state.timerMode == TimerMode::MINIMO)
        {
            fanPower = 25;
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

void PwmArd()
{
    
    #ifdef ARDUINO_AVR_NANO
    pinMode(OC1A_PIN, OUTPUT);

    // Clear Timer1 control and count registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << CS10);
    ICR1 = TCNT1_TOP;
    #endif

// TO DO: aggiungere il pwm setup per la black pill
    #ifdef STM32F411xE
    
    #endif
}