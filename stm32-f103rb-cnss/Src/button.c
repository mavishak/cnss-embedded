/*
 * button.c
 *
 *  Created on: 27 May 2021
 *      Author: Owner
 */
#include "button.h"
#include "stm32f103xb.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/
#include <string.h>
#include <stdlib.h>

static BOOL button_pressed;

void BOTTON_init()
{
	/*RCC enabled for user button PC13*/
	RCC->APB2ENR |= 0x00000010; //enable reset and control clock for port c (see reference manual 8.3.7)

	/*Configure user button PC13*/
	GPIOC->CRH &= 0xFF0FFFFF; //reset CNF and MODE only for bit 13, leave all the rest as they are
	GPIOC->CRH |= 0x00400000; //enable input with floating input reset IDR bit 13 (see reference manual 9.2.2)

	/*RCC enable AFIO*/
	RCC->APB2ENR |= 0x00000001; //enable AFIO clk for interrupts (see reference manual 8.3.7 and 9.4)

	/*Configure user button PC13*/
	GPIOC->CRH &= 0xFF0FFFFF; //reset CNF and MODE only for bit 13, leave all the rest as they are
	GPIOC->CRH |= 0x00800000; //enable input with pullup/pulldown reset IDR bit 13 (see reference manual 9.2.2)
	GPIOC->ODR |= 0x00002000; // pull up

	/*Enable interrupts for PC13 rising trigger*/
	__disable_irq();
	/*NOTE: It seems that EXTICR4 is reachable via the third index*/
	AFIO->EXTICR[3] |= 0x00000020; //Set interrupts for port c in line_13(see reference manual 9.4.6)
	EXTI->IMR |= 0x00002000; //Enable interrupt with mask (see reference manual 10.3.1)
	EXTI->RTSR |= 0x00002000 ; //rising trigger selection register ((see reference manual 10.3.3)
	NVIC_EnableIRQ(EXTI15_10_IRQn); //enable handler
	__enable_irq();
}

BOOL BUTTON_pressed(void){
	if(button_pressed){
		button_pressed = FALSE; // reset flag
		return TRUE;
	}
	return FALSE;
}

// Interrupt service routine for user button
//void EXTI15_10_IRQHandler(void)
//{
//	EXTI->PR = 0x00002000; //reset flag by writing 1 to bit 13 (reference manual 10.3.6)
//	button_pressed = TRUE;
//}
