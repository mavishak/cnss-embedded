/*
 * timers.h
 *
 *  Created on: 16 Mar 2021
 *      Author: Owner
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "common.h"
#include "event_queue.h"
#include "esp8266_Firebase.h"

#define MAX_COUNT 0x7FFFFFFF // largest posotive uint32

//Timer 2

/*init's timer to  interrupt once a second when enabled*/
void TIMER2_init(void);
void TIMER2_enable(void);
void TIMER2_disable(void);

/*Sets a timer delay to num_of_secs, blocks until done.
 *For polling */
void TIMER2_delay(uint32_t num_of_sec);

/*This function sets a target as num_of_sec, and enables timer.
 * For use in if statements*/
void TIMER2_set_timeout(uint32_t num_of_sec);

/*returns true if timer reached set_timouts target, otherwise returns false
 *  For use in if statements*/
BOOL TIMER2_timeout_done(void);

//Timer 3
void TIMER3_init(void);
void TIMER3_enable(void);
void TIMER3_disable(void);
void TIMER3_delay(uint32_t num_of_sec);
void TIMER3_set_timeout(uint32_t num_of_sec);
BOOL TIMER3_timeout_done(void);

//Timer 4
void TIMER4_init(void);
void TIMER4_enable(void);
void TIMER4_disable(void);
void TIMER4_delay(uint32_t num_of_sec);
void TIMER4_set_timeout(uint32_t num_of_sec);
BOOL TIMER4_timeout_done(void);

typedef struct timer {

	uint32_t countTicks;
	uint32_t targetTick;

} TIMER;

#endif /* TIMERS_H_ */
