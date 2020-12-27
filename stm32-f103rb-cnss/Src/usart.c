/*
 * usart.c
 *
 *  Created on: December 22 2020
 *      Author: Mayan and Naomi
 *
 *  23.12.2020: We dubuged usart_init(), and write() it seems that they work as they are supposed to
 *  			But we haven't manged to view the transmited letter on screen.
 *  			Need to dig deeper - and solve the problem.
 *
 *  27.12.2020: TXE not and USART2->DR not changing (in write()) this might be normal
 */

#include "usart.h"
#include "stm32f103xb.h"
#include "hc-sr501pir_sensor.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/


void init_usart(){

	/*Enable Alternate Funcion for PINs)*/
	//RCC->APB2ENR |= 0x00000001; // for interrupt (see RM 8.3.7)

	/*Enable USART2*/
	RCC->APB1ENR |= 0x00020000; // (see RM 8.3.8)

	/*Following directions RM pg.792 */
	USART2->CR1 |= 0x00002000; //Enable the USART by writing the UE bit in USART_CR1 register to 1 (see RM 27.6.4)
	//Program the M bit in USART_CR1 to define the word length to 8 (by default) (see RM 27.6.4)
	//Program the number of stop bits in USART_CR2 to 1 (by defualt) (see RM 27.6.5)

	/*Set Baude rate*/
	USART2->BRR = 0xEA6; // 9600 bps (see https://www.programmersought.com/article/11991629979/ and RM 27.6.3)

	/*Enable Uart Transmit*/
	USART2->CR1 |= 0x00000008; // Set the TE bit in USART_CR1 to send an idle frame as first transmission. see RM 27.6.4)

	/*Enable Uart Recirve*/
	//Maybe afterwords - as of now don't need
}

void write(){

	//while(((USART2->SR) & 0x00000040) !=  0x00000040); //wait until transmition is complete TC=1 (see RM 27.6.1)
	//wait while data is not transferd
	//wait while txe==0
	while(((USART2->SR) & 0x00000080) == 0x00000000);// TXE != 1 // == 0x00000000); // != 0x00000080 check TXE (see RM 27.6.1)
	USART2->DR = ('A' & 0xFF); //send data (see RM 27.6.2)
}
