/*
 * timer.c
 *
 *  Created on: 16 Mar 2021
 *      Author: Owner
 */
#include "stm32f103xb.h"
#include "timers.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

static uint32_t countTicks2;
static uint32_t countTicks3;
static uint32_t countTicks4;


void init_timer2(void){

	/* APB1 peripheral clock enable register (RCC_APB1ENR) {p.148 in the reference manual} */
	RCC->APB1ENR |= 0x00000001; // TIM2EN: TIM2 timer clock enable

	/* ARR*PSC=8000 -> 1ms */

	/* TIM2 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	TIM2->PSC = 0x0064; //0x0064[Hex] = 100[dec]

	/* TIM2 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	TIM2->ARR = 0x0050; //0x0050[Hex] = 80[dec]

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
}
void init_timer3(void){
	/* APB1 peripheral clock enable register (RCC_APB1ENR) {p.148 in the reference manual} */
	RCC->APB1ENR |= 0x00000002; // TIM3EN: TIM3 timer clock enable

	/* ARR*PSC=8000 -> 1ms */

	/* TIM3 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	TIM3->PSC = 0x0064; //0x0064[Hex] = 100[dec]

	/* TIM3 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	TIM3->ARR = 0x0050; //0x0050[Hex] = 80[dec]

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
}
void init_timer4(void)
{
	/* APB1 peripheral clock enable register (RCC_APB1ENR) {p.148 in the reference manual} */
	RCC->APB1ENR |= 0x00000004; // TIM4EN: TIM4 timer clock enable

	/* ARR*PSC=8000 -> 1ms */

	/* TIM4 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	TIM4->PSC = 0x0064; //0x0064[Hex] = 100[dec]

	/* TIM4 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	TIM4->ARR = 0x0050; //0x0050[Hex] = 80[dec]

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


void delay_with_timer2(uint32_t num_of_millis)
{
	enable_timer2();

	countTicks2 =0;
	while(countTicks2 < num_of_millis);

	disable_timer2();
}
void delay_with_timer3(uint32_t num_of_millis)
{
	enable_timer3();

	countTicks3 =0;
	while(countTicks3 < num_of_millis);

	disable_timer3();
}
void delay_with_timer4(uint32_t num_of_millis)
{
	enable_timer4();

	countTicks4 =0;
	while(countTicks4 < num_of_millis);

	disable_timer4();
}


void TIM2_IRQHandler(void)
{
	if((TIM2->SR & 0x0001) == 0x0001)
	{
		countTicks2++;
		TIM2->SR &= ~(0x0001);
	}
}
void TIM3_IRQHandler(void)
{
	if((TIM3->SR & 0x0001) == 0x0001)
	{
		countTicks3++;
		TIM3->SR &= ~(0x0001);
	}
}
void TIM4_IRQHandler(void)
{
	if((TIM4->SR & 0x0001) == 0x0001)
	{
		countTicks4++;
		TIM4->SR &= ~(0x0001);
	}
}

/*when the function reaches the timeout, it returns TRUE (=1).
 * Before using this function, call enable_timer2().
 * When done, call disable_timer2();
 * */

void timeout_with_timer2(uint32_t num_of_millis)
{
//	enable_timer2();
//
//	countTimeout2 =0;
//	while(countTicks2 < num_of_millis);
//
//	disable_timer2();
}
void timeout_with_timer3(uint32_t num_of_millis)
{

}
void timeout_with_timer4(uint32_t num_of_millis)
{

}
