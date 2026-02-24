#include "i2c.h"
#include "gpio.h"


#define I2C1EN (1U << 21)


#define PB6_I2C_SCL_ALT_FUNC	4
#define PB7_I2C_SDA_ALT_FUNC	4


void I2C1_init(){       //PB7->SDA,     PB6->SCL

    //Enable clock access
    RCC->APB1ENR |= I2C1EN;

    //Enable GPIO port
    gpio_init(GPIOB);

    //Set the gpio modes
    gpio_pin_init(GPIOB, 6, GPIO_ALTERNATE, PB6_I2C_SCL_ALT_FUNC);
	gpio_pin_init(GPIOB, 7, GPIO_ALTERNATE, PB7_I2C_SDA_ALT_FUNC);

    //Set output mode to open drain
    gpio_set_output_mode(GPIOB, 6, GPIO_OUTPUT_MODE_OPEN_DRAIN);
    gpio_set_output_mode(GPIOB, 7, GPIO_OUTPUT_MODE_OPEN_DRAIN);

    //Set Gpio pins to pullup
    gpio_set_pullup_pulldown(GPIOB, 6, GPIO_PULLUP);
    gpio_set_pullup_pulldown(GPIOB, 7, GPIO_PULLUP);
 
    //Enter i2c reset mode
    I2C1->CR1 |= (1U << 15);
}