#ifndef I2C_H
#define I2C_H

#include "stm32f411xe.h"

#define I2C1EN (1U << 21)
#define PB6_I2C_SCL_ALT_FUNC	4
#define PB7_I2C_SDA_ALT_FUNC	4


void I2C1_init();

#endif // I2C Header