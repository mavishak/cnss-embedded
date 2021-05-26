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



static USART usart2;
static USART usart1;
static uint8_t c; // for holding the USART->DR value


static uint8_t *START;  // for search UART2_write_line
static uint8_t *END;  // for search


BOOL USART1_NEW_LINE_FOUND_get(void){
	return usart1.new_line_found;
}

void USART1_NEW_LINE_READ_set(void){
	usart1.new_line_read = TRUE;
}


BOOL USART2_NEW_LINE_FOUND_get(void){
	return usart2.new_line_found;
}

void USART2_NEW_LINE_READ_set(void){
	usart2.new_line_read = TRUE;
}



void USART2_init(){

	/*This program works when TeraTerm speed is set to 9600 and USART_BRR is set to 0x34D.*/

	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; // (see RM 8.3.7)

	/*Configure USART2 Tx (PA2) as Output */
	GPIOA->CRL &= 0xFFFFF0FF; //Leave all bits as they are except for bit 2 (see RM 9.2.1)
	GPIOA->CRL |= 0x00000A00; //Configure as Alternate function output Push-pull | Speed 2 MHz (see RM 9.2.1)

	/*Configure USART2 Rx (PA3) as Input*/
	GPIOA->CRL &= 0xFFFF0FFF; //Leave all bits as they are except for bit 3 (see reference manual 9.2.2)
	//For Input Pull-Up (See reference manual p. 167)
	GPIOA->CRL |= 0x00008000; //configure as input with pull up/pull down (see reference manual 9.2.2).
	GPIOA->ODR |= 0x00000008;//(reference manual p. 161 and 9.2.4)


	/*Enable RCC for USART2*/
	RCC->APB1ENR |= 0x00020000; // (see reference manual 8.3.8)

	/*Enable RCC for Alternate funcion for PINs*/
	//RCC->APB2ENR |= 0x00000001; //  (see RM 8.3.7) //Is this line needed?? NO


	/*Following directions RM p. 792 (Setting Tx procesure)*/
	/*Following directions RM p. 795 (Setting Rx procesure) */
	USART2->CR1 |= 0x00002000; //Enable the USART by writing the UE bit in USART_CR1 register to 1 (see RM 27.6.4)
	//USART2->CR1 &= ~(0x00001000); //Program the M bit in USART_CR1 to define the word length to 8 (by default) (see RM 27.6.4)
	//USART2->CR1 &= ~(0x00000400); //Parity Controle Disable (by default) (see RM 27.6.4)
	//USART2->CR2 &= ~(0x00003000); //Program the number of stop bits in USART_CR2 to 1 (by defualt) (see RM 27.6.5)

	/*Set Baude rate*/
	USART2->BRR = 0x34D; //9600 bps (see RM p.798 for BRR calculation and RM p.93 Fig.8 for clock tree)

	/*Enable USART Transmit*/
	USART2->CR1 |= 0x00000008; // Set the TE bit in USART_CR1 to send an idle frame as first transmission. see RM 27.6.4)


	/*Enable USART Receive*/
	//USART2->CR1 |= 0x00000004;// Set the RE bit in USART_CR1 to enable USART Receive  (see RM 27.6.4)

	/*Enable USART Receive Interrupt*/
	 __disable_irq();
	USART2->CR1 |= 0x00000020; // Set RXNEIE to enable Rx interrupt(see RM 27.6.4)
	NVIC_SetPriorityGrouping(7); //This should disable interrupt nesting(priority wont be not allowed)//->MABY IT'S THE DEFAULT
	NVIC_SetPriority(USART2_IRQn,0); //set all interrupt priority to zero so that no preemption occurs.//->MABY IT'S THE DEFAULT
	NVIC_EnableIRQ(USART2_IRQn); //enable handler
	__enable_irq();
}

