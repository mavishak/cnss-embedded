/*
 * system_control.c
 *
 *  Created on: 6 במאי 2021
 *      Author: mayan
 */

#include "system_control.h"
#include "timers.h"
#include "esp8266_Firebase.h"
#include "usart.h"
#include "event_queue.h"

void monitor_switch_state(uint32_t seconds){

	if(timeout_done_timer2()){
		add_event(control_Handler);
		set_timeout_timer2(seconds);
	}
}



