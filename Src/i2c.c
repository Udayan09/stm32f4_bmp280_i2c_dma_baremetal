#include "i2c.h"

void i2c_init(I2C_TypeDef *i2c){
    if (i2c == I2C1)    RCC->APB1ENR |= (1 << 21);
    else if (i2c == I2C2)    RCC->APB1ENR |= (1 << 22);
    else if (i2c == I2C3)    RCC->APB1ENR |= (1 << 23);
}