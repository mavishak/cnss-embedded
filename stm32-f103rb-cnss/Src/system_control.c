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
#include "timers.h"

uint8_t SSID[WiFi_SIZE]; // extern see configurations
uint8_t PWD[WiFi_SIZE];  // extern see configurations
uint8_t DEVICE_ID[ID_SIZE]; // extern see configurations



BOOL SYSTEM_CONTROL_set_up(void){

	USART2_write((uint8_t*)("\033[96m\r\n\r\nDear user, thank you for participating in our project!\r\n\033[0m"));
	USART2_write((uint8_t*)("\033[96mJust a few more steps...\r\n\r\n\033[0m"));
	USART2_write((uint8_t*)("\033[96mThe security device needs to connect to your WiFi network.\r\n\033[0m"));
	USART2_write((uint8_t*)("\033[96mFor that, we need you to enter your network connection information.\r\n\033[0m"));
	USART2_write((uint8_t*)("\033[96mFor your assurance, this information will not be saved in any database!\r\n\033[0m"));

	// set network ssid
	USART2_set_buffer_Rx(); // reset buffer
	while(!USART2_ok()){
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\033[32m\r\nEnter network name (case-sensitive): \033[0m");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();

		while(!USART2_read_buffer_Rx(WiFi_SIZE, SSID)){
			USART2_NEW_LINE_READ_set();

			USART2_write((uint8_t*)"\033[91m\r\nThe network name is too long, try again: \033[0m");
			USART2_enable_Rx();
			while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
			USART2_disable_Rx();
		}
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\033[32mEnter 'ok' to continue or any other key to change network name: \033[0m");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();
	}
	USART2_NEW_LINE_READ_set();


	// set network password
	USART2_set_buffer_Rx(); // reset buffer
	while(!USART2_ok()){
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\033[32m\r\nEnter network access code (password): \033[0m");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();

		while(!USART2_read_buffer_Rx(WiFi_SIZE, PWD)){
			USART2_NEW_LINE_READ_set();

			USART2_write((uint8_t*)"\033[91m\r\nThe access code is to long, try again: \033[0m");
			USART2_enable_Rx();
			while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
			USART2_disable_Rx();
		}
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\033[32mEnter 'ok' to continue or any other key to change password: \033[0m");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();
	}
	USART2_NEW_LINE_READ_set();

	USART2_write((uint8_t*)"\033[96m\r\nTrying to connect...\r\nPlease wait, this might take a few minutes.\033[0m");
	USART2_write((uint8_t*)"\r\n\r\n");
	TIMER4_delay(5);
	return registeration_Handler();
}

void SYSTEM_CONTROL_monitor_switch_state(uint32_t seconds){

	if(TIMER2_timeout_done()){
		QUEUE_add_event(control_Handler);
		TIMER2_set_timeout(seconds);
	}
}



