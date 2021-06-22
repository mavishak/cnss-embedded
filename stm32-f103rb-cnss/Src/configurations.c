/*
 * configurations.c
 *
 *  Created on: 28 05 2021
 *      Author: mayan and naomi
 */

#include "configurations.h"
#include "usart.h"
#include "system_control.h"
#include <stdio.h>
#include <string.h>

static struct u_id ID;
BOOL RESET_FLAG;

void CONFIGURATIONS_uid_read(struct u_id *id){

	id->off0 = MMIO16(ID_ADDR + 0x0);
	id->off2 = MMIO16(ID_ADDR + 0x2);
	id->off4 = MMIO32(ID_ADDR + 0x4);
	id->off8 = MMIO32(ID_ADDR + 0x8);

}

void CONFIGURATIONS_set_device_id(void){

	CONFIGURATIONS_uid_read(&ID);
	memset(DEVICE_ID,'\0', ID_SIZE*sizeof(uint8_t));
	sprintf((char*)DEVICE_ID,"%X%X%lX%lX",ID.off0,ID.off2,ID.off4,ID.off8);

}

BOOL CONFIGURATIONS_set_network(void){

	USART2_write((uint8_t*)("\033[96m\r\nIf at some point nothing seems to be happening\033[0m"));
	USART2_write((uint8_t*)("\033[96m\r\ndisconnect the device from your computer, wait a few minutes, and then reconnect.\r\n\033[0m"));


	if(!SYSTEM_CONTROL_set_up()){
		USART2_write((uint8_t*)("\033[91m\r\nNetwok connection error.\033[0m"));
		USART2_write((uint8_t*)("\033[96m\r\nIf you are sure you entered the correct network name and password,\r\ndo the following:\033[0m"));
		USART2_write((uint8_t*)("\033[96m\r\nCheck your network signal, is it strong enough?\033[0m"));
		USART2_write((uint8_t*)("\033[96m\r\nIf it is not, try again later or from another physical location.\033[0m"));
		USART2_write((uint8_t*)("\033[96mIf it is, hit the reset button or disconnect the device from the computer,\033[0m"));
		USART2_write((uint8_t*)("\033[96m\r\nwait a few minutes, then reconnect and try again\033[0m"));
		USART2_write((uint8_t*)"\r\n\r\n");
		return FALSE;
	}
	else{
		USART2_write((uint8_t*)("\033[96m\r\nYour device was successfully registered.\033[0m"));
		USART2_write((uint8_t*)("\033[96m\r\nThis is your device ID: \033[0m"));
		USART2_write(DEVICE_ID);
		USART2_write((uint8_t*)("\033[32m\r\nCoppy it, you will need it later.\033[0m"));
		USART2_write((uint8_t*)("\033[96m\r\n\r\nYou can now safely disconnect the device and connect it to an electric power supply.\033[0m"));
		USART2_write((uint8_t*)"\033[96m\r\nIf you ever wish to reset your network details,\r\nconnect the device to the computer and open 'TeraTerm'.\033[0m");
		USART2_write((uint8_t*)"\r\n\r\n");
		return TRUE;
	}

}

void* configuration_Handler(void){

	CONFIGURATIONS_set_network(); // returns TRUE upon success

	USART2_write((uint8_t*)"\033[32m\r\nEnter 'ok' to continue: \033[0m");
	USART2_enable_Rx();
	while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
	USART2_disable_Rx();

	while(!USART2_ok()){
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\033[32m\r\nEnter 'ok' to continue: \033[0m");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();

	}
	USART2_NEW_LINE_READ_set();

	RESET_FLAG = TRUE; // allow another event just like this
	USART2_enable_Rx();
	return NULL;
}


