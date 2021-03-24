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
 *
 * 21.03.2021: See __LINE__ 285:
 * 			   found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ALREADY_CONNECTED);
 * 			   //NO IDEA WHY THIS IS SO CRITICAL BUT IT IS!
 */

#include "esp8266_Firebase.h"
#include "configurations.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "timers.h"


//#define FALSE 0
//#define TRUE 1

#define COMMAND_SIZE 256
#define HTTP_SIZE 512
#define CONTENT_SIZE 128
#define PATH_SIZE 128

static STATE found = STANDBY;
static uint8_t command[COMMAND_SIZE];

static uint8_t http[HTTP_SIZE];
static uint32_t http_len = 0 ;

static uint8_t content[CONTENT_SIZE]; //The HTTP body
static uint32_t content_len = 0 ;

static uint8_t image_path[PATH_SIZE];

//static uint32_t tries = 0;
//static uint32_t timeout = 0;



void setImagePath(void){

	memset((char*)image_path, '\0', PATH_SIZE*sizeof(uint8_t));
	sprintf((char*)image_path, "image/path");
}


/*This function sends an alert to realtime DB in containig the time of the alert firebase
 * before useing this function
 * init_usart1(), init_usart2() and init_timer4() must be executed.*/
BOOL recordAlert(void){


	//Reset ESP8266
//	if(!reset(3,1)){ //!TODO THIS FUNCTIONALITY NEEDS FIXING!!!
//		return FALSE;
//	}
//	delay_with_timer4(1);

	//write_usart2((uint8_t*)"0\r\n"); //with this it reaches AT+CWJAP


	//Set client mode
	if(!setClientMode(3,6)){
		return FALSE;
	}
	write_usart2((uint8_t*)"1\r\n");

	//Join access point
	if(!joinAccessPoint(3,6)){
		return FALSE;
	}
	write_usart2((uint8_t*)"2\r\n");

	/*Default: AT+CIPMUX=0 (according to: AT instruction set- 5.2.15)*/

	//Connect HOST IP
	if(!connectFirebaseHost(3,3,3,10)){
		return FALSE;
	}
	write_usart2((uint8_t*)"3\r\n");


	//Set Image Path
	setImagePath(); //Need to check params later
	write_usart2((uint8_t*)"4\r\n");

	//Create HTTP request
	createPostMsg();
	write_usart2((uint8_t*)"5\r\n");


	//Send number of data bytes
	if(!sendRequest(3,3,30,60)){
		closeCunnection(3,3);
		return FALSE;
	}
	write_usart2((uint8_t*)"6\r\n");

	//Read response
	if(!readResponse(180)){//timeout set t0 3 minutes
		closeCunnection(3,3);
		return FALSE;
	}

	write_usart2((uint8_t*)"7\r\n");

	//Close cunnection with firebase - this might be useless as firebase already closes connection with "CLOSED" response
	//closeCunnection(3,3);
	//write_usart2((uint8_t*)"8\r\n");

	return TRUE;

}


//-------------------------------------------------------------------------------------------------------------


/*This function pings ESP8266 modem with AT test command,
 * returns uppon success
 * tries: number of times to send ping incase of timeout or failure.
 * timeout (in seconds): number of seconds to wait for response.*/
BOOL ping(uint32_t tries, uint32_t timeout){

	found = STANDBY;
	write_usart1((uint8_t*)AT_COMMAND);
	while(tries > 0){
		while(found == STANDBY && !timeout_with_timer4(timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR); //returns true only if AT_OK is found
		}
		if(found == PASS){
			return TRUE;
		}
		else{ // FAIL OR TIMEOUT
			tries--;
			write_usart1((uint8_t*)AT_COMMAND);
		}
	}
	return FALSE;

}


/*This function resets ESP8266 modem with AT+RST command,
 * returns uppon success.
 * tries: number of times to send ping incase of timeout or failure.
 * timeout (in seconds): number of seconds to wait for response.
 * !TODO This functionality does not work properly - needs fixing.*/
