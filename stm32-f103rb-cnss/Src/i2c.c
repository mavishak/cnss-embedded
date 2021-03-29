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
	//RCC->APB2ENR |= RCC_APB2ENR_IOBEN //iO port B clock enable(P. 114)

	//RCC->APB1ENR &= RCC_APB1ENR_I2C1EN //enable I2C_1(P. 115)//?
	//RCC->APB1ENR &= RCC_APB1ENR_I2C2EN //enable I2C_2(P. 115)//?

	//??RCC- enable AFIO??

	//I2C2->CR2 |= 36; //freq value of APB1 buss=72/2=36 (p.774)
	//I2C2->CCR |= 180; //speed to 100kHz (standart mode max)
	//I2C2->TRISE |= 37; //1000 ns/ (CR2 period =1/36MHz) = TRISE +1

	//I2C2->CR1 |= I2C_cr1_ACK; //enable acks

	//stretch mode enabled by default

	//7 bit addressing mode enabled by default

	//config gpio for i2c use enable internal pull up
	//GPIOB->CRH |= GPIO_CRH_MODE10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE11; //alt function output open drain(p. 168, Table 27. I2C +p.172)
	//GPIOB->ODR |= (1<<10) | (1<<11);


	//I2C2->CR1 |= I2C_CR1_PE; //enable peripheral needs to be done last
}
void i2c_write_single(uint8_t device_address,uint8_t mem_address, uint8_t data){

	//uint32_t temp; //will help us clear ADDR bit

	/*---------send a start condition---------*/
	//I2C->CR1 |= I2C_CR1_START //generate our start condition(p. 772)

	/*---------Wait until we know that the start condition arrived---------*/
	//while(!(I2C2->SR1 & I2C_SR1_SB))//until I2C_CR2 SB=1 //tells us that the start condition successfully sent(p.777)
	//{
	//}

	/*---------send the slave's address by writing it in the data register---------*/
	//I2C->DR = device_address;//write it in I2C_OAR1 (p. 776)

	/*---------Wait until we know that there was an address match---------*/
	//while(!(I2C2->SR1 & I2C_SR1_ADDR))//until I2C_CR2 ADDR=1//tells us that there was an address match(p. 777)
	//{
	//}

	//We will read SR2 bit( ADDR bit is cleared by software reading SR1 register followed reading SR2(p. 780))
	//temp = I2C2->SR2;

	/*---------enter the memory/internal address to write to---------*/
	//I2C2->DR = mem_address;//address to write to

	/*---------wait for the address transfer to complete---------*/
	//while(!(I2C2->SR1 & I2C_SR1_TXE))
	//{
	//}

	/*---------write data to the address---------*/
	//I2C2->DR = data;//data to write in that address

	/*---------wait for the data transfer to complete---------*/
	//while(!(I2C2->SR1 & I2C_SR1_TXE))
	//{
	//}

	/*---------send another start condition to get the data---------*/
	//I2C2->CR1 |=I2C_CR1_STOP;
}


