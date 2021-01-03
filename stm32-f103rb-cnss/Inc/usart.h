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

void init_usart();
void init_buffer_Tx(uint8_t *msg);
void write();
void delay();

#define BUFF_SIZE 128
#define MSG "You are recivieng this msg beacaus a suspicious movement has been detected near a security system you are connected to.\r\n Please check your messages online\r\n"
typedef struct buffer{

	uint8_t Tx[BUFF_SIZE];
	uint32_t Tx_len;
	uint32_t write_index;


}Buffer;



#endif /* USART_H_ */
