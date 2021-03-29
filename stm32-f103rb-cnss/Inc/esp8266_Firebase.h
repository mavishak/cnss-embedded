/*
 * esp8266_Firebase.h
 *
 *  Created on: 9/02/2021
 *      Author: mayan and naomi
 */

#ifndef ESP8266_FIREBASE_H_
#define ESP8266_FIREBASE_H_

#include "esp8266_WiFi.h"
#include "stm32f103xb.h" //this file is included to every header file
#include "common.h"



//void ESPinit(void);
void TestWifiConnection(void);


BOOL ping(uint32_t tries, uint32_t timeout);
BOOL reset(uint32_t tries, uint32_t timeout);
BOOL setClientMode(uint32_t tries, uint32_t timeout);
BOOL joinAccessPoint(uint32_t tries, uint32_t timeout);
BOOL connectFirebaseHost(uint32_t _ssl_tries, uint32_t _cipstart_tries , uint32_t _ssl_timeout, uint32_t  _cipstart_timeout);
void createPostMsg(void);
BOOL sendRequest(uint32_t _CIPSEND_tries,uint32_t _SEND_OK_tries , uint32_t _CIPSEND_timeout, uint32_t _SEND_OK_timeout);
BOOL readResponse(uint32_t timeout);
BOOL closeCunnection(uint32_t tries, uint32_t timeout);

void setImagePath(void);

void connectFirbase(void);
BOOL recordAlert(void);
void *alert_Handler(void);

#endif /* ESP8266_FIREBASE_H_ */
