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

	USART2_write((uint8_t*)("\r\nWord to the wise: if at some point nothing seems to be happening"));
	USART2_write((uint8_t*)("\r\ndisconnect the device from your computer, wait a few minutes, and then reconnect.\r\n"));


	if(!SYSTEM_CONTROL_set_up()){
		USART2_write((uint8_t*)("\r\nNetwok connection error."));
		USART2_write((uint8_t*)("\r\nIf you are sure you entered the correct network name and password,\r\ndo the following:"));
		USART2_write((uint8_t*)("\r\nCheck your network signal, is it strong enough?"));
		USART2_write((uint8_t*)("\r\nIf it is not, try again later or from another physical location."));
		USART2_write((uint8_t*)("\r\nIf it is, hit the reset button or disconnect the device from the computer, wait a few minutes, then reconnect and try again."));
		USART2_write((uint8_t*)"\r\n\r\n");
		return FALSE;
	}
	else{
		USART2_write((uint8_t*)("\r\nYour device was successfully registered."));
		USART2_write((uint8_t*)("\r\nThis is your device ID: '"));
		USART2_write(DEVICE_ID);
		USART2_write((uint8_t*)("'\r\nPlease coppy it, you will need it later."));
		USART2_write((uint8_t*)("\r\n\r\nYou can now safely disconnect the device and connect it to an electric power supply."));
		USART2_write((uint8_t*)"\r\nIf you ever wish to reset your network details,\r\nconnect the device to the computer and open 'TeraTerm'.");
		USART2_write((uint8_t*)"\r\n\r\n");
		return TRUE;
	}

}

void* configuration_Handler(void){

	CONFIGURATIONS_set_network(); // returns TRUE upon success

	USART2_write((uint8_t*)"\r\nEnter 'ok' to continue: ");
	USART2_enable_Rx();
	while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
	USART2_disable_Rx();

	while(!USART2_ok()){
		USART2_NEW_LINE_READ_set();

		USART2_write((uint8_t*)"\r\nEnter 'ok' to continue: ");
		USART2_enable_Rx();
		while(!USART2_NEW_LINE_FOUND_get()); // wait for users input
		USART2_disable_Rx();
	}
	USART2_NEW_LINE_READ_set();

	RESET_FLAG = TRUE; // allow another event just like this
	USART2_enable_Rx();
	return NULL;
}


