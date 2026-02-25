#include "systick.h"

void systickDelayMs(int n){

    //Load systick count start value
    SysTick->LOAD = SYSTICK_LOAD_VAL;

    //Reset Systick Counter Value
    SysTick->VAL = 0;

    //Enable Systick and Set system clock
    SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

    //Dummy read to clear the COUNTFLAG
    (void)SysTick->CTRL;

    //Wait for n counts
    for (int i = 0; i < n; i++){
        while(((SysTick->CTRL & (1U << 16)) == 0));
    }

    //Stop systick
    SysTick->CTRL = 0;
}

void systick_1hz_int(void){
    SysTick->LOAD = 16000 - 1;

    //Reset Systick Counter Value
    SysTick->VAL = 0;

    //Enable Systick and Set system clock
    SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC | CTRL_INT_ENABLE;
}