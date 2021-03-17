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

void init_timer2(){

//-----???????-----------------
	/* Clock control register (RCC_CR) {p.132 in the reference manual} */
	//By Default: RCC->CR |= 0x00000001; // HSION: Internal high-speed clock enable

	/* APB1 peripheral reset register (RCC_APB1RSTR) {p.142 in the reference manual} */
	//Not useful: RCC->APB1RSTR |= 0x00000001; // TIM2RST: TIM2 timer reset(redundant ???)
//-----???????-----------------


	/* APB1 peripheral clock enable register (RCC_APB1ENR) {p.148 in the reference manual} */
	RCC->APB1ENR |= 0x00000001; // TIM2EN: TIM2 timer clock enable

	/* AFIO  {p. in the reference manual} */
	//RCC->APB2ENR |= 0x00000001;

	/* TIM2 Set Prescaler {see: 15.4.11 in the Reference manual}*/
	TIM2->PSC = 0xFFFF;

	/* TIM2 Set TIMx auto-reload register {p.419 in the Reference manual}*/
	TIM2->ARR = 0x0007;//0xFFFF;

	/* TIMx control register 1 (TIMx_CR1) {p.404 in the reference manual} */
	//TIM2->CR1 |= 0x0010; // DIR
	//TIM2->CR1 |= 0x0080; // ARPE
	TIM2->CR1 |= 0x0001; // CEN: Counter enable


	//TIM2->CR1 |= 0x0080; // ARPE: Auto-reload preload enable(redundant ???)

	/* TIMx status register (TIMx_SR) {p.410 in the Reference manual} */
	//TIM2->SR |= 0x0001; // UIF: Update interrupt flag

	/* TIMx auto-reload register (TIMx_ARR) {p.419 in the Reference manual} */
	//TIM2->ARR//holds the number of ticks that the timer will count

	//When the time is over - the UIF bit is set to 1
}

/*USART2 has to be enabled*/
void test_timer2(void){

	uint32_t i = TIM2->CNT;
	write_usart2((uint8_t *)i);

//	write_usart2((uint8_t *)TIM2->CNT);
	write_usart2((uint8_t *)"\r\n");
}




void wait_with_timer2(uint32_t num_of_millis){
	init_timer2();
}


