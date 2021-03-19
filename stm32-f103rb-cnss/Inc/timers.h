/*
 * timers.h
 *
 *  Created on: 16 Mar 2021
 *      Author: Owner
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define TRUE 1
#define FALSE 0

//Timer 2
void init_timer2(void);
void enable_timer2(void);
void disable_timer2(void);
void delay_with_timer2(uint32_t num_of_millis);
void timeout_with_timer2(uint32_t num_of_millis);


//Timer 3
void init_timer3(void);
void enable_timer3(void);
void disable_timer3(void);
void delay_with_timer3(uint32_t num_of_millis);
void timeout_with_timer3(uint32_t num_of_millis);


//Timer 4
void init_timer4(void);
void enable_timer4(void);
void disable_timer4(void);
void delay_with_timer4(uint32_t num_of_millis);
void timeout_with_timer4(uint32_t num_of_millis);

#endif /* TIMERS_H_ */
