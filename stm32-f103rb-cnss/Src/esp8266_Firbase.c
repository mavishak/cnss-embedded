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
 *  15.02.2021: Problems:
 *  			1. Reset when downloading code to modul -> Not so bad... ()
 *  			2. AT + RESET stops program from continuing to run -> BAD - > why is this happening?
 *  			3. Not receiving > after AT+CIPSEND
 *
 *  04.03.2021: We succeed to send a POST message to Real-time database! (using AT commands, HTTPS and SSL)
 *  			1. We needed to write: AT+CIPSSLSIZE=4096
 *  			2. We needed to write: sprintf((char*)command, "AT+CIPSTART=\"SSL\",\"%s\",%ld\r\n",(char*)firebase_host, https_port)
 *  			3. The POST message was:
 *  			    POST /rest/test/posts.json? HTTP/1.1\r\n
 *					Host: %s\r\n
 *					Content-Type: application/json\r\n
 *					Content-Length: 49\r\n\r\n
 *					{"author": "Nemo Resh", "title": "Fish are blue"}
 *					\r\n
 */

#include "esp8266_Firebase.h"
#include "configurations.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#define FALSE 0
#define TRUE 1

#define COMMAND_SIZE 256
#define HTTP_SIZE 512
#define CONTENT_SIZE 128
#define PATH_SIZE 128

static uint32_t found = FALSE;
static uint8_t command[COMMAND_SIZE];

static uint8_t http[HTTP_SIZE];
static uint32_t http_len = 0 ;

static uint8_t content[CONTENT_SIZE]; //The HTTP body
static uint32_t content_len = 0 ;

static uint8_t image_path[PATH_SIZE];




void setImagePath(void){

	memset((char*)image_path, '\0', PATH_SIZE*sizeof(uint8_t));
	sprintf((char*)image_path, "image/path");
}


void connectFirbase(){

}


void recordAlert(void){
	// before useing this function init_usart1(); and  init_usart2(); must be executed

	//Reset ESP8266
	//reset(); //!TODO THIS FUNCTIONALITY NEEDS FIXING!!!


	//Set client mode
	setClientMode();
	write_usart2((uint8_t*)"1\r\n");


	//Join access point
	joinAccessPoint();
	write_usart2((uint8_t*)"2\r\n");

	/*Default: AT+CIPMUX=0 (according to: AT instruction set- 5.2.15)*/

	//Connect HOST IP
	connectFirebaseHost();
	write_usart2((uint8_t*)"3\r\n");


	//Set Image Path
	setImagePath(); //Need to check params later
	write_usart2((uint8_t*)"4\r\n");

	//Create HTTP request
	createPostMsg();
	write_usart2((uint8_t*)"5\r\n");


	//Send number of data bytes
	sendRequest();
	write_usart2((uint8_t*)"6\r\n");

	//Read response
	readResponse();
	write_usart2((uint8_t*)"7\r\n");

	//Close cunnection with firebase - this might be useless as firebase already closes connection with "CLOSED" response
	//closeCunnection();
	write_usart2((uint8_t*)"8\r\n");


}


//-------------------------------------------------------------------------------------------------------------


/*This function pings ESP8266 modem with AT test command,
 * returns uppon success.*/
void ping(void){

	found = FALSE;

	write_usart1((uint8_t*)AT_COMMAND);
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR); //returns true only if AT_OK is found
	}

}


/*This function resets ESP8266 modem with AT+RST command,
 * returns uppon success.
 * !TODO This functionality does not work properly - needs fixing with TIMEOUT or some other way.*/
void reset(void){

	found = FALSE;
	write_usart1((uint8_t*)AT_RST);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}

}


/*This function sets ESP8266 modem to client mode,
 * returns uppon success.*/
void setClientMode(void){

	found = FALSE;
	write_usart1((uint8_t*)AT_CWMODE);
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}
}


/*This function connects the WiFi modem ESP8266 to the given SSID in configurations.h,
 * returns upon success.*/
void joinAccessPoint(void){

	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);

	found = FALSE;
	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
	}

}


/*This function cunnects to firebase via secure HTTP (HTTPS) using SSL,
 * returns upon success.*/
void connectFirebaseHost(){


	//Create secure cunnection via SSL
	found = FALSE;
	write_usart1((uint8_t*)"AT+CIPSSLSIZE=4096\r\n");//at_instruction: 5.2.4 page 50
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}



	//Connect Firebase Host
	found = FALSE;
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSTART=\"SSL\",\"%s\",%ld\r\n",(char*)firebase_host, https_port);

	write_usart1((uint8_t*)command);

	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ALREADY_CONNECTED);
	}


}


/*This function builds HTTP request to send to firebase,
 * Returns when done*/
void createPostMsg(void){

	//Set HTTP body content
	memset((char*)content, '\0', CONTENT_SIZE*sizeof(uint8_t));
	sprintf((char*)content,"{\"image_path\": \"%s\", \"notes\": \"alarm went off!\", \"timestamp\": {\".sv\": \"timestamp\"}}",(char*)image_path);
	content_len = strlen((char*)content);
	//content_len = strlen("{\"image_path\": \"\", \"notes\": \"alarm went off!\", \"timestamp\": {\".sv\": \"timestamp\"}}") + strlen((char*)image_path);


	//Set HTTP request
	memset((char*)http, '\0', HTTP_SIZE*sizeof(uint8_t));
	sprintf((char*)http,"POST /devices/%s/history.json?auth=%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n%s\r\n",(char*)device_id,(char*)firebase_auth_key,(char*)firebase_host,content_len,(char*)content); // HTTP/1.0- Allow only one request
	//sprintf((char*)http,"POST /devices/%s/history.json?auth=%s&print=silent HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n{\"image_path\": \"%s\", \"notes\": \"alarm went off\", \"timestamp\": {\".sv\": \"timestamp\"}}\r\n",(char*)device_id,(char*)firebase_auth_key,(char*)firebase_host,content_len,(char*)image_path); // HTTP/1.0- Allow only one request
	http_len = strlen((char*)http)-strlen("\r\n"); // the last \r\n is for the AT command, and not included in the request's length

}


/*This function Sends request to firbase,
 * returns apun success.*/
void sendRequest(void){

	found = FALSE;

	/*Send Request Length - number of data bytes to be sent*/
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSEND=%ld\r\n",http_len);

	write_usart1((uint8_t*)command);
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)">", (uint8_t *)AT_ERROR);
	}
	found = FALSE;

	/*Send HTTP request*/
	write_usart1((uint8_t*)http);

	/*Wait for SEND_OK after this a response will come*/
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)SEND_OK, (uint8_t *)AT_FAIL);
	}

}


/*This function waits for response from firebase,
 * and returns once response is recieved.*/
void readResponse(void){

	found = FALSE;
	while(!found){
		//found = search_usart1_buffer_Rx((uint8_t *)"\r\n\r\nOK\r\n", (uint8_t *)AT_FAIL); //We counting on the appearance of OK in the HTTP response (we wont see the full response)
		found = search_usart1_buffer_Rx((uint8_t *)"CLOSED\r\n", (uint8_t *)AT_FAIL);
	}

}


/*This function closes connection*/
void closeCunnection(void){

	found = FALSE;
	write_usart1((uint8_t*)AT_CIPCLOSE);
	while(!found){
		found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
	}

}


/*delay for 1/4 of a second*/
void delay(void){
	uint32_t i = 10000000; // 1/4 of a second
	while(i-- > 0);
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


