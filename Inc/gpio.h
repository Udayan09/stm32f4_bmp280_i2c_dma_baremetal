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


void gpio_init(GPIO_TypeDef* gpio);
void gpio_pin_init(GPIO_TypeDef* gpio, uint8_t pinNumber, gpioModes_t pinMode, uint8_t altFunc);
void gpio_setPin(GPIO_TypeDef* gpio, uint8_t pinNumber, uint8_t pinValue);
void gpio_togglePin(GPIO_TypeDef* gpio, uint8_t pinNumber);
uint8_t gpio_getPin(GPIO_TypeDef* gpio, uint8_t pinNumber);


#endif // GPIO_H
