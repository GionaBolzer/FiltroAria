#include "main.h"

Button button(BUTTON_PIN); // costruisco classe su bottone pin
Screen displayClasse;
Logica state;
// STM32Duino harware timer
#ifdef STM32F411xE
HardwareTimer *MyTim;
uint32_t channel;
#endif

void setup()
{

#ifdef LOG
    Serial.begin(115200);
    Serial.println("Welcome home, Sir");
#endif

#ifdef SCREEN

    displayClasse.begin();
    delay(100);
    displayClasse.home(PM_READ_2_5, PM_READ_10, fanPower, state.timeHome);
#endif

#ifdef PMSENSOR
    pmsSerial.begin(9600);
    pms.passiveMode();
#endif

#ifdef FAN
    PwmInit();
#endif
}

void loop()
{

#ifdef PMSENSOR
    wakeUpSensor();
    readSensor();
#endif

#ifdef FAN
    power(PM_READ_2_5);
    setPwmDuty(fanPower); // Change this value 0-100 to adjust duty cycle
#endif

    // check state button return short or long
    Pressed b = button.scan();

#ifdef SCREEN
    refreshHome();
    switch (state.change(b))
    {
    case State::HOME:
        displayClasse.home(PM_READ_2_5, PM_READ_10, fanPower, state.timeHome);
        break;
    case State::MODE:
        displayClasse.paginaMode(state.timeSchermo, state.powerSchermo);
        break;
    case State::MODESELTIMER:
        displayClasse.paginaModeSelTimer(state.timeSchermo, state.powerSchermo);
        break;
    case State::MODESELPOWER:
        displayClasse.paginaModeSelPower(state.timeSchermo, state.powerSchermo);
        break;
    }
#endif
}

// REFRESH  VALUE IN HOME SCREEN EVERY SECOND
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
            fanPower = state.powerHome;
        }
        if (millis() > homeTimer + REFRESH_HOME)
        {
            if (state.timeHome > 0 && int((millis() - state.timerHome) / 60000) >= 1)
            {
                state.timeHome -= int((millis() - state.timerHome) / 60000);
                state.timerHome = millis();
            }

            displayClasse.home(PM_READ_2_5, PM_READ_10, fanPower, state.timeHome);
            homeTimer = millis();
        }
    }
}

#ifdef PMSENSOR

// WAKE UP SENSOR EVERY GET_PM_READ_EVERY SECONDS
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

// READ SENSOR AFTER WAIT_PM SECONDS AFTER IS WAKE UP AND THEN PUT IN SLEEP
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
            PM_READ_2_5 = data.PM_AE_UG_2_5;
            PM_READ_10 = data.PM_AE_UG_10_0;
            // PM_READ_2_5 = 25;
            // PM_READ_10 = 30;
        }
        else
        {
#ifdef LOG
            Serial.println("No data.");
#endif
                // PM_READ_2_5 = 271;
                // PM_READ_10 = 314;
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

// WRITE DUTY CYCLE FOR PWM
void setPwmDuty(byte duty)
{

#ifdef ARDUINO_AVR_NANO
    OCR1A = (word)(duty * TCNT1_TOP) / 100;
#endif

#ifdef STM32F411xE
    MyTim->setCaptureCompare(channel, duty, PERCENT_COMPARE_FORMAT); // 100%
#endif
}

// RETURN POWER FAN IN % FROM SENSORE READ OR FROM MODE SCREEN
void power(uint32_t read)
{
    if (FORCE_POWER == 1)
    {

        fanPower = state.powerHome;
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

// HARDWARE SETUP FOR PWM  25khz
void PwmInit()
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
    OCR1A = (word)(fanPower * TCNT1_TOP) / 100;
#endif

#ifdef STM32F411xE
    // TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PA6), PinMap_PWM);
    // uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA6), PinMap_PWM));
    // MyTim = new HardwareTimer(Instance);
    // MyTim->setPWM(channel, FAN, PWM_FREQ_HZ, fanPower); // 25khz Hertz, 20% dutycycle

    TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(PA6), PinMap_PWM);
    channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(PA6), PinMap_PWM));
    MyTim = new HardwareTimer(Instance);
    MyTim->setMode(channel, TIMER_OUTPUT_COMPARE_PWM1, FAN);
    MyTim->setOverflow(PWM_FREQ_HZ, HERTZ_FORMAT);                 // 10000 microseconds = 10 milliseconds
    MyTim->setCaptureCompare(channel, 20, PERCENT_COMPARE_FORMAT); // 50%
    MyTim->resume();

#endif
}