#ifndef UART_H
#define UART_H

#include "gpio.h"

#define SYS_FREQ        16000000
#define UART_BAUD_RATE  115200


void UART1_init();
void UART1_write(uint8_t data);
uint8_t UART1_read();
void dma2_stream7_init(uint32_t src, uint32_t dest, uint32_t len);
    
#endif