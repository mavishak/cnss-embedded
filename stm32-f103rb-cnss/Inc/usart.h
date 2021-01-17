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

void init_usart2();
void set_usart2_buffer_Tx(uint8_t *msg);
void write_usart2();

void init_usart1();
void set_usart1_buffer_Tx(uint8_t *command);
void set_usart1_buffer_Rx();
void write_usart1();
void read_usart1();
//void delay();

#define BUFF_SIZE 128
#define MSG "A suspicious movement has been detected near a security system you are connected to.\r\nPlease check your messages online.\r\n\r\n"

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
