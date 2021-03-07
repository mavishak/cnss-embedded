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


void ping(void);
void reset(void);
void setClientMode(void);
void joinAccessPoint(void);
void connectFirebaseHost(void);
void createPostMsg(void);
void sendRequest(void);
void readResponse(void);
void closeCunnection(void);

void delay(void);
void setImagePath(void);

void connectFirbase(void);
void recordAlert(void);

#endif /* ESP8266_FIREBASE_H_ */
