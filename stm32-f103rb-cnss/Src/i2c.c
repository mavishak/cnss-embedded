/*
 * i2c.c
 *
 *  Created on: 28 Mar 2021
 *  Author: Mayan and naomi
 *
 *  CCR explaned:
 *  http://fastbitlab.com/stm32-i2c-lecture-12-i2c-serial-clock-settings-with-explanations/
 *
 *
 *  OV7670 git:
 *  https://github.com/Ursadon/ov7670-stm32/blob/master/drivers/ov7670/ov7670.c
 *  https://github.com/erikandre/stm32-ov7670/blob/master/src/OV7670_control.c
 *
 */

#include "stm32f103xb.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>

static uint32_t temp; //will help us clear ADDR bit
static uint32_t i2c_buff; //the address of the buffer= where we want to store the data we receive(short explanation on the reference manual p. 277)

/*starting our communication in I2C:
*1.	send a start condition
*2.	Wait until SB in I2C Status register 1 (I2C_SR1) (p.777)-> this is how we will know that the start condition is actually sent
*3.	send the slave-the address so you can find the actual slave you're talking to
*4.	wait until the slave replies(until the bit ADDR in I2C Status register 1 (I2C_SR1) (p.777) is set- cause it means that Received address matched)
*5.	once the drafts finds a match then you can start go ahead and you know proceed with your rx or TX whatever it is you're gonna do
*/

void init_i2c1(void){


	/*Enable I2C1 RCC clock*/
	RCC->APB1ENR |= 0x00200000;

	/*Enable use of SCL (PB6) and SDA (PB7)*/
	RCC->APB2ENR |= 0x00000008;

	/*Enable AFIO*/
	RCC->APB2ENR |= 0x00000001; // ... test with and without

	/*Set SCL (PB6) and SDA (PB7) [reference manual p. 168 Table  27]*/
	GPIOB->CRL &= 0x00FFFFFF; // reset relevebt bits (see: reference manual 9.2.1)
	GPIOB->CRL |= 0xEE000000; // set as Alternate function output Open-drain, max speed 2 MHz.



	/*Set I2C Configurations*/

	/*Enable ACK*/
	I2C1->CR1 |= 0x0400;


	//The following is the required sequence in master mode. [reference manual p. 758]

	/*Set Peripheral clock frequency*/
	I2C1->CR2 &= 0xFFC0; // reset relavent bits FREQ[5:0]
	I2C1->CR2 |= 0x0008; // set frequency to 8MHz same frequency as APB1

	/*Set CCR register: http://fastbitlab.com/stm32-i2c-lecture-12-i2c-serial-clock-settings-with-explanations/
	 *See: [reference manual 26.6.8], [datasheet p. 59 Table 40]*/

	/*Set CCR to generate 100kHz frequency in Sm mode (referance manual p. 782 "For intance"*/
	I2C1->CCR = 0x0028;


	/* Configure the rise time register: TRISE*/

	/*Set TRISE to maximum allowed SCL rise time in Sm mode (referance manual p. 783 "For intance".*/
	I2C1->TRISE = 0x0009;


	/*Enable I2C peripheral (PE = 1)*/
	I2C1->CR1 |= 0x0001; //The CCR register must be configured only when the I2C is disabled (PE = 0) [reference manual p. 781 26.6.8].






	//_________________INITIAL CODE_________________________//
	/*Enable io port B and AFIO clk to allow comunication */
	//RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN; //(see: Ref Manual p. 114) //RCC- enable AFIO??

	/*Enable I2C_2 clk*/
	//RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; //(see: Ref Manual p. 115 + drawing on p.276)


	//!--NVIC_EnableIRQ(I2C2_EV_IRQn);//enable interrupt line - not in use because we use DMA in Rx

	/*FIX - NEED TO CACLCULATE!*/
	//I2C2->CR2 |= 36; //frequency value of APB1 buss=72/2=36 (Ref Manual p.774)
	//I2C2->CCR |= 180; //speed to 100kHz (standard mode max)
	//I2C2->TRISE |= 37; // period time 1000 ns/ (CR2 period =1/36MHz) = TRISE +1
	/*FIX - NEED TO CACLCULATE!*/

	//I2C2->CR1 |= I2C_CR1_ACK; //enable acknowledgement (Ref Manual p. 772)

	//stretch mode enabled by default (Ref Manual p. 772)

	//7 bit addressing mode enabled by default (Ref Manual p. 773)

	/*
	* config gpio for i2c use enable internal pull up
	* configure to alternate function open drain mode (Ref Manual p. 168, Table 27 I2C ):
	* PB10: I2C2_SCL ,PB11: I2C2_SDA (STM32 datasheet p. 30)
	*/
	//GPIOB->CRH &= 0xFFFF00FF; // clean content in PB10, PB11 (see: Ref Manual p. 172, 9.2.2)
	//GPIOB->CRH |= 0x0000EE00; // set PB10 and PB11 to: Alternate function output Open-drain, max speed 2 MHz.
	//It doesn't seem we need to set GPIOB->ODR, but we saw someone do it so... ??? :Q (see: Ref Manual p. 161 Table 20)

	/*Enable DMA Interupts*/
	//I2C2->CR2 |= I2C_CR2_ITBUFEN; //enable corresponding interrupts (buffer interrupts)
	//I2C2->CR2 |= I2C_CR2_ITEVTEN; //event interrupts

	//I2C2->CR1 |= I2C_CR1_PE; //enable peripheral needs to be done last (Ref Manual p. 772, 26.6.1)

	//_________________INITIAL CODE_________________________//
}


