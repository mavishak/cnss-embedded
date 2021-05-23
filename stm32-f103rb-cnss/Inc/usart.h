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

void USART2_init();
void set_usart2_buffer_Tx(uint8_t *msg);
void write_usart2(uint8_t *msg);//This function should call set_usart2_buffer_Tx

void USART1_init();
void set_usart1_buffer_Tx(uint8_t *command);
void set_usart1_buffer_Rx();
STATE USART1_search_buffer_Rx(uint8_t *pass, uint8_t *fail);
SWITCH_STATE find_state_usart1_Buffer_Rx(uint8_t *on, uint8_t *off,uint8_t *no_path);
void write_usart1(uint8_t *command);//This function should call set_usart1_buffer_Tx


/*returns TRUE if a new line was found in Rx buffer*/
BOOL USART1_NEW_LINE_FOUND_get(void);

/*sets flag NEW_LINE_READ - must be called after using: USART1_search_buffer_Rx and find_state_usart1_Buffer_Rx*/
void USART1_NEW_LINE_READ_set(void);

/*special function writes content in USART1 Rx buffer: from start to end to the screen*/
void USART2_write_line(uint8_t *start, uint8_t *end);

typedef struct usart2{

	uint8_t Tx[BUFF_SIZE];
	uint32_t Tx_len;
	uint32_t write_index;

}USART_2;


typedef struct usart1{

	uint8_t Tx[BUFF_SIZE];
	uint8_t Rx[BUFF_SIZE];
	uint32_t Tx_len;
	uint32_t Rx_len;
	uint32_t write_index;
	uint32_t read_index;

}USART_1;



#endif /* USART_H_ */
