/*
 * system_control.c
 *
 *  Created on: 6 05 2021
 *      Author: mayan
 */

#include "system_control.h"
#include "timers.h"
#include "esp8266_Firebase.h"
#include "usart.h"
#include "event_queue.h"

void monitor_switch_state(uint32_t seconds){

	if(TIMER2_timeout_done()){
		add_event(control_Handler);
		TIMER2_set_timeout(seconds);
	}
}



