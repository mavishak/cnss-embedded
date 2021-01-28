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
 * 08.01.2021: SOLVED!!! The USART2 clock is PCLK1 which is by default only 8MHz NOT 36MHz.
 * 			   (see RM p.798 for BRR calculation and RM p.93 Fig.8 for clock tree).
 * 			   So, basically our problem was that we thought that PCLK1 was set to 36MHz by default.
 * 			   For later practice follow the clock tree and default initiolization of clock registers.
 *
 * 22.01.2021: Setting the base lines for USART1 for communication with ESP8266
 * 			   MADE A FEW SMALL CHANGES TO USART2 init:
 * 			   1. commented out RCC->APB2ENR |= 0x00000001;
 * 			   2. changed static USART_2 usart2; //changed from buff
 * 			   need to check that it is still working...
 *
 * 23.01.2021: Writing code for USART1  - comunication with ESP8266
 * 			   NOTE: Code not Tested yet.
 *  		   Questions:
 *  		   1. How to know Rx is complete/receive? assumption: "\r\n" or "\r\n\r\n"
 *  		   2. When disabling Rx should we disable Rx interrupts as  well?
 *			   3. Maybe it will be smarter to call the set functions from the init?
 *			   4. What to do incase of buffer overflow in Receive interrupt?
 *			   5. Is it correct to have USART1_IRQHandler do all that it is doing now?
 *			   	  Our idea is that only when \r\n
 *			   	  is found and command is read handler will be se in queue to avoid over running response
 *
 * 24.01.2021: We haven't succeded to create a connection with ESP8266.
 * 			   the interrupt is not accessed.
 * 			   (We accessed it by mistake after tuching the pins)
 * 			   What we tried and didn't work:
 * 			   1. We tried clearing USART1-SR (TC - transmit complete) by reseting it's value
 * 			   2. input_floating instead os pull-up
 * 			   3. waiting
 * 			   4. Enable RCC for Alternate funcion for PINs - Highly unlikely... we don't think it should be enabled at all.
 *
 * 			   On another subject - The init_usart1() test prints are printed twice even though they are supposed to print once.
 * 			   We are pertty sure that the BRR is set currectly (but it might be worth the review...)
 *
 * 25.01.2021: We are maneging to get input from ESP8266 but not the currect one:
 * 			   1. It might not actually be input - maybe the \0 is printed again and again
 * 			      hard to believe- because the input grows each time.
 * 			   2. There is probably a problum of a buffer overflow or another programble problem that is not being taken care
 * 			      of since the computer is 'blinging'.
 * 			      blings - every time there is a \r\n on screen.
 * 			   3. Maybe we are enterring the ISR repitedly because the iterrupt flag is not being cleered.
 * 			      WHY ARE WE ENTERRING THE ISR SO MANT TIMES WE ONLY WANT TO GET OK??
 * 			   4. BRR problem with USART1 - maybe it shouldn't be similar to USART2.
 * 			   5. problem with jumper attachments - very likely. The ISR is entered only after connecting CH_PD (EN) to 3.3v
 * 			      powering it, playing and then reconnecting the USB.
 * 			   6. Tx in STM32 is using 5V - should be 3.3V.  Use a logic level shifter or voltage divider between it??
 *
 *
 *
 */

#include "usart.h"
#include "stm32f103xb.h"
#include "hc-sr501pir_sensor.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/
#include <string.h>
#include <stdlib.h>

static USART_2 usart2;
static USART_1 usart1;
static uint8_t c; // for holding the USART->DR value

/*This dunctions Inits all registors that have to do with enabling USART2 (ST-LINK/V.2)
 *inorder to send message to computer.
 *Note: Interrupts are not enabled intentionally.
 *This program works when TeraTerm speed is set to 9600*/
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



/* Usart1 will be use for communication with esp8266.
 * Generally,
 * By default, Rx interrupts will be enabled in our program
 * If you want to use Tx, call function Write_Uart1() that will send your msg and than enable Rx again */
