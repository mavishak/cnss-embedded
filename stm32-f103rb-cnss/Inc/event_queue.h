/*
 * event_queue.h
 *
 *  Created on: Dec 1, 2020
 *  Author: Mayan Shaked and Naomi Resnik
 *
 *  This file declares the functionality associated with the implementation of the event queue.
 */

#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include "stm32f103xb.h" //this file is included to every header file
#include <stdlib.h>
#include "common.h"
#define QUEUE_SIZE 10//64 //128

enum mode { GO, STOP };

//typedef void (*Handler)(); //typedef for function that receives void and returns void
typedef void *(*Handler)(); // typedef for function that receives void and returns void*


typedef struct event{
   Handler handler;
} Event;


typedef struct queue{

	Event eq[QUEUE_SIZE];// declare queue
	uint32_t writeIndex; //The index to write a new event to from an ISR
	uint32_t readIndex; //Index to read event from and run handler
	enum mode accept; //This variable indicates when we should stop accepting because system is to be shut down.

} Queue;

/*Initialize shared queue*/
void init_queue();

/*This function receives and event and adds it to queue*/
void add_event(Handler handler);

/*This function retrieves an event from queue and runs its handle.
 * If queue is empty return NULL. Upon success returns handler res */
void *do_event();

/*this function return true if queue is full*/
BOOL queue_isFull();

/*This function returns true if queue is empty*/
BOOL queue_isEmpty();

#endif /* EVENT_QUEUE_H_ */
