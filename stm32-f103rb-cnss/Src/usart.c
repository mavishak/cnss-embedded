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
 * 03.01.2021: A message is now sent to screen rather than one letter.
 * 			   The problems listed above are still valid :/
 *
 * 08.01.2021: SOLVED!!! The USART2 clock is PCLK1 wich is by default only 8MHz NOT 36MHz.
 * 			   (see RM p.798 for BRR calculation and RM p.93 Fig.8 for clock tree).
 * 			   So, basically our problem was that we thought that PCLK1 was set to 36MHz by default.
 * 			   For later practice follow the clock tree and default intiolization of clock registers.
 *
 * 22.01.2021: Setting the base lines for USART1 for comunication with ESP8266
 * 			   MADE A FEW SMALL CHANGES TO USART2 init:
 * 			   1. commented out RCC->APB2ENR |= 0x00000001;
 * 			   2. changed static USART_2 usart2; //changed from buff
 * 			   need to check that it is still working...
 *
 */

#include "usart.h"
#include "stm32f103xb.h"
#include "hc-sr501pir_sensor.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/
#include <string.h>
#include <stdlib.h>

static USART_2 usart2; //changed from buff

/*This dunctions Inits all registors that have to do with enabling USART2 (ST-LINK/V.2)
 *inorder to send message to computer.
 *Note: Interrupts are not enabled intentionally.
 *		This program works when TeraTerm speed is set to 9600*/
void init_usart2(){

	/*This program works when TeraTerm speed is set to 9600 and USART_BRR is set to 0x34D.*/

	/*ADDED...*/
	/*Enable RCC for Alternate Funcion for PINs*/
	//RCC->APB2ENR |= 0x00000001; //  (see RM 8.3.7) //I don't think this line is needed...

	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; // (see RM 8.3.7)

	/*Configure USART2 Tx (PA2) as Output */
	GPIOA->CRL &= 0xFFFFF0FF; //Leave all bits as they are except for bit 2 (see RM 9.2.1)
	GPIOA->CRL |= 0x00000A00; //Configure as Alternate function output Push-pull | Speed 2 MHz (see RM 9.2.1)
	//supposedly this is supposed to be better:
	//GPIOA->CRL |= 0x00000B00; //Configure as Alternate function output Push-pull | Speed 50 MHz (see RM 9.2.1)
	/*...ADDED*/


	/*Enable RCC for USART2*/
	RCC->APB1ENR |= 0x00020000; // (see RM 8.3.8) IS THIS REALLY NEEDED??


	/*Following directions RM pg.792 */
	USART2->CR1 |= 0x00002000; //Enable the USART by writing the UE bit in USART_CR1 register to 1 (see RM 27.6.4)
	//USART2->CR1 &= ~(0x00001000); //Program the M bit in USART_CR1 to define the word length to 8 (by default) (see RM 27.6.4)
	//USART2->CR1 &= ~(0x00000400); //Parity Controle Disable (by default) (see RM 27.6.4)
	//USART2->CR2 &= ~(0x00003000); //Program the number of stop bits in USART_CR2 to 1 (by defualt) (see RM 27.6.5)

	/*Set Baude rate*/
	USART2->BRR = 0x34D; //9600 bps (see RM p.798 for BRR calculation and RM p.93 Fig.8 for clock tree)

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


void init_usart1(){

	//Can enable only Tx or Rx Each time (In Arduino we couldn't we can't remeber why)??

	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; // (see RM 8.3.7)

	/*Configure USART1 Tx (PA9) as Output*/
	GPIOA->CRH &= 0xFFFFFF0F; //Leave all bits as they are except for bit 9 (see RM 9.2.2)
	GPIOA->CRH |= 0x000000A0; //Configure as Alternate function output Push-pull | max speed 2 MHz (See RM 9.2.2 and pg.181).
	//Maybe this is supposed to be better:
	//GPIOA->CRH |= 0x000000B0; //Configure as Alternate function output Push-pull | Speed 50 MHz (see RM 9.2.2)

	/*Configure USART1 Rx (PA10) as Input*/
	//Input Pull-Up or Pull-Down? Or can we use Input-Floating??

	/*Enable RCC for USART1*/
	RCC->APB2ENR |= 0x00004000; // (See RM 8.3.7)

	/*Following directions RM pg.792 (Setting Tx procesure)*/
	USART1->CR1 |= 0x00002000; //Enable the USART by writing the UE bit in USART_CR1 register to 1 (see RM 27.6.4)
	//USART1->CR1 &= ~(0x00001000); //Program the M bit in USART_CR1 to define the word length to 8 (by default) (see RM 27.6.4)
	//USART1->CR1 &= ~(0x00000400); //Parity Controle Disable (by default) (see RM 27.6.4)
	//USART1->CR2 &= ~(0x00003000); //Program the number of stop bits in USART_CR2 to 1 (by defualt) (see RM 27.6.5)

	/* SHOULD WE DO THIS?? (What is DMA)
	 * Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
	 * place. Configure the DMA register as explained in multibuffer communication.
	 */

	/*Set Baude Rate for USART1 9600 bps*/
	// WHAT IS THE CLOCK FREQUENCY

	/*Following directions RM pg.795 (Setting Rx procesure) */


	/*Enable Uart Transmit*/

	/*Enable Uart Receive*/

	/*Enable Uart Receive Interrupt*/

}

/*This function sets the Tx buffer up with chosen message.
 * One may choose to use the default MSG defined in usart.h*/
void set_usart2_buffer_Tx(uint8_t *msg){


	memset(usart2.Tx, '\0', BUFF_SIZE*sizeof(uint8_t));
	if(BUFF_SIZE - (strlen((char*)msg) + 1) < 0){
		strcpy((char*)usart2.Tx,"Error msg to Long");
	}
	else{
		strcpy((char*)usart2.Tx,(char*)msg);
	}

	usart2.Tx_len = strlen((char*)msg);
	usart2.write_index = 0;
}

/*USART2 write function with no interrupt.
 *This function writes msg written in buffet_Tx to USART2_DR.*/
void write_usart2(){


	while(usart2.write_index < usart2.Tx_len)
	{
		while(((USART2->SR) & 0x00000080) == 0x00000000);// wait while data is not yet transferd (TXE != 1)(see RM 27.6.1)
		USART2->DR = (uint8_t)(usart2.Tx[usart2.write_index] & 0xFF); //send data (see RM 27.6.2)
		//USART2->DR = (uint8_t)('U' & 0xFF); //for testing
		usart2.write_index++;
	}
	usart2.write_index = 0;
	while(((USART2->SR) & 0x00000040) !=  0x00000040); //wait until transmition is complete TC=1 (see RM 27.6.1)

}



/*USART2 intterupt handler*/
/*void USART2_IRQHandler(void){

	USART2->DR = (0x55 & 0xFF);//(uint8_t)('U' & 0xFF); //send data (see RM 27.6.2)
	//USART2->CR1 &= ~(0x00000080); // reset TXEIE no interrupt (see RM 27.6.4)

}*/



void set_usart1_buffer_Tx(uint8_t *command);
void set_usart1_buffer_Rx();
void write_usart1();
void read_usart1();


