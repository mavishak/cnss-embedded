/*
 * gpio_signal.h
 *
 *  Created on: 7 בדצמ׳ 2020
 *      Author: mayan
 */

#ifndef GPIO_SIGNAL_H_
#define GPIO_SIGNAL_H_

#include "stm32f103xb.h"
#include "common.h"

void init();
void init_interrupt();
void init_interrupt_D6();
uint32_t was_pressed();
void toggle_ld2();
void *interrupt_handler(void);

//enum {OFF, ON} state;
SWITCH_STATE state;


#endif /* GPIO_SIGNAL_H_ */
