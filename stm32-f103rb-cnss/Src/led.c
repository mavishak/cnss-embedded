/*
 * led.c
 *
 *  Created on: 27 May 2021
 *      Author: Owner
 */
#include "led.h"
#include "stm32f103xb.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/
#include <string.h>
#include <stdlib.h>

void LED_init(){
	/*USING LD2 (PA5) as output*/
	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; //enable reset and control clock for port A (see reference manual 8.3.7)

	/*Configure led PA5*/
	GPIOA->CRL &= 0xFF0FFFFF; //reset CNF and MODE only for bit 5, leave all the rest as they are
	GPIOA->CRL |= 0x00200000; //PA5 configured to General purpose output push-pull | Output mode, max speed 2 MHz

}

void LED_toggle(){
	// Toggle GPIOA pin 5.
	GPIOA->ODR ^= 0x0020;
}

void LED_on()
{
	GPIOA->ODR |= 0x0020;
}

void LED_off()
{
	GPIOA->ODR &= ~(0x0020);
}


