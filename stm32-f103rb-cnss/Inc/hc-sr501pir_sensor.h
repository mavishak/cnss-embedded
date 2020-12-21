/*
 * hc-sr501pir_sensor.h
 *
 *  Created on: 21 December  2020
 *      Author: Mayan and Naomi
 */

#ifndef HC_SR501PIR_SENSOR_H_
#define HC_SR501PIR_SENSOR_H_

void init_sensor_with_interrupt();
void init_sensor_led_response();
void toggle_led();

enum {OFF, ON} state;

#endif /* HC_SR501PIR_SENSOR_H_ */
