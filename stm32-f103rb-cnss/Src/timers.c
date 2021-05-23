/*
 * timer.c
 *
 *  Created on: 16 Mar 2021
 *      Author: Mayan and Naomi
 */
#include "stm32f103xb.h"
#include "timers.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>



static TIMER timer2;
static TIMER timer3;
static TIMER timer4;


/*init's timer2 to  interrupt once a second when enabled*/
void init_timer2(void){

	/* APB1 peripheral clock enable register (RCC_APB1ENR) {p.148 in the reference manual} */
	RCC->APB1ENR |= 0x00000001; // TIM2EN: TIM2 timer clock enable

	//	/* ARR*PSC=8000 -> 1ms */
	//
	//	/* TIM2 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	//	TIM2->PSC = 0x0064; //0x0064[Hex] = 100[dec]
	//
	//	/* TIM2 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	//	TIM2->ARR = 0x0050; //0x0050[Hex] = 80[dec]
	//
	//	TIM2->CR1 |= 0x0004; // URS- generate an interrupt only in over/under flow

	/* ARR*PSC=8,000,000 -> 1s */

	/* TIM2 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	TIM2->PSC = 0x07D0; //0x07D0[Hex] = 2000[dec]

	/* TIM2 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	TIM2->ARR = 0x0FA0; //0x0FA0[Hex] = 4000[dec]

	TIM2->CR1 |= 0x0004; // URS- generate an interrupt only in over/under flow

	__disable_irq();
	TIM2->DIER |= 0x0001; // Update Interrupt Enable
	TIM2->EGR |= 0x0001; // Update Genaration
	NVIC_SetPriorityGrouping(7); //This should disable interrupt nesting(priority wont be not allowed)//->MABY IT'S THE DEFAULT
	NVIC_SetPriority(TIM2_IRQn,0); //set all interrupt priority to zero so that no preemption occurs.//->MABY IT'S THE DEFAULT
	NVIC_EnableIRQ(TIM2_IRQn); //enable handler
	__enable_irq();

	/* TIMx control register 1 (TIMx_CR1) {p.404 in the reference manual} */
	//TIM2->CR1 |= 0x0010; // DIR- counter direction (By default: counting up)
	//TIM2->CR1 |= 0x0001; // CEN: Counter enable (we enabled it in the delay- in order not to trigger an interrupt)

	/*Initialize timer*/
	timer2.countTicks = 0;
	timer2.targetTick = 0;

}

/*init's timer3 to  interrupt once a milli second when enabled*/
void init_timer3(void){
	/* APB1 peripheral clock enable register (RCC_APB1ENR) {p.148 in the reference manual} */
	RCC->APB1ENR |= 0x00000002; // TIM3EN: TIM3 timer clock enable

	/* ARR*PSC=8000 -> 1ms */

//	/* TIM3 Set Prescaler {see: 15.4.11 in the Reference manual}*/
//	TIM3->PSC = 0x0064; //0x0064[Hex] = 100[dec]
//
//	/* TIM3 Set TIMx auto-reload register {p.419 in the Reference manual}*/
//	TIM3->ARR = 0x0050; //0x0050[Hex] = 80[dec]
//
//	TIM3->CR1 |= 0x0004; // URS- generate an interrupt only in over/under flow

	/* ARR*PSC=8,000,000 -> 1s */

	/* TIM3 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	TIM3->PSC = 0x07D0; //0x07D0[Hex] = 2000[dec]

	/* TIM3 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	TIM3->ARR = 0x0FA0; //0x0FA0[Hex] = 4000[dec]

	TIM3->CR1 |= 0x0004; // URS- generate an interrupt only in over/under flow

	__disable_irq();
	TIM3->DIER |= 0x0001; // Update Interrupt Enable
	TIM3->EGR |= 0x0001; // Update Genaration
	NVIC_SetPriorityGrouping(7); //This should disable interrupt nesting(priority wont be not allowed)//->MABY IT'S THE DEFAULT
	NVIC_SetPriority(TIM3_IRQn,0); //set all interrupt priority to zero so that no preemption occurs.//->MABY IT'S THE DEFAULT
	NVIC_EnableIRQ(TIM3_IRQn); //enable handler
	__enable_irq();

	/* TIMx control register 1 (TIMx_CR1) {p.404 in the reference manual} */
	//TIM3->CR1 |= 0x0010; // DIR- counter direction (By default: counting up)
	//TIM3->CR1 |= 0x0001; // CEN: Counter enable (we enabled it in the delay- in order not to trigger an interrupt)

	/*Initialize timer*/
	timer3.countTicks = 0;
	timer3.targetTick = 0;

}

/*init's timer4 to  interrupt once a second when enabled*/
void init_timer4(void)
{
	/* APB1 peripheral clock enable register (RCC_APB1ENR) {p.148 in the reference manual} */
	RCC->APB1ENR |= 0x00000004; // TIM4EN: TIM4 timer clock enable

	/* ARR*PSC=8,000,000 -> 1s */

	/* TIM4 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	TIM4->PSC = 0x07D0; //0x07D0[Hex] = 2000[dec]

	/* TIM4 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	TIM4->ARR = 0x0FA0; //0x0FA0[Hex] = 4000[dec]

	TIM4->CR1 |= 0x0004; // URS- generate an interrupt only in over/under flow

	__disable_irq();
	TIM4->DIER |= 0x0001; // Update Interrupt Enable
	TIM4->EGR |= 0x0001; // Update Genaration
	NVIC_SetPriorityGrouping(7); //This should disable interrupt nesting(priority wont be not allowed)//->MABY IT'S THE DEFAULT
	NVIC_SetPriority(TIM4_IRQn,0); //set all interrupt priority to zero so that no preemption occurs.//->MABY IT'S THE DEFAULT
	NVIC_EnableIRQ(TIM4_IRQn); //enable handler
	__enable_irq();

	/* TIMx control register 1 (TIMx_CR1) {p.404 in the reference manual} */
	//TIM4->CR1 |= 0x0010; // DIR- counter direction (By default: counting up)
	//TIM4->CR1 |= 0x0001; // CEN: Counter enable (we enabled it in the delay- in order not to trigger an interrupt)

	/*Initialize timer*/
	timer4.countTicks = 0;
	timer4.targetTick = 0;

}