void init_usart1(){

	write_usart2((uint8_t*)"In init_usart1\r\n");//TESTING

	//Can enable only Tx or Rx Each time (In Arduino we couldn't we can't remeber why)?? Only one at a time because they use the same data register

	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; // (see RM 8.3.7)

	/*Configure USART1 Tx (PA9) as Output*/
	GPIOA->CRH &= 0xFFFFFF0F; //Leave all bits as they are except for bit 9 (see RM 9.2.2)
	GPIOA->CRH |= 0x000000A0; //Configure as Alternate function output Push-pull | max speed 2 MHz (See RM 9.2.2 and pg.181).
	//Maybe this is supposed to be better:
	//GPIOA->CRH |= 0x000000B0; //Configure as Alternate function output Push-pull | Speed 50 MHz (see RM 9.2.2)

	/*Configure USART1 Rx (PA10) as Input*/
	GPIOA->CRH &= 0xFFFFF0FF; //Leave all bits as they are except for bit 10 (see RM 9.2.2)
	//For Input Pull-Up (See RM pg.167)
	GPIOA->CRH |= 0x00000800; //Configure as input with pull up/pull down (See RM 9.2.2).
	GPIOA->ODR |= 0x00000400;//(See RM pg.161 and 9.2.4)
	//For floating input (See RM pg.167)
	//GPIOA->CRH |= 0x00000400; //Configure as floating input (See RM 9.2.2)- might be better?


	/*Enable RCC for USART1*/
	RCC->APB2ENR |= 0x00004000; // (See RM 8.3.7)

	/*Enable RCC for Alternate funcion for PINs*/
	//RCC->APB2ENR |= 0x00000001; //  (see RM 8.3.7) //Is this line needed??

	/*Following directions RM pg.792 (Setting Tx procesure)*/
	/*Following directions RM pg.795 (Setting Rx procesure) */
	USART1->CR1 |= 0x00002000; //Enable the USART by writing the UE bit in USART_CR1 register to 1 (see RM 27.6.4)
	//USART1->CR1 &= ~(0x00001000); //Program the M bit in USART_CR1 to define the word length to 8 (by default) (see RM 27.6.4)
	//USART1->CR1 &= ~(0x00000400); //Parity Controle Disable (by default) (see RM 27.6.4)
	//USART1->CR2 &= ~(0x00003000); //Program the number of stop bits in USART_CR2 to 1 (by defualt) (see RM 27.6.5)

	/* SHOULD WE DO THIS?? (What is DMA)
	 * Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
	 * place. Configure the DMA register as explained in multibuffer communication.
	 * WE DONT THINK WE NEED IT.
	 */

	/*Set Baude Rate for USART1 115200 // !X9600 bpsX!*/
	USART1->BRR = 0x45; //0x34D; //9600 bps (see RM p.798 for BRR calculation and RM p.93 Fig.8 for clock tree) //We think that USART1&USART2 use the same clock (HSI)


	///*Enable Uart Transmit*/

	/*Init Receive buffer*/
	set_usart1_buffer_Rx();

	/*Enable USART Receive*/
	USART1->CR1 |= 0x00000004;// Set the RE bit in USART_CR1 to enable USART Receive  (see RM 27.6.4)

	write_usart2((uint8_t*)"End of init_usart1 - #1\r\n");

	/*Enable USART Receive Interrupt*/
	 __disable_irq();
	USART1->CR1 |= 0x00000020; // Set RXNEIE (see RM 27.6.4)
	NVIC_SetPriority(USART1_IRQn,0); //set all interrupt priority to zero so that no preemption occurs.
	NVIC_EnableIRQ(USART1_IRQn); //enable handler
	USART1->SR &= ~(0x00000020);
	NVIC_ClearPendingIRQ(USART1_IRQn);
	__enable_irq();

	//write_usart2((uint8_t*)"End of init_usart1 - #2\r\n");
}


/*This function sets the Tx buffer up with chosen message.
 * One may choose to use the default MSG defined in usart.h*/
void set_usart2_buffer_Tx(uint8_t *msg){


	memset(usart2.Tx, '\0', BUFF_SIZE*sizeof(uint8_t));
	if((BUFF_SIZE - strlen((char*)msg) + 1) < 0){
		strcpy((char*)usart2.Tx,"Error msg to Long");
		usart2.Tx_len = strlen((char*)"Error msg to Long");
	}
	else{
		strcpy((char*)usart2.Tx,(char*)msg);
		usart2.Tx_len = strlen((char*)msg);
	}

	usart2.write_index = 0;

}


/*USART2 write function with no interrupt.
 *This function writes msg written in buffet_Tx to USART2_DR.*/
void write_usart2(uint8_t* msg){


	set_usart2_buffer_Tx(msg);

	//USART2->SR &= ~(0x00000040); //WE DON"T THINK THIS WILL HELP.. TC IS CLEARED AUTUMATICALLY BY WRTING TO DR.
	while(usart2.write_index < usart2.Tx_len)
	{
		while(((USART2->SR) & 0x00000080) == 0x00000000);// wait while data is not yet transferd (TXE != 1)(see RM 27.6.1)
		USART2->DR = (uint8_t)(usart2.Tx[usart2.write_index] & 0xFF); //send data (see RM 27.6.2)
		usart2.write_index++;
	}
	usart2.write_index = 0;
	usart2.Tx_len = 0;
	while(((USART2->SR) & 0x00000040) !=  0x00000040); //wait until transmition is complete TC=1 (see RM 27.6.1)

}

