#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f411xe.h"

#define SYSTICK_LOAD_VAL    16000
#define CTRL_ENABLE         (1U << 0)
#define CTRL_INT_ENABLE     (1U << 1)
#define CTRL_CLKSRC         (1U << 2)

void systickDelayMs(int n);
void systick_1hz_int(void);

#endif