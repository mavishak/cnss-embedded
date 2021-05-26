/*
 * system_control.c
 *
 *  Created on: 6 05 2021
 *      Author: mayan
 */

#include "system_control.h"
#include "timers.h"
#include "esp8266_Firebase.h"
#include "usart.h"
#include "event_queue.h"
#include "system_control.h"
#include "configurations.h"

uint8_t SSID[SIZE];
uint8_t PWD[SIZE];


BOOL SYSTEM_CONTROL_set_up(void){

	USART2_write((uint8_t*)("\r\nDear user, thankyou for being a part of our project!\r\n"));
	USART2_write((uint8_t*)("Just a few more steps...\r\n\r\n"));
	USART2_write((uint8_t*)("In order to register your device we need to set your network details.\r\n"));
	USART2_write((uint8_t*)("For your assurance, this information will not be saved in any database!\r\n"));

	// set network ssid
	while(!USART2_ok()){
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\r\nPlease enter your network ssid: ");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();

		while(!USART2_read_buffer_Rx(SIZE, SSID)){
			USART2_NEW_LINE_READ_set();

			USART2_write((uint8_t*)"\r\nThe network ssid is to long, please try again: ");
			USART2_enable_Rx();
			while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
			USART2_disable_Rx();
		}
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\r\nYour network ssid is '");
		USART2_write(SSID);
		USART2_write((uint8_t*)"'\r\n");

		USART2_write((uint8_t*)"Enter 'ok' to continue or any other key to change network ssid: ");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();
	}
	USART2_NEW_LINE_READ_set();


	// set network password
	USART2_set_buffer_Rx(); // reset buffer
	while(!USART2_ok()){
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\r\nPlease enter the network password: ");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();

		while(!USART2_read_buffer_Rx(SIZE, PWD)){
			USART2_NEW_LINE_READ_set();

			USART2_write((uint8_t*)"\r\nThe password is to long, please try again: ");
			USART2_enable_Rx();
			while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
			USART2_disable_Rx();
		}
		USART2_NEW_LINE_READ_set();


		USART2_write((uint8_t*)"\r\nYour network password is '");
		USART2_write(PWD);
		USART2_write((uint8_t*)"'\r\n");

		USART2_write((uint8_t*)"Enter 'ok' to continue or any other key to change password: ");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();
	}
	USART2_NEW_LINE_READ_set();

	USART2_write((uint8_t*)"Trying to connect...\r\nPlease wait, this might take a few minutes.\r\n");
	return registeration_Handler();
}

void SYSTEM_CONTROL_monitor_switch_state(uint32_t seconds){

	if(TIMER2_timeout_done()){
		add_event(control_Handler);
		TIMER2_set_timeout(seconds);
	}
}



