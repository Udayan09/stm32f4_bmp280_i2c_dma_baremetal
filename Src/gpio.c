/*
 * gpio.cpp
 *
 *  Created on: Oct 30, 2025
 *      Author: RUF3KOR
 */

#include "gpio.h"

void gpio_init(GPIO_TypeDef* gpio){
    if (gpio == GPIOA)
        RCC->AHB1ENR |= (1U << 0);
    else if (gpio == GPIOB)
        RCC->AHB1ENR |= (1U << 1);
    else if (gpio == GPIOC)
        RCC->AHB1ENR |= (1U << 2);
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
        gpio->ODR |= (1U << pinNumber);
    else
        gpio->ODR &= ~(1U << pinNumber);
}

void gpio_set_output_mode(GPIO_TypeDef* gpio, uint8_t pinNumber, gpioOutputMode_t outputMode){
    gpio->OTYPER &= ~(1U << pinNumber);      //Clears bit
    gpio->OTYPER |=  (outputMode << pinNumber);
}

void gpio_set_pullup_pulldown(GPIO_TypeDef* gpio, uint8_t pinNumber, gpio_pupd_t pupd){
    gpio->PUPDR &= ~(3U << (pinNumber * 2));
    gpio->PUPDR |=  (pupd << (pinNumber * 2));
}

void gpio_togglePin(GPIO_TypeDef* gpio, uint8_t pinNumber){
    gpio->ODR ^= (1U << pinNumber);
}

// Read input value (returns 0 or 1)
uint8_t gpio_getPin(GPIO_TypeDef* gpio, uint8_t pinNumber) {
    return ((gpio->IDR >> pinNumber) & 1U);
}




