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

//void ESPinit(void);
void TestWifiConnection(void);
void connectFirbase(void);
void waitFor(uint32_t* response);
void addData(uint32_t* name, uint32_t* value);
void pushData(void);
void ping(void);
void delay(void);
void timestamp(void);


#endif /* ESP8266_FIREBASE_H_ */
