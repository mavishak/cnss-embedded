/*
 * hc-sr501pir_sensor.h
 *
 *  Created on: 21 December  2020
 *      Author: Mayan and Naomi
 */

#ifndef HC_SR501PIR_SENSOR_H_
#define HC_SR501PIR_SENSOR_H_
#include "common.h"

void SENSOR_init();
void init_sensor_led_response();
void enable_sensor(void);
void disable_sensor(void);
void toggle_led();

//enum {OFF, ON} state;
SWITCH_STATE s;

#endif /* HC_SR501PIR_SENSOR_H_ */