/*Device address 0x42 for write, 0x43 for read [OV7670 datasheet p. 12]*/
void I2C_start(uint8_t address){//, uint8_t direction){

	/*wait while communication ongoing [reference manual 26.6.7]*/
	while((I2C1->SR2 & 0x0002) == 0x0002); // BUSY bit

	/*Generate start bit*/
	I2C1->CR1 |= 0x0100; // (reference manual 26.6.1)

	/*Wait until start condition is generated successfully [refernce manual 26.6.6]*/
	while((I2C1->SR1 & 0x0001) == 0x0001); // SB bit
	//should this be != ??

	//SB=1: Cleared by software by reading the SR1 register followed by writing the DR register

	/*Write address*/
	I2C1->DR = (uint8_t)(device_address & 0xFF);

	/*Wait until address is recieved and matched [refernce manual 26.6.6]*/
	while(!(I2C1->SR1 & 0x0002) == 0x0002); // ADDR bit

	//ADDER=1: Cleared by software reading SR1 register followed reading SR2

	temp = I2C1->SR2;
}


/*Note: data can be mem_address [first column in OV7670 datasheet Table 5]*/
void I2C_write(uint8_t data){

	/*Send data*/
	I2C2->DR = (uint8_t)(data & 0xFF);

	/*Wait data to be received*/
	while((I2C2->SR1 & 0x0080) == 0x0080); //TXE bit

	//Cleared by software writing to the DR register (referance manual 26.26.6)
}


void I2C_stop(){

	/*Generate stop condition*/
	I2C2->CR1 |= 0x0200;//(reference manual 26.6.1)
}

uint8_t I2C_read_nack(){
	//?????
}







void test_communication(void){


}








//using regular flags (not DMA)
void i2c_write(uint8_t device_address,uint8_t mem_address, uint8_t data, uint8_t length){


	/*---------Maybe according to Reference manual p. 760, Figure 273. Transfer sequence diagram for master transmitter(7-bit master transmitter-maybe we should 10-bit???)---------*/

	/*send a start condition: S (see: Ref Manual 760)*/
	I2C2->CR1 |= I2C_CR1_START; //generate our start condition(Reference manual p. 772)

	/*Wait until we know that the start condition arrived: EV5 (see: Ref Manual 760)*/
	while((I2C2->SR1 & 0x0001) == 0x0001);// wait until I2C_CR2 SB=1, start condition is sent successfully (Ref Manual p.777)


	/*send the slave's address by writing it in the data register: Address (see: Ref Manual 760)*/
	I2C2->DR = (uint8_t)(device_address & 0xFF);// (Reference manual p. 777) //maybe we need to write here:0x42 because of:(OV7670/OV7171 Advanced Information Preliminary Datasheet p. 11)

	/*Wait until we know that there was an address match: EV6 (see: Ref Manual 760)*/
	while(!(I2C2->SR1 & 0x0002) == 0x0002);//wait until I2C_CR2 ADDR=1, there was an address match( Ref Manual p. 777)

	//Reading by software SR1 register followed by reading SR2  clears ADDR bit(p. 780))
	temp = I2C2->SR2;//clears ADDR


	/*Enter the internal memory address of the slave to write to: EV8_1 - Data1 ??? (see: Ref Manual 760)*/
	I2C2->DR = (uint8_t)(mem_address & 0xFF);//address to write to//maybe the Address(Hex) in the table 5 in OV7670/OV7171 Advanced Information Preliminary Datasheet p. 11

	/*Wait for the address transfer to complete - cleared by writing DR register*/
	while((I2C2->SR1 & 0x0080) == 0x0080); // wait until TXE = 1


	//now we can put data in the register
	while(length > 0)
	{
		/*write data to the address*/
		I2C2->DR = (uint8_t)(data & 0xFF);//data to write in that address

		/*wait for the data transfer to complete*/
		while((I2C2->SR1 & 0x0080) == 0x0080); // wait until TXE = 1

		length--;
	}

	/*send stop condition: EV8_2 (see: Ref Manual 760)*/
	I2C2->CR1 |=I2C_CR1_STOP;//(Reference manual p. 772)
}



