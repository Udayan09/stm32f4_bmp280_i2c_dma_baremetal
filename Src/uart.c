#include "uart.h"

#define USART1EN        (1U << 4)
#define DMA2EN		    (1U << 22)
#define DMA_STREAM_EN   (1U << 0)

#define PA9_USART_TX_ALT_FUNCT  7
#define PA10_USART_RX_ALT_FUNCT  7

static uint16_t compute_uart_div(uint32_t periphClk, uint32_t baudRate);

int __io_putchar(int ch){
	UART1_write(ch);
	return ch;
}

void dma2_stream7_init(uint32_t src, uint32_t dest, uint32_t len){

	RCC->AHB1ENR |= DMA2EN;			//DMA2 clock enable

    DMA2_Stream7->CR &= ~DMA_STREAM_EN;      //Disable DMA2 Stream7

    while(DMA2_Stream7->CR & DMA_STREAM_EN);

    /*Clear Interrupts*/
    DMA2->HIFCR |= (1U << 22);
    DMA2->HIFCR |= (0xf << 24);

    DMA2_Stream7->PAR = dest;           //Setting dest as peripheral address (Memory -> UART_Tx)

    DMA2_Stream7->M0AR = src;           //Setting src as memory0 address

    DMA2_Stream7->NDTR = len;           //Set length of transfer

    DMA2_Stream7->CR = (4U << 25);      //Select Channel4 of DMA stream7

    DMA2_Stream7->CR |= (1U << 10);     //Enable memory increment

    DMA2_Stream7->CR |= (1U << 6);      //Data transfer Dir: Memory -> Peripheral

    DMA2_Stream7->CR |= (1U << 4);      //Enable Transfer Complete Register

    DMA2_Stream7->FCR &= ~(1U << 2);    //Disable FIFO, Enables Direct mode

    DMA2_Stream7->CR |= DMA_STREAM_EN;  //Enable DMA2_Stream7
    
    USART1->CR3 |= (1 << 7);			//Enable DMA for Tx
    
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);  //Enable DMA_Stream7 Interrupts
}

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

    USART1->CR1 |= (3 << 2);			//Enable UART TX & RX

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