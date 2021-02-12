/*
 * esp8266_WiFi.h
 *
 *  Created on: 23 Jan 2021
 *      Author: mayan and naomi
 */

#ifndef ESP8266_WIFI_H_
#define ESP8266_WIFI_H_

//Commands
#define AT_COMMAND "AT\r\n"
#define AT_RST "AT+RST\r\n"
#define AT_CWJAP "AT+CWJAP"
#define AT_CIPCLOSE "AT+CIPCLOSE\r\n"

//Responses
#define AT_OK "OK\r\n"
#define AT_ERROR "ERROR\r\n"
#define AT_FAIL "FAIL\r\n"
#define AT_ALREADY_CONNECTED "ALREADY CONNECTED\r\n"

//enum Commands{AT_COMMAND}
//enum Respondes{AT_OK, AT_ERROR}

#endif /* ESP8266_WIFI_H_ */
