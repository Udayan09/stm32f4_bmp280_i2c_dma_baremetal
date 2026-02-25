#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f411xe.h"

//Definitions
#define PIN_HIGH    1
#define PIN_LOW     0

// GPIO mode enumeration
typedef enum {
    GPIO_INPUT,         
    GPIO_OUTPUT,
    GPIO_ALTERNATE,
    GPIO_ANALOG
} gpioModes_t;

typedef enum {
    GPIO_NO_PULLUP_PULLDOWN,         
    GPIO_PULLUP,
    GPIO_PULLDOWN,
} gpioPupd_t;

typedef enum{
    GPIO_OUTPUT_MODE_PUSH_PULL,
    GPIO_OUTPUT_MODE_OPEN_DRAIN
} gpioOutputMode_t;

typedef enum{
    GPIO_OSPEED_LOW,
    GPIO_OSPEED_MEDIUM,
    GPIO_OSPEED_FAST,
    GPIO_OSPEED_HIGH
} gpioOspeed_t;


void gpio_init(GPIO_TypeDef* gpio);
void gpio_pin_init(GPIO_TypeDef* gpio, uint8_t pinNumber, gpioModes_t pinMode, uint8_t altFunc);
void gpio_set_output_mode(GPIO_TypeDef* gpio, uint8_t pinNumber, gpioOutputMode_t outputMode);
void gpio_set_pullup_pulldown(GPIO_TypeDef* gpio, uint8_t pinNumber, gpioPupd_t pupd);
void gpio_set_ospeed(GPIO_TypeDef* gpio, uint8_t pinNumber, gpioOspeed_t speed);
void gpio_setPin(GPIO_TypeDef* gpio, uint8_t pinNumber, uint8_t pinValue);
void gpio_togglePin(GPIO_TypeDef* gpio, uint8_t pinNumber);
uint8_t gpio_getPin(GPIO_TypeDef* gpio, uint8_t pinNumber);


#endif // GPIO_H
