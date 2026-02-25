#include "uart.h"

static uint16_t compute_uart_div(uint32_t periphClk, uint32_t baudRate);

/*PA9 -> Tx,    PA10 -> Rx*/
void UART1_init(){

    //GPIO init
    gpio_init(GPIOA);       //Enable portA

    //Set gpio mode -> output
    gpio_pin_init(GPIOA, 9, GPIO_ALTERNATE, PA9_USART_TX_ALT_FUNCT);
    gpio_pin_init(GPIOA, 10, GPIO_ALTERNATE, PA10_USART_RX_ALT_FUNCT);

    //Set gpio high speed
    gpio_set_ospeed(GPIOA, 9, GPIO_OSPEED_HIGH);
    gpio_set_ospeed(GPIOA, 10, GPIO_OSPEED_HIGH);

    //UART Config
    RCC->APB2ENR |= USART1EN;

    //Compute Baud divider value
    USART1->BRR = compute_uart_div(SYS_FREQ, UART_BAUD_RATE);				

    //Set UART mode
    USART1->CR1 &= ~(1 << 12);		//8bit Mode

    //Enable uart Tx Rx
    USART1->CR1 |= (3 << 2);			//Enable UART TX & RX

    //Uart rx interrupt
    USART1->CR1 |= (1 << 5);			//Enable UART Interrupts

    //Enable USART1 Peripheral
    USART1->CR1 |= (1 << 13);		//Enables UART

    //Enable Interrupt handler
    NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 1);
}

void UART1_write(uint8_t data){    
    USART1->DR = data;
	while (!(USART1->SR & (1 << 6)));	//Waits for Tx to be done
}

uint8_t UART1_read(){
    while (!(USART1->SR & (1 << 5)));
    return USART1->DR;
}

static uint16_t compute_uart_div(uint32_t periphClk, uint32_t baudRate){
    return ((periphClk + (baudRate/2U))/baudRate);
}