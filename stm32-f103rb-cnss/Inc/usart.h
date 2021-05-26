/*
 * usart.h
 *
*  Created on: December 22 2020
 *      Author: Mayan and Naomi
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f103xb.h" //this file is included to every header file
#include <stdlib.h>
#include "common.h"


#define BUFF_SIZE 512

/*This functions Inits all registors that have to do with enabling USART2 (ST-LINK/V.2)
 *inorder to send message to computer.
 *Note: Interrupts are not enabled intentionally.
 *This program works when TeraTerm speed is set to 9600*/
void USART2_init();

/*This function sets the Tx buffer up with chosen message.*/
void USART2_set_buffer_Tx(uint8_t *msg);

/*This function inits Rx buffer variables*/
void USART2_set_buffer_Rx(void);

/*USART2 write function with no interrupt.
 *This function writes msg written in buffet_Tx to USART2_DR.*/
void USART2_write(uint8_t *msg);

/*special function writes USART1 Rx buffer (start to end) to the screen
 * param start and end are pointers to usart1.Rx buffer*/
void USART2_write_line(uint8_t *start, uint8_t *end);

/*returns TRUE if a new line was found in Rx buffer*/
BOOL USART2_NEW_LINE_FOUND_get(void);

/*sets flag NEW_LINE_READ - must be called after reading line*/
void USART2_NEW_LINE_READ_set(void);

/*this function coppies the content of buffer Rx in to dest
 * returns TRUE if dest was succefuully set, otherwise returns FALSE*/
BOOL USART2_read_buffer_Rx(uint32_t dest_size, uint8_t *dest);

BOOL USART2_ok(void);

/*Enable Rx only when user needs to write*/
void USART2_enable_Rx(void);

/*Disable Rx when user finished writing*/
void USART2_disable_Rx(void);





/* Usart1 will be use for communication with esp8266. */
void USART1_init();

/*This function sets the Tx buffer up with chosen command.*/
void USART1_set_buffer_Tx(uint8_t *command);

/*This function inits Rx buffer variables*/
void USART1_set_buffer_Rx();

/*This function returns 3 values type STATE - defined by common.h:
 * PASS - when pass param is found.
 * FAIL - when fail param is found.
 * STANDBY - when neither pass param or fail param are found.*/
STATE USART1_search_buffer_Rx(uint8_t *pass, uint8_t *fail);

/*this function searches USART1 buffer Rx for on or off, to be used
 * in esp8266_Firebase.c in searchSwitchState() -> parseResponse.
 * This function does NOT clean buffer*/
SWITCH_STATE USART1_check_state_buffer_Rx(uint8_t *on, uint8_t *off,uint8_t *no_path);

/*USART1 write function with no interrupt.*/
void USART1_write(uint8_t *command);


/*returns TRUE if a new line was found in Rx buffer*/
BOOL USART1_NEW_LINE_FOUND_get(void);

/*sets flag NEW_LINE_READ - must be called after using: USART1_search_buffer_Rx and USART1_check_state_buffer_Rx*/
void USART1_NEW_LINE_READ_set(void);





typedef struct usart{

	uint8_t Tx[BUFF_SIZE];
	uint8_t Rx[BUFF_SIZE];
	uint32_t Tx_len;
	uint32_t Rx_len;
	uint32_t write_index;
	uint32_t read_index;
	BOOL new_line_found; // This variable flags a new line in Rx
	BOOL new_line_read; // This variable needs to be set when new line in Rx is read

}USART;



#endif /* USART_H_ */
