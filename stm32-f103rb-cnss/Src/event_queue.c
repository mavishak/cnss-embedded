/*
 * event_queue.c
 *
 *  Created on: Dec 1, 2020
 *  Author: Mayan Shaked and Naomi Resnik
 *  When head[Q] == tail[Q] queue is empty
 *  When head[Q] == tail[Q] + 1 % Q.size queue is full
 */

/* IMPORTANT NOTES AND QUESTIONS: This code (event_queue.c/h) has not bin tested.
 * 1. When queue is full how will the event stay reserved? should we keep trying to enter it until it succeeds?
 * 2. Will it be better to check if queue is full outside of the function in-order to save time and memory?
 * 3. When queue.access == STOP should we check it in the add_event function or out of it?
 * 4. What happens if the handler has an argument? When could that happen?
 * 5. What is the best way to test and debug this event_queue?
 * 6. Check is header file contains everything it needs...
 * 7. Generally speaking what is the projects structure in terms of files (events, handlers etc.) ?
 * 8. How to DEBUG correctly???
 * 9. When dealing with global variables How to lock them in case of interrupt preemption?
 * 	  Answer: We will not allow preemption
 * */

#include "event_queue.h"
#include "usart.h"


static Queue queue;


void init_queue()
{
	//write_usart2((uint8_t*)("<-init_queue->\r\n"));
	queue.writeIndex = 0;
	queue.readIndex = 0;
	queue.accept = GO;
}


void add_event(Handler handler)
{

	if(queue_isFull()){
		//write_usart2((uint8_t*)("\r\n<-queue is full->\r\n"));
		return;
	}

	queue.eq[queue.writeIndex].handler = handler;

	if(queue.writeIndex < (QUEUE_SIZE-1)) //changed from  < QUEUE_SIZE
		queue.writeIndex++;
	else
		queue.writeIndex = 0;

	//write_usart2((uint8_t*)("\r\n<-queue add event->\r\n"));

}



void *do_event()
{
	void *res;

	if(queue_isEmpty()){
		//write_usart2((uint8_t*)("\r\n<-queue is empty->\r\n"));
		return NULL;
	}

   res = queue.eq[queue.readIndex].handler();
   queue.eq[queue.readIndex].handler = NULL; // cleanup


   if(queue.readIndex < (QUEUE_SIZE - 1))//changed from < QUEUE_SIZE
      queue.readIndex++;
   else
      queue.readIndex = 0;

   //write_usart2((uint8_t*)("\r\n<-queue do event->\r\n"));

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

