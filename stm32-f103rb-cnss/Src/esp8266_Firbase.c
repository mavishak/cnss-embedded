/*
 * esp8266_Firbase.c
 *
 *  Created on: 09/02/2021
 *      Author: mayan and naomi
 */

#include "esp8266_Firebase.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

#define FALSE 0
#define TRUE 1

#define COMMAND_SIZE 128
#define HTTP_SIZE 256

static uint32_t found = FALSE;
static uint8_t command[COMMAND_SIZE];

static uint8_t http[HTTP_SIZE];
static uint32_t http_len = 0 ;

//see: https://openweathermap.org/current
static uint8_t api[] = "api.openweathermap.org" ;
static uint8_t api_key[] = "4e622e19669401cd85da98eb2c3852f5" ;

void ESPinit(void){

	// before useing this function init_usart1(); and  init_usart2(); must be executed

	//Reset ESP8266
	write_usart1((uint8_t*)AT_RST);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, NULL);
	}
	write_usart2((uint8_t*)"AT_RST PASSED\r\n");
	found = FALSE;

	//Join access point
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	//command_len += strlen("AT+CIPSTART=") + strlen("TCP") + strlen("") + 7; // 7 = "",""\r\n
	//sprintf((char*)command, command_len ,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);
	sprintf((char*)command, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);

	write_usart2((uint8_t*)command); // test
	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
	}
	write_usart2((uint8_t*)"AT_CWJAP PASSED\r\n");
	found = FALSE;


	/*Default: AT+CIPMUX=0 (according to: AT instruction set- 5.2.15)*/

	//Connect to API
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSTART=\"TCP\",\"%s\",80\r\n",api);

	//write_usart2((uint8_t*)command); // test
	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ALREADY_CONNECTED);
	}
	//write_usart2((uint8_t*)"AT+CIPSTART PASSED\r\n");
	found = FALSE;


	//Set HTTP request
	memset((char*)http, '\0', HTTP_SIZE*sizeof(uint8_t));
	sprintf((char*)http,("GET /data/2.5/weather?q=London,uk&appid=%s HTTP/1.0\r\nHost: %s\r\n\r\n\r\n"),api_key,api); // HTTP/1.0- Allow only one request
	http_len = strlen((char*)http)-strlen("\r\n"); // the last \r\n is for the AT command, and not included in the request's length

	//Send number of data bytes
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSEND=%ld\r\n",http_len);


	//write_usart2((uint8_t*)command); // test
	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)">", (uint8_t *)AT_ERROR);
	}
	found = FALSE;

	write_usart1((uint8_t*)http);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
	}
	found = FALSE;

	//Close TCP connection
	write_usart1((uint8_t*)AT_CIPCLOSE);
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, NULL);
	}
	write_usart2((uint8_t*)"AT_CIPCLOSE PASSED\r\n");
	found = FALSE;


}
void waitFor(uint32_t* response){


}
void addData(uint32_t* name, uint32_t* value){

}
void pushData(void){

}
void ping(void){

}


