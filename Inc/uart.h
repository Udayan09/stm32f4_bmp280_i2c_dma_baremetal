#ifndef UART_H
#define UART_H

#include "gpio.h"

#define USART1EN    (1U << 4)

#define PA9_USART_TX_ALT_FUNCT  7
#define PA10_USART_RX_ALT_FUNCT  7

#define SYS_FREQ        16000000
#define UART_BAUD_RATE  115200


void UART1_init();
void UART1_write(uint8_t data);
    
#endif