/*USART1 write function with no interrupt.*/
void write_usart1(uint8_t *command){

	/*Disable Rx*/
	USART1->CR1 &= ~(0x00000004);// Reset the RE bit in USART_CR1 to disable USART Receive see RM 27.6.4)

	///*Disable Rx interrupt?...*/

	/*Enable Tx*/
	USART1->CR1 |= 0x00000008; // Set the TE bit in USART_CR1 to send an idle frame as first transmission. see RM 27.6.4)

	/*Set Usart usart1_buffer_Tx with command*/
	set_usart1_buffer_Tx(command);

	/*Send command*/
	//USART1->SR &= ~(0x00000040); //WE DON"T THINK THIS WILL HELP.. TC IS CLEARED AUTUMATICALLY BY WRTING TO DR.
	while(usart1.write_index < usart1.Tx_len)
	{
		while(((USART1->SR) & 0x00000080) == 0x00000000);// wait while data is not yet transferd (TXE != 1)(see RM 27.6.1)
		USART1->DR = (uint8_t)(usart1.Tx[usart1.write_index] & 0xFF); //send data (see RM 27.6.2)
		usart1.write_index++;
	}
	usart1.write_index = 0;
	usart1.Tx_len = 0;

	/*Wait 'till transmit complete (TC=1) after sending  full msg*/
	while(((USART1->SR) & 0x00000040) !=  0x00000040); //wait until transmition is complete TC=1 (see RM 27.6.1)

	/*Disable tx*/
	USART1->CR1 &= ~(0x00000008);// Reset the TE bit in USART_CR1 to disable USART transmit (see RM 27.6.4)

	/*Enable Rx*/
	USART1->CR1 |= 0x00000004;// Set the RE bit in USART_CR1 to enable USART Receive  (see RM 27.6.4)

	///*Enable Rx interrupts?...*/

}


void set_usart1_buffer_Tx(uint8_t *command){

	/*Write command into usart1_buffer_Tx*/
	memset(usart1.Tx, '\0', BUFF_SIZE*sizeof(uint8_t));
	if((BUFF_SIZE - strlen((char*)command) + 1) < 0){
		strcpy((char*)usart1.Tx,"Error command to Long");
		usart1.Tx_len = strlen((char*)"Error command to Long");
	}
	else{
		strcpy((char*)usart1.Tx,(char*)command);
		usart1.Tx_len = strlen((char*)command);
	}
	usart1.write_index = 0;

}


/*This function inits Rx buffer variables - should be called in init_usart1*/
void set_usart1_buffer_Rx(){

	memset(usart1.Rx, '\0', BUFF_SIZE*sizeof(uint8_t));
	usart1.Rx_len = 0;
	usart1.read_index = 0;

}


void read_usart1(){ //THIS IS NO GOOD! ANOTHER INTERRUPT CAN ACCUR IN THE MIDDLE

	write_usart2((uint8_t*)"In read_usart1\r\n");

	write_usart2((uint8_t*)usart1.Rx); //write response to screen

	write_usart2((uint8_t*)"End of read_usart1\r\n");

	/*Read character until '\r\n'*/

	//than check response (in different function)

}

/*USART2 intterupt handler*/
/*void USART2_IRQHandler(void){

	USART2->DR = (0x55 & 0xFF);//(uint8_t)('U' & 0xFF); //send data (see RM 27.6.2)
	//USART2->CR1 &= ~(0x00000080); // reset TXEIE no interrupt (see RM 27.6.4)

}*/

/*USART1 Interrupt Handler - Only Rx is set to have interrupts*/
void USART1_IRQHandler(void){

	//write_usart2((uint8_t*)"In USART1_IRQHandler\r\n");
	//int i = 5;
	//if(i==8);
	c = USART1->DR;
	/*
	if((BUFF_SIZE - usart1.Rx_len + 1) < 0 ){
		//OverFlow - Do Something
		write_usart2((uint8_t*)"Over Flow Error - USART1 Rx Buffer"); //write response to screen
	}
	else{
		usart1.Rx[usart1.read_index] = (uint8_t)(c & 0xFF);
		usart1.read_index++;
		usart1.Rx_len++;

		//FOR STARTERS..
		if(c == (uint8_t)'\n'){ // for now this is enough later change this according to diffrent endings "\r\n" OR "\r\n\r\n"
			write_usart2((uint8_t*)"at line c == \n");
			read_usart1();
			set_usart1_buffer_Rx(); //reset buffer
		}
	}

	write_usart2((uint8_t*)"usart1.Rx = ");
	write_usart2((uint8_t*)usart1.Rx);
	write_usart2((uint8_t*)"\r\n");
	write_usart2((uint8_t*)"End USART1_IRQHandler\r\n");
	*/


}







