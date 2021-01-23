/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "stm32f103xb.h"
// #include "cmsis_gcc.h"/*for __disable/enable_irq()*/
// #include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/
#include "event_queue.h"
#include "hc-sr501pir_sensor.h"
#include "usart.h"




int main(void)
{

	//init_sensor_with_interrupt();
	//init_sensor_led_response();
	init_usart2();
	set_usart2_buffer_Tx((uint8_t*)MSG);
	while(1)
	{
		write_usart2();//Used only when Testing USART2 without sensor
	}
}



/*Interrupt service routine for sensor*/
/*void EXTI15_10_IRQHandler(void)
{

	EXTI->PR |= 0x00000400; //reset flag by writing 1 to bit 10 (reference manual 10.3.6)
	toggle_led();

}
*/


void SysTick_Handler(void)
{

}

