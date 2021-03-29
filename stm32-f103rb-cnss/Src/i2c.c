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
//RCC->APB2ENR |= RCC_APB2ENR_IOBEN //iO port B clock enable(P. 114)

//RCC->APB1ENR &= RCC_APB1ENR_I2C1EN //enable I2C_1(P. 115)
//RCC->APB1ENR &= RCC_APB1ENR_I2C2EN //enable I2C_2(P. 115)

//I2C2->CR2 |= 36; //freq value of APB1 buss=72/2=36 (p.774)
/*---------send a start condition---------*/
//I2C_CR1 START=1 //generate our start condition(p. 772)

/*---------Wait---------*/
//until I2C_CR2 SB=1 //tells us that the start condition successfully sent(p.777)

/*---------send the slave's address---------*/
//write it in I2C_OAR1 (p. 776)

/*---------Wait---------*/
//until I2C_CR2 ADDR=1//tells us that there was an address match(p. 777)
