/*
 * esp8266_Firbase.c
 *
 *  Created on: 09/02/2021
 *      Author: mayan and naomi
 */

#include "esp8266_Firebase.h"
#include "usart.h"

#define FALSE 0
#define TRUE 1

static uint32_t found = FALSE;
static uint8_t command[50];
static uint32_t command_len = 0 ;
void ESPinit(void){

	// before useing this function init_usart1(); and  init_usart2(); must be executed

	//Reset ESP8266
	write_usart1((uint8_t*)AT_RST);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, NULL);
	}
	write_usart2((uint8_t*)"AT_RST PASSED\r\n");
	found = FALSE;

	//Connect to Wi-Fi, Join Access point
	memset(command, '\0', 50*sizeof(uint8_t));
	command_len += strlen("AT+CWJAP=") + strlen(SSID) + strlen(PWD) + 7; // 7 = "",""\r\n
	sprintf((char*)command, command_len ,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);


	write_usart2((uint8_t*)command); // test
	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
	}
	write_usart2((uint8_t*)"AT_CWJAP PASSED\r\n");
	found = FALSE;

	//Connect to API
	memset(command, '\0', 50*sizeof(uint8_t));
	command_len += strlen("AT+CIPSTART=") + strlen("TCP") + strlen("") + 7; // 7 = "",""\r\n
	sprintf((char*)command, command_len ,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);













}
void waitFor(uint32_t* response){


}
void addData(uint32_t* name, uint32_t* value){

}
void pushData(void){

}
void ping(void){

}


