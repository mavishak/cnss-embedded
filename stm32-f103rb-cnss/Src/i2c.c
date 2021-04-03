/*
 * i2c.c
 *
 *  Created on: 28 Mar 2021
 *      Author: Owner
 */

#include "stm32f103xb.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>

/*starting our communication in I2C:
*1.	send a start condition
*2.	Wait until SB in I2C Status register 1 (I2C_SR1) (p.777)-> this is how we will know that the start condition is actually sent
*3.	send the slave-the address so you can find the actual slave you're talking to
*4.	wait until the slave replies(until the bit ADDR in I2C Status register 1 (I2C_SR1) (p.777) is set- cause it means that Received address matched)
*5.	once the drafts finds a match then you can start go ahead and you know proceed with your rx or TX whatever it is you're gonna do
*/

/*---------Init I2c---------*/
void init_i2c(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //iO port B clock enable(P. 114)

	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; //enable I2C_2(P. 115 + drawing on p.276)

	//RCC- enable AFIO??

	//NVIC_EnableIRQ(I2C2_EV_IRQn);//enable interrupt line//not in use because we use DMA in Rx

	I2C2->CR2 |= 36;//NEED TO CACLCULATE! //freq value of APB1 buss=72/2=36 (p.774)
	I2C2->CCR |= 180;//NEED TO CACLCULATE! //speed to 100kHz (standard mode max)
	I2C2->TRISE |= 37;//NEED TO CACLCULATE! //1000 ns/ (CR2 period =1/36MHz) = TRISE +1

	I2C2->CR1 |= I2C_CR1_ACK; //enable acknowledge

	//stretch mode enabled by default

	//7 bit addressing mode enabled by default (reference manual p. 773)

	/*config gpio for i2c use enable internal pull up
	*configure to alternate function open drain mode (p. 168, Table 27 I2C ):
	*PB10: I2C2_SCL ,PB11: I2C2_SDA (STM32 datasheet p.30)*/
	GPIOB->CRH |=  GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE11; //alt function output open drain(p.172)
	GPIOB->ODR |= (1<<10) | (1<<11);//Port output data register

	//I2C2->CR2 |= I2C_CR2_ITBUFEN; //enable corresponding interrupts (buffer interrupts)
	//I2C2->CR2 |= I2C_CR2_ITEVTEN; //event interrupts

	I2C2->CR1 |= I2C_CR1_PE; //enable peripheral needs to be done last
}
//using regular flags (not DMA)
void i2c_write(uint8_t device_address,uint8_t mem_address, uint8_t data, uint8_t length){

	uint32_t temp; //will help us clear ADDR bit

	/*---------Maybe according to Reference manual p. 760, Figure 273. Transfer sequence diagram for master transmitter(7-bit master transmitter-maybe we should 10-bit???)---------*/

	/*---------send a start condition---------*/
	I2C2->CR1 |= I2C_CR1_START; //generate our start condition(Reference manual p. 772)

	/*---------Wait until we know that the start condition arrived---------*/
	while(!(I2C2->SR1 & I2C_SR1_SB))//until I2C_CR2 SB=1 //tells us that the start condition successfully sent(p.777)
	{
	}

	/*---------send the slave's address by writing it in the data register---------*/
	I2C2->DR = device_address;// (Reference manual p. 777)//maybe we need to write here:0x42 because of:(OV7670/OV7171 Advanced Information Preliminary Datasheet p. 11)

	/*---------Wait until we know that there was an address match---------*/
	while(!(I2C2->SR1 & I2C_SR1_ADDR))//until I2C_CR2 ADDR=1//tells us that there was an address match(p. 777)
	{
	}

	//Reading by software SR1 register followed by reading SR2  clears ADDR bit(p. 780))
	temp = I2C2->SR2;//clears ADDR

	/*---------enter the internal memory address of the slave to write to---------*/
	I2C2->DR = mem_address;//address to write to//maybe the Address(Hex) in the table 5 in OV7670/OV7171 Advanced Information Preliminary Datasheet p. 11

	/*---------wait for the address transfer to complete---------*/
	while(!(I2C2->SR1 & I2C_SR1_TXE))
	{
	}

	//now we can put data in the register

	for( ; length>0 ; length--)
	{
		/*---------write data to the address---------*/
		I2C2->DR = data;//data to write in that address

		/*---------wait for the data transfer to complete---------*/
		while(!(I2C2->SR1 & I2C_SR1_TXE))
		{
		}
	}
	/*---------send stop condition---------*/
	I2C2->CR1 |=I2C_CR1_STOP;//(Reference manual p. 772)
}

//Using DMA
void i2c_read(uint8_t device_address, uint8_t length){
	uint32_t temp = 0; //will help to clear flags

	/*---------enable the DMA---------*/
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;// DMA1 clock enable (Reference manual p. 111)
	I2C2->CR2 |= I2C_CR2_DMAEN;// DMA requests enable (Reference manual p. 774)

	//????????????Maybe the next line redundant because we wrote it in the init_i2c():
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
	DMA1_Channel5->CCR |= DMA_CCR4_TCIE; //Transfer complete interrupt enable for the DMA (Reference manual p.286)
	DMA1_Channel5->CCR |= DMA_CCR5_MINC; //Memory increment mode enabled (Reference manual p.286)

	//6.Activate the channel by setting the ENABLE bit in the DMA_CCRx register.
	DMA1_Channel5->CCR |= DMA_CCR5_EN; // Channel enable (Reference manual p.286)

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
	while((DMA1->ISR & DMA_ISR_TCIFS) == 0);

	/*---------sending stop condition---------*/
	I2C2->CR1 |= I2C_CR1_STOP;
}



