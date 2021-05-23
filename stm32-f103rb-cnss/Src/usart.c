/*
 * usart.c
 *
 *  Created on: December 22 2020
 *      Author: Mayan and Naomi
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


static BOOL USART1_NEW_LINE_FOUND; // This variable flags a new line in USART1 Rx
static BOOL USART1_NEW_LINE_READ;

BOOL USART1_NEW_LINE_FOUND_get(void){
	return USART1_NEW_LINE_FOUND;
}

void USART1_NEW_LINE_READ_set(void){
	USART1_NEW_LINE_READ = TRUE;
}

static uint8_t *START; // for search
static uint8_t *END;

/*This functions Inits all registors that have to do with enabling USART2 (ST-LINK/V.2)
 *inorder to send message to computer.
 *Note: Interrupts are not enabled intentionally.
 *This program works when TeraTerm speed is set to 9600*/
void USART2_init(){

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


/* Usart1 will be use for communication with esp8266. */
void USART1_init(){

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
	//RCC->APB2ENR |= 0x00000001; //  (see RM 8.3.7) //Is this line needed?? NO

	/*Following directions RM pg.792 (Setting Tx procesure)*/
	/*Following directions RM pg.795 (Setting Rx procesure) */
	USART1->CR1 |= 0x00002000; //Enable the USART by writing the UE bit in USART_CR1 register to 1 (see RM 27.6.4)
	//USART1->CR1 &= ~(0x00001000); //Program the M bit in USART_CR1 to define the word length to 8 (by default) (see RM 27.6.4)
	//USART1->CR1 &= ~(0x00000400); //Parity Controle Disable (by default) (see RM 27.6.4)
	//USART1->CR2 &= ~(0x00003000); //Program the number of stop bits in USART_CR2 to 1 (by defualt) (see RM 27.6.5)


	/*Set Baude Rate for USART1 115200 (The optimal buadrate for AT COMMANDS)*/
	USART1->BRR = 0x45; // 115200 bps (see RM p.798 for BRR calculation and RM p.93 Fig.8 for clock tree) //We think that USART1&USART2 use the same clock (HSI)


	/*Enable Tx*/
	USART1->CR1 |= 0x00000008; // Set the TE bit in USART_CR1 to send an idle frame as first transmission. see RM 27.6.4)


	/*Enable USART Receive*/
	USART1->CR1 |= 0x00000004;// Set the RE bit in USART_CR1 to enable USART Receive  (see RM 27.6.4)


	/*Enable USART Receive Interrupt*/
	 __disable_irq();
	USART1->CR1 |= 0x00000020; // Set RXNEIE to enable Rx interrupt(see RM 27.6.4)
	NVIC_SetPriorityGrouping(7); //This should disable interrupt nesting(priority wont be not allowed)//->MABY IT'S THE DEFAULT
	NVIC_SetPriority(USART1_IRQn,0); //set all interrupt priority to zero so that no preemption occurs.//->MABY IT'S THE DEFAULT
	NVIC_EnableIRQ(USART1_IRQn); //enable handler
	__enable_irq();

}


/*This function sets the Tx buffer up with chosen message.
 * One may choose to use the default MSG defined in usart.h*/
void set_usart2_buffer_Tx(uint8_t *msg){


	memset(usart2.Tx, '\0', BUFF_SIZE*sizeof(uint8_t));
	if((BUFF_SIZE - strlen((char*)msg) + 1) < 0){
		strcpy((char*)usart2.Tx,"Error msg to Long\r\n");
		usart2.Tx_len = strlen((char*)"Error msg to Long\r\n");
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

	while(usart2.write_index < usart2.Tx_len)
	{
		while(((USART2->SR) & 0x00000080) == 0x00000000);// wait while data is not yet transfered (TXE != 1)(see RM 27.6.1)
		USART2->DR = (uint8_t)(usart2.Tx[usart2.write_index] & 0xFF); //send data (see RM 27.6.2)
		usart2.write_index++;
	}
	while(((USART2->SR) & 0x00000040) !=  0x00000040); //wait until transmission is complete TC=1 (see RM 27.6.1)
	usart2.write_index = 0;
	usart2.Tx_len = 0;

}


void USART2_write_line(uint8_t *start, uint8_t *end){

	// Set buffer Tx
	memset(usart2.Tx, '\0', BUFF_SIZE*sizeof(uint8_t));
	if((BUFF_SIZE - (end - start) + 1) < 0){
		strcpy((char*)usart2.Tx,"Error msg to Long\r\n");
		usart2.Tx_len = strlen((char*)"Error msg to Long\r\n");
	}
	else{
		uint32_t i = 0;
		usart2.Tx_len = end - start;
		while(start < end){
			usart2.Tx[i] = *start;
			start++;
			i++;
		}

	}

	// Write to screen
	usart2.write_index = 0;

	while(usart2.write_index < usart2.Tx_len)
	{
		while(((USART2->SR) & 0x00000080) == 0x00000000);// wait while data is not yet transfered (TXE != 1)(see RM 27.6.1)
		USART2->DR = (uint8_t)(usart2.Tx[usart2.write_index] & 0xFF); //send data (see RM 27.6.2)
		usart2.write_index++;
	}
	while(((USART2->SR) & 0x00000040) !=  0x00000040); //wait until transmission is complete TC=1 (see RM 27.6.1)
	usart2.write_index = 0;
	usart2.Tx_len = 0;
}


/*USART1 write function with no interrupt.*/
void write_usart1(uint8_t *command){


	/*Set usart1_buffer_Tx with command*/
	set_usart1_buffer_Tx(command);

	/*Prepare buffer Rx for response*/
	set_usart1_buffer_Rx();

	// set initial values to Rx interrupt flags
	USART1_NEW_LINE_FOUND = FALSE;
	USART1_NEW_LINE_READ = TRUE;

	/*Send command*/
	while(usart1.write_index < usart1.Tx_len)
	{
		while(((USART1->SR) & 0x00000080) == 0x00000000);// wait while data is not yet transfered (TXE != 1)(see RM 27.6.1)
		USART1->DR = (uint8_t)(usart1.Tx[usart1.write_index] & 0xFF); //send data (see RM 27.6.2)
		usart1.write_index++;
	}
	while(((USART1->SR) & 0x00000040) !=  0x00000040); //wait until transmission is complete TC=1 (see RM 27.6.1)
	usart1.write_index = 0;
	usart1.Tx_len = 0;

}


void set_usart1_buffer_Tx(uint8_t *command){

	/*Write command into usart1_buffer_Tx*/
	memset(usart1.Tx, '\0', BUFF_SIZE*sizeof(uint8_t));
	if((BUFF_SIZE - strlen((char*)command) + 1) < 0){
		strcpy((char*)usart1.Tx,"Error command to Long\r\n");
		usart1.Tx_len = strlen((char*)"Error command to Long\r\n");
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

	// FOR DEBUGGING SEARCH BUFFER RX //
	START = usart1.Rx;
	END = usart1.Rx;

}



/*This function returns 3 values type STATE - defined by common.h:
 * PASS - when pass param is found.
 * FAIL - when fail param is found.
 * STANDBY - when neither pass param or fail param are found.*/
STATE USART1_search_buffer_Rx(uint8_t *pass, uint8_t *fail){

	/*!TODO:need to check that usart1.Rx buffer wasn't overflow*/
	if((usart1.Rx_len + 1) < BUFF_SIZE){

		START = END;
		END = usart1.Rx + usart1.Rx_len;

		if(strstr((const char*)usart1.Rx , (const char*)pass)){
			USART2_write_line((uint8_t*)START, (uint8_t*)END); // write response to screen
			return (uint32_t)PASS;
		}
		else if(strstr((const char*)usart1.Rx , (const char*)fail)){
			USART2_write_line((uint8_t*)START, (uint8_t*)END); // write response to screen
			return (uint32_t)FAIL;
		}
		else{
			USART2_write_line((uint8_t*)START, (uint8_t*)END); // write response to screen
			return (uint32_t)STANDBY;
		}

	}

	else{
		/*!TODO: when usart1.Rx buffer is overflown start check from end?*/
		write_usart2((uint8_t*)"\r\nBUFFER_OVERFLOW::RX BUFFER CONTENT\r\n");
		write_usart2((uint8_t*)usart1.Rx);
		return (uint32_t)FAIL;
	}

}

/*this function searches USART1 buffer Rx for on or off, to be used
 * in esp8266_Firebase.c in searchSwitchState() -> parseResponse.
 * This function does NOT clean buffer*/
SWITCH_STATE find_state_usart1_Buffer_Rx(uint8_t *on, uint8_t *off,uint8_t *no_path){

	/*!TODO:need to check that usart1.Rx buffer wasn't overflow*/
	if((usart1.Rx_len + 1) < BUFF_SIZE){

		if(strstr((const char*)usart1.Rx , (const char*)on)){
			write_usart2((uint8_t*)usart1.Rx); //write response to screen
			return (uint32_t)ON;
		}
		else if(strstr((const char*)usart1.Rx , (const char*)off)){
			write_usart2((uint8_t*)usart1.Rx); //write response to screen
			return (uint32_t)OFF;
		}
		else if(strstr((const char*)usart1.Rx , (const char*)no_path)){
			write_usart2((uint8_t*)usart1.Rx); //write response to screen
			return (uint32_t)NO_PATH;
		}
		else{
			write_usart2((uint8_t*)usart1.Rx);//for debuging
			return (uint32_t)NON;
		}

	}

	else{
		/*!TODO: when usart1.Rx buffer is overflown start check from end??*/
		write_usart2((uint8_t*)"\r\nBUFFER_OVERFLOW\r\n");
		write_usart2((uint8_t*)usart1.Rx);
		return (uint32_t)NON;
	}

}

/*USART1 Interrupt Handler - Only Rx is set to have interrupts*/
void USART1_IRQHandler(void){

	if(((USART1->SR) & 0x00000020) == 0x00000020){ //Check if RXNE=1, this means that Rx interrupt occurred (see RM 27.6.1)

		c = USART1->DR; // clear RXNE bit
		if((usart1.read_index + 1) >= BUFF_SIZE){
			usart1.read_index = 0;
		}
		usart1.Rx[usart1.read_index] = (uint8_t)(c & 0xFF);
		usart1.read_index++;
		usart1.Rx_len++; // count total chars received


		//***

		if(c == (uint8_t)'\n' && USART1_NEW_LINE_READ){
			USART1_NEW_LINE_FOUND = TRUE;
			USART1_NEW_LINE_READ = FALSE;
		}
		else if(c != (uint8_t)'\n' && USART1_NEW_LINE_READ){
			USART1_NEW_LINE_FOUND = FALSE;
		}


	}


}



/*USART2 intterupt handler*/
/*void USART2_IRQHandler(void){

	USART2->DR = (0x55 & 0xFF);//(uint8_t)('U' & 0xFF); //send data (see RM 27.6.2)
	//USART2->CR1 &= ~(0x00000080); // reset TXEIE no interrupt (see RM 27.6.4)

}*/





