#include "i2c.h"
#include "gpio.h"


#define I2C1EN (1U << 21)
#define PB6_I2C_SCL_ALT_FUNC	4
#define PB7_I2C_SDA_ALT_FUNC	4

#define I2C_100KHZ          80
#define I2C_MX_TRISE_STD    17      //(1000ns/Tpclk) + 1

/*i2c control symbols*/
#define CR1_START       (1U << 8)       //Generate start bit
#define CR1_STOP        (1U << 8)       //Generate stop bit
#define CR1_ACK        (1U << 8)       //Generate ACK


/*i2c status symbols*/
#define SR1_START       (1U << 0)       //Start byte sent
#define SR1_ADDR        (1U << 1)       //Address sent
#define SR1_BTF         (1U << 2)       //Byte transfer finished
#define SR1_RxNE        (1U << 6)       //Receiver Data register not empty
#define SR1_TXE         (1U << 7)       //Transfer Data register empty
#define SR2_BUSY        (1U << 1)

/*Function Prototype*/
static void I2C1_start(void);
static void I2C1_address(uint8_t slaveAddr, uint8_t dir);
static void I2C1_stop(void);


/*I2C Functions*/
void I2C1_init(){       //PB7->SDA,     PB6->SCL

    /*GPIO CONFIG*/
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
 
    /*I2C CONFIG*/
    RCC->APB1ENR |= I2C1EN;         //Enable i2c clock access
   
    I2C1->CR1 |= (1U << 15);        //Enter i2c reset mode

    I2C1->CR1 &= ~(1U << 15);       //Come out of i2c reset mode

    I2C1->CR2 |= (0x10 << 0);       //Setting i2c input clk freq

    I2C1->CCR = I2C_100KHZ;         //Setting multiplier to get i2c standard mode

    I2C1->TRISE = I2C_MX_TRISE_STD; //Set Max trise time

    I2C1->CR1 |= (1U << 0);         //Enable i2c periperal
}

void I2C1_byteWrite(uint8_t slaveAddr, uint8_t regAddr, uint8_t data){
    while(I2C1->SR2 & SR2_BUSY);       //Wait while i2c is busy

    I2C1_start();       //Start i2c

    I2C1_address(slaveAddr, 0);

    while (!(I2C1->SR1 & SR1_TXE));     //Wait for transmitter empty
    I2C1->DR = regAddr;

    while (!(I2C1->SR1 & SR1_TXE));
    I2C1->DR = data;     

    while (!(I2C1->SR1 & SR1_BTF));     //Wait for byte transfer finished

    I2C1_stop();
}

uint8_t I2C1_byteRead(uint8_t slaveAddr, uint8_t regAddr){
    uint8_t rxData;

    I2C1_start();
    I2C1_address(slaveAddr, 0);

    while (!(I2C1->SR1 & SR1_TXE));
    I2C1->DR = regAddr;

    while (!(I2C1->SR1 & SR1_BTF)); 

    /*Repeat Start*/
    I2C1_start();
    I2C1_address(slaveAddr, 0);

    I2C1->CR1 &= ~(CR1_ACK);        //Prepare NACK

    /*Clear Addr*/
    volatile uint32_t temp;
    temp = I2C1->SR1;
    temp = I2C1->SR2;
    (void)temp;

    I2C1_stop();

    while (!(I2C1->SR1 & SR1_RxNE));

    rxData = I2C1->DR;      //Read received data

    I2C1->CR1 |= (CR1_ACK);

    return rxData;
}


//i2c start sequence
static void I2C1_start(void){
    
    I2C1->CR1 |= CR1_START;                 //I2C start

    while (!(I2C1->SR1 & SR1_START));       //Wait for start to be set
}

//i2c address sequence
static void I2C1_address(uint8_t slaveAddr, uint8_t dir){
    
    I2C1->DR = slaveAddr << 1 | dir;      //Set Slave Address + read/write  0->write, 1->Read

    while (!(I2C1->SR1 & SR1_ADDR));    //Wait for address set flag

    /*Clear Addr by reading SR1 and SR2*/
    volatile uint32_t temp;
    temp = I2C1->SR1;
    temp = I2C1->SR2;
    (void)temp;
}

//i2c stop sequence
static void I2C1_stop(void){
    I2C1->CR1 |= CR1_STOP;
}