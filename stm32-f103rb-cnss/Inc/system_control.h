/*
 * system_control.h
 *
 *  Created on: 6 במאי 2021
 *      Author: mayan
 */

#ifndef SYSTEM_CONTROL_H_
#define SYSTEM_CONTROL_H_
#include "stm32f103xb.h"
#include "common.h"

BOOL SYSTEM_CONTROL_set_up(void);

void SYSTEM_CONTROL_monitor_switch_state(uint32_t seconds);

#endif /* SYSTEM_CONTROL_H_ */
