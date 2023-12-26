#ifndef PINDEF_H
#define PINDEF_H


#ifdef STM32F411xE
#define BUTTON_PIN  PA5
#define RxPin PC13
#define TxPin PC14
#define FAN PA6
#endif

#ifdef ARDUINO_AVR_NANO
#define RxPin 2 // Rx on Nano
#define TxPin 3 // Tx on Nano
#define BUTTON_PIN 4
#endif

#endif  