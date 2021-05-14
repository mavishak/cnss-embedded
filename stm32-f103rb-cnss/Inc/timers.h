/*
 * timers.h
 *
 *  Created on: 16 Mar 2021
 *      Author: Owner
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "common.h"


//Timer 2
void init_timer2(void);
void enable_timer2(void);
void disable_timer2(void);
void delay_with_timer2(uint32_t num_of_sec); //polling
BOOL timeout_with_timer2(uint32_t num_of_sec); // for use in while loops
void set_timeout_timer2(uint32_t num_of_sec); // for use in if statements
BOOL timeout_done_timer2(void); // for use in if statements


//Timer 3
void init_timer3(void);
void enable_timer3(void);
void disable_timer3(void);
void delay_with_timer3(uint32_t num_of_sec);
BOOL timeout_with_timer3(uint32_t num_of_sec);
void set_timeout_timer3(uint32_t num_of_sec);
BOOL timeout_done_timer3(void);

//Timer 4
void init_timer4(void);
void enable_timer4(void);
void disable_timer4(void);
void delay_with_timer4(uint32_t num_of_sec);
BOOL timeout_with_timer4(uint32_t num_of_sec);
void set_timeout_timer4(uint32_t num_of_sec);
BOOL timeout_done_timer4(void);

typedef struct timer{

	uint32_t countTicks;
	uint32_t targetTick;

}TIMER;

#endif /* TIMERS_H_ */