BOOL reset(uint32_t tries, uint32_t timeout){

	found = STANDBY;
	write_usart1((uint8_t*)AT_RST);
	while(tries > 0){
		while(found == STANDBY && !timeout_with_timer4(timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
		}
		if(found == PASS){
			return TRUE;
		}
		else{ // FAIL OR TIMEOUT
			tries--;
			write_usart1((uint8_t*)AT_RST);
		}
	}
	return FALSE;

}


/*This function sets ESP8266 modem to client mode,
 * returns uppon success.
 * tries: number of times to send ping incase of timeout or failure.
 * timeout (in seconds): number of seconds to wait for response.*/
BOOL setClientMode(uint32_t tries, uint32_t timeout){

	found = STANDBY;
	write_usart1((uint8_t*)AT_CWMODE);
	while(tries > 0){
		while(found == STANDBY && !timeout_with_timer4(timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
		}
		if(found == PASS){
			return TRUE;
		}
		else{ // FAIL OR TIMEOUT
			tries--;
			write_usart1((uint8_t*)AT_CWMODE);
		}
	}
	return FALSE;
}


/*This function connects the WiFi modem ESP8266 to the given SSID in configurations.h,
 * returns upon success.
 * tries: number of times to send ping incase of timeout or failure.
 * timeout (in seconds): number of seconds to wait for response.*/
BOOL joinAccessPoint(uint32_t tries, uint32_t timeout){

	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PWD);

	found = STANDBY;
	write_usart1((uint8_t*)command);
	while(tries > 0){
		while(found == STANDBY && !timeout_with_timer4(timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_FAIL);
		}
		if(found == PASS){
			return TRUE;
		}
		else{ // FAIL OR TIMEOUT
			tries--;
			write_usart1((uint8_t*)command);
		}
	}
	return FALSE;

}


/*This function cunnects to firebase via secure HTTP (HTTPS) using SSL,
 * returns upon success.
 * tries: number of times to send ping incase of timeout or failure.
 * timeout (in seconds): number of seconds to wait for response
 * need to enter tries and timout for both SSL AT_command and CIPSTART AT_command*/
BOOL connectFirebaseHost(uint32_t _ssl_tries, uint32_t _cipstart_tries , uint32_t _ssl_timeout, uint32_t  _cipstart_timeout){


	//Create secure cunnection via SSL
	found = STANDBY;
	write_usart1((uint8_t*)"AT+CIPSSLSIZE=4096\r\n");//at_instruction: 5.2.4 page 50
	while(_ssl_tries > 0){
		while(found == STANDBY && !timeout_with_timer4(_ssl_timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
		}
		if(found == PASS){
			break; //move on to next command
		}
		else{ // FAIL OR TIMEOUT
			_ssl_tries--;
			write_usart1((uint8_t*)"AT+CIPSSLSIZE=4096\r\n");
		}
	}

	if(found == FAIL || found == STANDBY){
		return FALSE;
	}



	//Connect Firebase Host
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSTART=\"SSL\",\"%s\",%ld\r\n",(char*)firebase_host, https_port);

	found = STANDBY;
	write_usart1((uint8_t*)command);
	while(_cipstart_tries > 0){
		while(found == STANDBY && !timeout_with_timer4(_cipstart_timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
			if(found == STANDBY){
				found = search_usart1_buffer_Rx((uint8_t *)AT_ALREADY_CONNECTED, (uint8_t *)AT_ERROR);//NO IDEA WHY THIS IS SO CRITICAL BUT IT IS!
			}
		}
		if(found == PASS){
			return TRUE;
		}
		else{ // FAIL OR TIMEOUT
			_cipstart_tries--;
			write_usart1((uint8_t*)command);
		}
	}
	return FALSE;



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
	sprintf((char*)http,"POST /devices/%s/history.json?auth=%s HTTP/1.0\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n%s\r\n",(char*)device_id,(char*)firebase_auth_key,(char*)firebase_host,content_len,(char*)content); // HTTP/1.0- Allow only one request
	//sprintf((char*)http,"POST /devices/%s/history.json?auth=%s&print=silent HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n{\"image_path\": \"%s\", \"notes\": \"alarm went off\", \"timestamp\": {\".sv\": \"timestamp\"}}\r\n",(char*)device_id,(char*)firebase_auth_key,(char*)firebase_host,content_len,(char*)image_path); // HTTP/1.0- Allow only one request
	http_len = strlen((char*)http)-strlen("\r\n"); // the last \r\n is for the AT command, and not included in the request's length

}


/*This function Sends request to firbase,
 * returns apun success.
 * tries: number of times to send ping incase of timeout or failure.
 * timeout (in seconds): number of seconds to wait for response
 */
BOOL sendRequest(uint32_t _CIPSEND_tries,uint32_t _SEND_OK_tries , uint32_t _CIPSEND_timeout, uint32_t _SEND_OK_timeout ){


	/*Send Request Length - number of data bytes to be sent*/
	memset((char*)command, '\0', COMMAND_SIZE*sizeof(uint8_t));
	sprintf((char*)command, "AT+CIPSEND=%ld\r\n",http_len);

	found = STANDBY;
	write_usart1((uint8_t*)command);
	while(_CIPSEND_tries > 0){
		while(found == STANDBY && !timeout_with_timer4(_CIPSEND_timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)">", (uint8_t *)AT_ERROR);
			if(found == STANDBY){
				found = search_usart1_buffer_Rx((uint8_t *)">", (uint8_t *)"CLOSED\r\n");
			}
		}
		if(found == PASS){
			break; //move on to next command
		}
		else{ // FAIL OR TIMEOUT
			_CIPSEND_tries--;
			write_usart1((uint8_t*)command);
		}
	}
	if(found == FAIL || found == STANDBY){
		return FALSE;
	}


	/*Send HTTP request*/
	found = STANDBY;
	write_usart1((uint8_t*)http);

	/*Wait for SEND_OK after this a response will come*/
	while(_SEND_OK_tries > 0){
		while(found == STANDBY && !timeout_with_timer4(_SEND_OK_timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)SEND_OK, (uint8_t *)AT_FAIL);
		}
		if(found == PASS){
			return TRUE;
		}
		else{
			_SEND_OK_tries--;
			write_usart1((uint8_t*)http);
		}
	}
	return FALSE;

}


/*This function waits for response from firebase,
 * and returns once response is recieved.
 * timeout (in seconds): number of seconds to wait for response*/
BOOL readResponse(uint32_t timeout){

	found = STANDBY;
	while(found == STANDBY && !timeout_with_timer4(timeout)){
		found = search_usart1_buffer_Rx((uint8_t *)"CLOSED\r\n", (uint8_t *)AT_FAIL);
	}
	if(found == PASS){
		return TRUE;
	}
	else{
		return FALSE;
	}

}


/*This function closes connection
 * tries: number of times to send ping incase of timeout or failure.
 * timeout (in seconds): number of seconds to wait for response*/
BOOL closeCunnection(uint32_t tries, uint32_t timeout){

	found = STANDBY;
	write_usart1((uint8_t*)AT_CIPCLOSE);
	while(tries > 0){
		while(found == STANDBY && !timeout_with_timer4(timeout)){
			found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
		}
		if(found == PASS){
			return TRUE;
		}
		else{ // FAIL OR TIMEOUT
			tries--;
			write_usart1((uint8_t*)AT_CIPCLOSE);
		}
	}
	return FALSE;

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


