/*
 * event_queue.c
 *
 *  Created on: Dec 1, 2020
 *  Author: Mayan Shaked and Naomi Resnik
 *  When head[Q] == tail[Q] queue is empty
 *  When head[Q] == tail[Q] + 1 % Q.size queue is full
 */


#include "event_queue.h"
#include "usart.h"


static Queue queue;


void QUEUE_init()
{
	//write_usart2((uint8_t*)("<-QUEUE_init->\r\n"));
	queue.writeIndex = 0; // tail, holds next index to write in
	queue.readIndex = 0; // head, holds next index to read from
	queue.accept = GO;
}


void QUEUE_add_event(Handler handler)
{

	if(queue_isFull()){
		return;
	}

	queue.eq[queue.writeIndex].handler = handler;

	if(queue.writeIndex < (QUEUE_SIZE - 1))
		queue.writeIndex++;
	else
		queue.writeIndex = 0;

}



void *QUEUE_do_event()
{
	void *res;

	if(queue_isEmpty()){
		return NULL;
	}

   res = queue.eq[queue.readIndex].handler();
   queue.eq[queue.readIndex].handler = NULL; // cleanup


   if(queue.readIndex < (QUEUE_SIZE - 1))
      queue.readIndex++;
   else
      queue.readIndex = 0;

   return res;

}


BOOL queue_isFull()
{
	return (queue.readIndex == ((queue.writeIndex + 1) % QUEUE_SIZE));
}


BOOL queue_isEmpty()
{
	 return queue.readIndex == queue.writeIndex;
}

