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
 *  27.12.2020: USART2->DR is not changing and so is TXE. This might be normal.
 *  			There might be a problem with ST-LINK/v.2
 *  			Note: Changed to branch main_v.2 because of problems in main branch
 *
 *  27.12.2020: We maneged to connect TeraTerm and USART2 by enabling PA2.
 *  			Problem: TeraTerm is not displaying Character correctly.
 *
 *  28.12.2020: Interrupt TXE enabled. Need
 *  			TBD: to enable TC=1 interrupt.
 *  			Problem: TeraTerm still dislaying garbage.
 *
 *  30.12.2020: Good News: Got TeraTerm To work!
 *  			Bad News: I have no Idea why it's working... :/
 *  			Basically the problem is in the setting of the BAUD RATE.
 *  			We need to get a better understanding of it's calculation and of the clock tree.
 *  			How is it working:
 *  			TeraTerm speed is set to 2400 and USART_BRR is set to 0xD05.
 *  			(I don't know if it matters but - GPIOA is configured to a speed of 2 MHz.)
 *  			Note: Interrupt is now disabled - Instructions from above.
 *
 *
 */

#include "usart.h"
#include "stm32f103xb.h"
#include "hc-sr501pir_sensor.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/


void init_usart(){

	/*This program works when TeraTerm speed is set to 2400 and USART_BRR is set to 0xD05.
	 * I HAVE NO IDEA WHY???*/

	/*Testing...*/
	//RCC->CFGR |= 0x00000400; //(AHB)HCLK/2 NOT HELPFULL...
	//THIS DIDN"T WORK!!! LEAVING FOR BETTER UNDERSTANDING.
	/*...Testing*/


	/*ADDED...*/
	/*Enable RCC for Alternate Funcion for PINs*/
	//RCC->APB2ENR |= 0x00000001; //  (see RM 8.3.7)

	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; // (see RM 8.3.7)

	/*Configure USART2 Tx (PA2) as Output */
	GPIOA->CRL &= 0xFFFFF0FF; //Leave all bits as they are exept for bit 2 (see RM 9.2.1)
	GPIOA->CRL |= 0x00000A00; //Configure as Alternate function output Push-pull | Speed 2 MHz (see RM 9.2.1)
	//supposedly this is supposed to be better:
	//GPIOA->CRL |= 0x00000B00; //Configure as Alternate function output Push-pull | Speed 50 MHz (see RM 9.2.1)
	/*...ADDED*/


	/*Enable RCC for USART2*/
	RCC->APB1ENR |= 0x00020000; // (see RM 8.3.8)


	/*Following directions RM pg.792 */
	USART2->CR1 |= 0x00002000; //Enable the USART by writing the UE bit in USART_CR1 register to 1 (see RM 27.6.4)
	//USART2->CR1 &= ~(0x00001000); //Program the M bit in USART_CR1 to define the word length to 8 (by default) (see RM 27.6.4)
	//USART2->CR1 &= ~(0x00000400); //Parity Controle Disable (by default) (see RM 27.6.4)
	//USART2->CR2 &= ~(0x00003000); //Program the number of stop bits in USART_CR2 to 1 (by defualt) (see RM 27.6.5)

	/*Set Baude rate*/
	//USART2->BRR = 0xEA6; // 9600 bps (see https://www.programmersought.com/article/11991629979/ and RM 27.6.3)
	USART2->BRR = 0xD05;

	/*Enable Uart Transmit*/
	USART2->CR1 |= 0x00000008; // Set the TE bit in USART_CR1 to send an idle frame as first transmission. see RM 27.6.4)

	/*Enable Uart Transmit Interrupt*/
	/*__disable_irq();
	USART2->CR1 |= 0x00000080; // Set TXEIE (see RM 27.6.4)
	// Should we set TCIE (Transmission complete interrupt eneble) interrupt??
	//	- according to RM when Tx is done we need to wait until TC=1.
	NVIC_SetPriority(USART2_IRQn,0); //set all interrupt priotity to zero so that no preemption uccors.
	NVIC_EnableIRQ(USART2_IRQn); //enable handler
	__enable_irq();*/

	/*Enable Uart Recirve*/
	//Maybe afterwords - as of now don't need
}

/*USART2 write function with no interrupt*/
void write(){

	//while(((USART2->SR) & 0x00000040) !=  0x00000040); //wait until transmition is complete TC=1 (see RM 27.6.1)
	//wait while data is not transferd
	//wait while txe==0
	while(((USART2->SR) & 0x00000080) == 0x00000000);// TXE != 1 // == 0x00000000); // != 0x00000080 check TXE (see RM 27.6.1)
	USART2->DR = (uint8_t)('U' & 0xFF); //send data (see RM 27.6.2)

}



/*USART2 intterupt handler*/
/*void USART2_IRQHandler(void){

	USART2->DR = (0x55 & 0xFF);//(uint8_t)('U' & 0xFF); //send data (see RM 27.6.2)
	//USART2->CR1 &= ~(0x00000080); // reset TXEIE no interrupt (see RM 27.6.4)

}*/