//Using DMA
void i2c_read(uint8_t device_address, uint8_t length){
	//uint32_t temp = 0; //will help to clear flags

	/*---------enable the DMA---------*/
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;// DMA1 clock enable (Reference manual p. 111)
	I2C2->CR2 |= I2C_CR2_DMAEN;// DMA requests enable (Reference manual p. 774)

	//????????????Maybe the next line redundant because we wrote it in init_i2c()???????????:
	I2C2->CR1 |= I2C_CR1_ACK; //enable acknowledges

	/*---------configure the DMA Channel procedure(Reference manual p.278)---------*/
	/*Why channel 5? because of Reference manual p.281+282*/
	//The first transfer address is the one programmed in the DMA_CPARx/DMA_CMARx registers(Reference manual p. 277)

	//1.Setting the peripheral register address in the DMA_CPARx register.
	DMA1_Channel5->CPAR = (uint32_t)&I2C2->DR;//the address of the data register of the i2c(the address of where we reading from)(short explanation on the reference manual p. 277)

	//2.Setting the memory address in the DMA_CMARx register.
	DMA1_Channel5->CMAR = (uint32_t)i2c_buff;//the address of the buffer= where we want to store the data we receive(short explanation on the reference manual p. 277)

	//3.Configure the total number of data to be transferred in the DMA_CNDTRx register (After each peripheral event, this value will be decremented.)
	DMA1_Channel5->CNDTR = length; //the number of bytes we receiving(the number of transfers from the data register(I2C2->DR) to the buffer(i2c_buff)(short explanation on the reference manual p. 277)

	//4.Configure the channel priority using the PL[1:0] bits in the DMA_CCRx register (Reference manual p.286)

	//5.Configure data transfer direction, circular mode, peripheral & memory incremented mode, peripheral & memory data size, and interrupt after half and/or full transfer in the DMA_CCRx register
	DMA1_Channel5->CCR |= DMA_CCR_TCIE; //Transfer complete interrupt enable for the DMA (Reference manual p.286)
	DMA1_Channel5->CCR |= DMA_CCR_MINC; //Memory increment mode enabled (Reference manual p.286)

	//6.Activate the channel by setting the ENABLE bit in the DMA_CCRx register.
	DMA1_Channel5->CCR |= DMA_CCR_EN; // Channel enable (Reference manual p.286)

	/*---------generate start condition---------*/
	I2C2->CR1 |= I2C_CR1_START; //(Reference manual p.772)
	while(!(I2C2->SR1 & I2C_SR1_SB)) //wait until the SB flag is set
	{
	}

	/*---------send address---------*/
	I2C2->DR = device_address +1;
	while(!(I2C2->SR1 & I2C_SR1_ADDR))//wait for the device match
	{
	}

	temp = I2C2->SR2;//causing to clear ADDR flag

	/*---------wait for the transfer to complete from the DMA---------*/
	while((DMA1->ISR & DMA_ISR_TCIF1) == 0);

	/*---------sending stop condition---------*/
	I2C2->CR1 |= I2C_CR1_STOP;
}