void USART1_init(){

	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; // (see RM 8.3.7)

	/*Configure USART1 Tx (PA9) as Output*/
	GPIOA->CRH &= 0xFFFFFF0F; //Leave all bits as they are except for bit 9 (see RM 9.2.2)
	GPIOA->CRH |= 0x000000A0; //Configure as Alternate function output Push-pull | max speed 2 MHz (See RM 9.2.2 and pg.181).


	/*Configure USART1 Rx (PA10) as Input*/
	GPIOA->CRH &= 0xFFFFF0FF; //Leave all bits as they are except for bit 10 (see RM 9.2.2)
	//For Input Pull-Up (See RM pg.167)
	GPIOA->CRH |= 0x00000800; //Configure as input with pull up/pull down (See RM 9.2.2).
	GPIOA->ODR |= 0x00000400;//(See RM pg.161 and 9.2.4)


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



void USART2_enable_Rx(void){

	USART2->CR1 |= 0x00000004;// set the RE bit in USART_CR1 to enable USART Receive  (see RM 27.6.4)

}

void USART2_disable_Rx(void){

	USART2->CR1 &= ~(0x00000004);// reset the RE bit in USART_CR1 to enable USART Receive  (see RM 27.6.4)

}

void USART2_set_buffer_Tx(uint8_t *msg){


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

void USART2_set_buffer_Rx(){

	memset(usart2.Rx, '\0', BUFF_SIZE*sizeof(uint8_t));
	usart2.Rx_len = 0;
	usart2.read_index = 0;

}

void USART2_write(uint8_t* msg){


	/*Set usart1_buffer_Tx with msg*/
	USART2_set_buffer_Tx(msg);

	/*Prepare buffer Rx for response*/
	USART2_set_buffer_Rx();

	// set initial values to Rx interrupt flags
	usart2.new_line_found = FALSE;
	usart2.new_line_read = TRUE;

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

BOOL USART2_read_buffer_Rx(uint32_t dest_size, uint8_t *dest){


	memset(dest, '\0', dest_size*sizeof(uint8_t));
	if((usart2.Rx_len + 1) < dest_size){

		uint32_t i = 0; // Rx index
		while(usart2.Rx[i] != '\r'){
			dest[i] = usart2.Rx[i];
			i++;
		}
		return TRUE;
	}
	return FALSE;

}

BOOL USART2_ok(void){

	if(strstr((const char*)usart2.Rx , (const char*)"ok\r\n")){
		return TRUE;
	}
	if(strstr((const char*)usart2.Rx , (const char*)"OK\r\n")){
		return TRUE;
	}
	if(strstr((const char*)usart2.Rx , (const char*)"Ok\r\n")){
		return TRUE;
	}
	return FALSE;

}





void USART1_write(uint8_t *command){


	/*Set usart1_buffer_Tx with command*/
	USART1_set_buffer_Tx(command);

	/*Prepare buffer Rx for response*/
	USART1_set_buffer_Rx();

	// set initial values to Rx interrupt flags
	usart1.new_line_found = FALSE;
	usart1.new_line_read = TRUE;

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

void USART1_set_buffer_Tx(uint8_t *command){

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

void USART1_set_buffer_Rx(){

	memset(usart1.Rx, '\0', BUFF_SIZE*sizeof(uint8_t));
	usart1.Rx_len = 0;
	usart1.read_index = 0;

	// FOR DEBUGGING SEARCH BUFFER RX //
	START = usart1.Rx;
	END = usart1.Rx;

}

STATE USART1_search_buffer_Rx(uint8_t *pass, uint8_t *fail){

	/*!TODO:need to check that usart1.Rx buffer wasn't overflow*/
	if((usart1.Rx_len + 1) < BUFF_SIZE){

		START = END;
		END = usart1.Rx + usart1.Rx_len;

		if(strstr((const char*)usart1.Rx , (const char*)pass)){
			USART2_write_line((uint8_t*)START, (uint8_t*)END); // write response to screen
			return PASS;
		}
		else if(strstr((const char*)usart1.Rx , (const char*)fail)){
			USART2_write_line((uint8_t*)START, (uint8_t*)END); // write response to screen
			return FAIL;
		}
		else{
			USART2_write_line((uint8_t*)START, (uint8_t*)END); // write response to screen
			return STANDBY;
		}

	}

	else{
		/*!TODO: when usart1.Rx buffer is overflown start check from end?*/
		USART2_write((uint8_t*)"\r\nBUFFER_OVERFLOW::RX BUFFER CONTENT\r\n");
		USART2_write((uint8_t*)usart1.Rx);
		return FAIL;
	}

}

SWITCH_STATE USART1_check_state_buffer_Rx(uint8_t *on, uint8_t *off,uint8_t *no_path){

	/*!TODO:need to check that usart1.Rx buffer wasn't overflow*/
	if((usart1.Rx_len + 1) < BUFF_SIZE){

		START = END;
		END = usart1.Rx + usart1.Rx_len;

		if(strstr((const char*)usart1.Rx , (const char*)on)){
			USART2_write_line((uint8_t*)START, (uint8_t*)END); //write response to screen
			return (uint32_t)ON;
		}
		else if(strstr((const char*)usart1.Rx , (const char*)off)){
			USART2_write_line((uint8_t*)START, (uint8_t*)END);  //write response to screen
			return (uint32_t)OFF;
		}
		else if(strstr((const char*)usart1.Rx , (const char*)no_path)){
			USART2_write_line((uint8_t*)START, (uint8_t*)END);  //write response to screen
			return (uint32_t)NO_PATH;
		}
		else{
			USART2_write((uint8_t*)usart1.Rx);//for debuging
			return (uint32_t)NON;
		}

	}

	else{
		/*!TODO: when usart1.Rx buffer is overflown start check from end??*/
		USART2_write((uint8_t*)"\r\nBUFFER_OVERFLOW::RX BUFFER CONTENT\r\n");
		USART2_write((uint8_t*)usart1.Rx);
		return (uint32_t)NO_PATH;
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

		if(c == (uint8_t)'\n' && usart1.new_line_read){
			usart1.new_line_found = TRUE;
			usart1.new_line_read = FALSE;
		}
		else if(c != (uint8_t)'\n' && usart1.new_line_read){
			usart1.new_line_found = FALSE;
		}

	}

}

/*USART2 Interrupt Handler - Only Rx is set to have interrupts*/
void USART2_IRQHandler(void){

	if(((USART2->SR) & 0x00000020) == 0x00000020){ //Check if RXNE=1, this means that Rx interrupt occurred (see RM 27.6.1)

		c = USART2->DR; // clear RXNE bit
		if((usart2.read_index + 1) >= BUFF_SIZE){
			usart2.read_index = 0;
		}
		usart2.Rx[usart2.read_index] = (uint8_t)(c & 0xFF);
		usart2.read_index++;
		usart2.Rx_len++; // count total chars received

		if(c == (uint8_t)'\n' && usart2.new_line_read){
			usart2.new_line_found = TRUE;
			usart2.new_line_read = FALSE;
		}
		else if(c != (uint8_t)'\n' && usart2.new_line_read){
			usart2.new_line_found = FALSE;
		}

	}

}