void enable_timer2(void)
{
	TIM2->CR1 |= 0x0001; // CEN: Counter enable
}

void enable_timer3(void)
{
	TIM3->CR1 |= 0x0001; // CEN: Counter enable
}

void enable_timer4(void)
{
	TIM4->CR1 |= 0x0001; // CEN: Counter enable
}


void disable_timer2(void)
{
	TIM2->CR1 &= ~(0x0001); // CEN: Counter disable
}

void disable_timer3(void)
{
	TIM3->CR1 &= ~(0x0001); // CEN: Counter disable
}

void disable_timer4(void)
{
	TIM4->CR1 &= ~(0x0001); // CEN: Counter disable

}


void delay_with_timer2(uint32_t num_of_sec)
{
	enable_timer2();

	timer2.countTicks = 0;
	while(timer2.countTicks < num_of_sec);

	disable_timer2();

}

void delay_with_timer3(uint32_t num_of_sec)
{
	enable_timer3();

	timer3.countTicks = 0;
	while(timer3.countTicks < num_of_sec);
	disable_timer3();

}

void delay_with_timer4(uint32_t num_of_sec)
{
	enable_timer4();

	timer4.countTicks = 0;
	while(timer4.countTicks < num_of_sec);

	disable_timer4();

}


/*when the function reaches the timeout, it returns TRUE (=1).
 Else returns false*/
BOOL timeout_with_timer2(uint32_t num_of_sec)
{
	enable_timer2();

	if(timer2.countTicks >= num_of_sec){
		disable_timer2();
		timer2.countTicks = 0;
		return TRUE;
	}
	return FALSE;

}

/*when the function reaches the timeout, it returns TRUE (=1).
 Else returns false*/
BOOL timeout_with_timer3(uint32_t num_of_sec)
{
	enable_timer3();

	if(timer3.countTicks >= num_of_sec){
		disable_timer3();
		timer3.countTicks = 0;
		return TRUE;
	}
	return FALSE;

}

/*when the function reaches the timeout, it returns TRUE (=1).
 Else returns false*/
BOOL timeout_with_timer4(uint32_t num_of_sec)
{
	enable_timer4();

	if(timer4.countTicks >= num_of_sec){
		disable_timer4();
		timer4.countTicks = 0;
		return TRUE;
	}
	return FALSE;

}


void set_timeout_timer2(uint32_t num_of_sec)
{
	timer2.targetTick = num_of_sec;
	enable_timer2();
}

BOOL timeout_done_timer2(void){


	if(timer2.countTicks >= timer2.targetTick){

		disable_timer2();
		timer2.countTicks = 0;
		timer2.targetTick = 0; // cleanup
		return TRUE;
	}
	return FALSE;

}

/*This function sets timer3 num_of_sec_timer3 to param num_of_sec, and enables timer3 timeout*/
void set_timeout_timer3(uint32_t num_of_sec)
{
	timer3.targetTick = num_of_sec;
	enable_timer3();
}

/*returns true if timeout is done, otherwise returns false*/
BOOL timeout_done_timer3(void){

	if(timer3.countTicks >= timer3.targetTick){

		disable_timer3();
		timer3.countTicks = 0;
		timer3.targetTick = 0; // cleanup
		return TRUE;
	}
	return FALSE;

}


/*This function sets timer3 num_of_sec_timer3 to param num_of_sec, and enables timer3 timeout*/
void set_timeout_timer4(uint32_t num_of_seconds)
{
	timer4.targetTick = num_of_seconds;
	enable_timer4();
}

/*returns true if timeout is done, otherwise returns false*/
BOOL timeout_done_timer4(void){

	if(timer4.countTicks >= timer4.targetTick){

		disable_timer4();
		timer4.countTicks = 0;
		timer4.targetTick = 0; // cleanup
		return TRUE;
	}
	return FALSE;

}



void TIM2_IRQHandler(void)
{
	if((TIM2->SR & 0x0001) == 0x0001)
	{
		TIM2->SR &= ~(0x0001);
		timer2.countTicks++;

	}
	write_usart2((uint8_t*)("\r\nTIMER2\r\n"));//For test


}

void TIM3_IRQHandler(void)
{
	if((TIM3->SR & 0x0001) == 0x0001)
	{
		TIM3->SR &= ~(0x0001);
		timer3.countTicks++;

	}
	write_usart2((uint8_t*)("\r\nTIMER3\r\n"));//For test


}

void TIM4_IRQHandler(void)
{
	if((TIM4->SR & 0x0001) == 0x0001)
	{
		TIM4->SR &= ~(0x0001);
		timer4.countTicks++;

	}
	write_usart2((uint8_t*)("\r\nTIMER4\r\n"));//For test

}

