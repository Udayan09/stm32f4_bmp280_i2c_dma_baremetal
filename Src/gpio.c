/*
 * gpio.cpp
 *
 *  Created on: Oct 30, 2025
 *      Author: RUF3KOR
 */

#include "gpio.h"

void gpio_init(GPIO_TypeDef* gpio){
    if (gpio == GPIOA)
        RCC->AHB1ENR |= (1 << 0);
    else if (gpio == GPIOB)
        RCC->AHB1ENR |= (1 << 1);
    else if (gpio == GPIOC)
        RCC->AHB1ENR |= (1 << 2);
}

void gpio_pin_init(GPIO_TypeDef* gpio, uint8_t pinNumber, gpioModes_t pinMode, uint8_t altFunc)
{
    
    // Configure pin mode
    gpio->MODER &= ~(3U << (pinNumber * 2));
    gpio->MODER |=  (pinMode << (pinNumber * 2));

    gpio->OSPEEDR |= (3U << (pinNumber * 2));

    if (pinMode = GPIO_ALTERNATE){
        if (pinNumber <= 7){
            gpio->AFR[0] &= ~(0xf << (pinNumber * 4));
            gpio->AFR[0] |= (altFunc << (pinNumber * 4));
        }
        else {
            gpio->AFR[1] &= ~(0xf << ((pinNumber-8) * 4));
            gpio->AFR[1] |= (altFunc << ((pinNumber-8) * 4));
        }
    }
}

// Set output value (0 or 1)
void gpio_setPin(GPIO_TypeDef* gpio, uint8_t pinNumber, uint8_t pinValue) {
    if (pinValue == 1)
        gpio->ODR |= (1 << pinNumber);
    else
        gpio->ODR &= ~(1 << pinNumber);
}

void gpio_togglePin(GPIO_TypeDef* gpio, uint8_t pinNumber){
    gpio->ODR ^= (1 << pinNumber);
}

// Read input value (returns 0 or 1)
uint8_t gpio_getPin(GPIO_TypeDef* gpio, uint8_t pinNumber) {
    return ((gpio->IDR >> pinNumber) & 1U);
}




