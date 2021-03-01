/*
 * esp8266_Firbase.c
 *
 *  Created on: 09/02/2021
 *      Author: mayan and naomi
 *
 *  12.02.2021: AT_RST works after that stuck in while loop of AT+CWJAP
 *  			!TODO: Need to debug
 *  			Note: DO NOT PUT NULL AS FAIL DEFAULT
 *
 *  15.02.2021: Problems: YIII HIIII
 *  			1. Reset when downloading code to modul -> Not so bad... ()
 *  			2. AT + RESET stops program from continuing to run -> BAD - > why is this happening?
 *  			3. Not receiving > after AT+CIPSEND
 */

#include "esp8266_Firebase.h"
#include "configurations.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
//#include <time.h>

#define FALSE 0
#define TRUE 1

#define COMMAND_SIZE 256
#define HTTP_SIZE 256

static uint32_t found = FALSE;
static uint8_t command[COMMAND_SIZE];

static uint8_t http[HTTP_SIZE];
static uint32_t http_len = 0 ;


void connectFirbase(void){
	// before useing this function init_usart1(); and  init_usart2(); must be executed

	//Reset ESP8266
	//	write_usart1((uint8_t*)AT_RST);
	//
	//	while(!found){
	//		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	//	}
	//	write_usart2((uint8_t*)"AT_RST PASSED\r\n");
	//	found = FALSE;

	//Set client mode
	write_usart1((uint8_t*)AT_CWMODE);
	write_usart2((uint8_t*)"BEFORE AT_CWMODE WHILE\r\n");
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}
	write_usart2((uint8_t*)"AT_CWMODE PASSED\r\n");
	found = FALSE;


	//Join access point
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);

	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
	}
	found = FALSE;


	/*Default: AT+CIPMUX=0 (according to: AT instruction set- 5.2.15)*/

	//Connect to API
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSTART=\"TCP\",\"%s\",%ld\r\n",(char*)firebase_host, https_port);

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
	sprintf((char*)http,("GET /data/2.5/weather?q=London,uk&appid=%s HTTP/1.0\r\nHost: %s\r\n\r\n\r\n"),(char*)firebase_auth_key,(char*)firebase_host); // HTTP/1.0- Allow only one request
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

	// SEND OK
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
	}
	found = FALSE;

	// READ RESPONSE
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)"\r\n\r\nOK\r\n", (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
		found = search_usart1_buffer_Rx((uint8_t *)"CLOSED\r\n", (uint8_t *)AT_FAIL);
	}
	found = FALSE;

	//Close TCP connection: THIS MIGHT BE UNESESARY BECAUSE HTTP is vs 1.0 wich allows one request
	write_usart1((uint8_t*)AT_CIPCLOSE);
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}
	found = FALSE;

}


void TestWifiConnection(void){

	// before useing this function init_usart1(); and  init_usart2(); must be executed


	//Reset ESP8266
//	write_usart1((uint8_t*)AT_RST);
//
//	while(!found){
//		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
//	}
//	write_usart2((uint8_t*)"AT_RST PASSED\r\n");
//	found = FALSE;


	//Set client mode
	write_usart1((uint8_t*)AT_CWMODE);
	write_usart2((uint8_t*)"BEFORE AT_CWMODE WHILE\r\n");
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}
	write_usart2((uint8_t*)"AT_CWMODE PASSED\r\n");
	found = FALSE;


	//Join access point
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);

	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
	}
	found = FALSE;



	/*Default: AT+CIPMUX=0 (according to: AT instruction set- 5.2.15)*/

	//Connect to API
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSTART=\"TCP\",\"%s\",80\r\n", (char*)weather_api);

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
	sprintf((char*)http,("GET /data/2.5/weather?q=London,uk&appid=%s HTTP/1.0\r\nHost: %s\r\n\r\n\r\n"),weather_api_key,weather_api); // HTTP/1.0- Allow only one request
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

	// SEND OK
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
	}
	found = FALSE;

	// READ RESPONSE
	while(!found){
		//found = search_usart1_buffer_Rx((uint8_t *)"\r\n\r\nOK\r\n", (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
		found = search_usart1_buffer_Rx((uint8_t *)"CLOSED\r\n", (uint8_t *)AT_FAIL);
	}
	found = FALSE;

	//Close TCP connection: THIS MIGHT BE UNESESARY BECAUSE HTTP is vs 1.0 wich allows one request
	write_usart1((uint8_t*)AT_CIPCLOSE);
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}
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

/*delay for 1/4 of a second*/
void delay(void){
	uint32_t i = 10000000; // 1/4 of a second
	while(i-- > 0);
}


