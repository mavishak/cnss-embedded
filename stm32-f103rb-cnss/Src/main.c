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
  #warning "FPU is not initialised, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "stm32f103xb.h"
// #include "cmsis_gcc.h"/*for __disable/enable_irq()*/
// #include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/


#include "event_queue.h"
#include "hc-sr501pir_sensor.h"
#include "usart.h"
#include "esp8266_Firebase.h"
#include "esp8266_WiFi.h" //for testing usart1...
#include "timers.h"
#include "common.h"


/*FOR TESTING*/
//#include "event_queue.h"
//#include "gpio_signal.h" /*for testing*/
/*FOR TESTING*/


int main(void)
{

	/*FOR TESTING*/
	//init_queue();
	//init_interrupt();
	/*FOR TESTING*/

	init_usart2(); // for debugging

	write_usart2((uint8_t*)("\r\n_______________\r\n"));//For test

	init_queue();
	init_sensor_with_interrupt();
	//init_sensor_led_response();

	//init_timer2();
	//init_timer3();
	init_timer4();

	init_usart1(); // for ESP8266


	//if(recordAlert()){
	//	write_usart2((uint8_t*)(":)\r\n"));
	//}
	//else{
	//	write_usart2((uint8_t*)(":(\r\n"));
	//}


	while(1)
	{
		/*FOR TESTING*/
		do_event();
		/*FOR TESTING*/

		//if(recordAlert()){
		//	write_usart2((uint8_t*)(":)\r\n"));
		//}
		//else{
		//	write_usart2((uint8_t*)(":(\r\n"));
		//}
		//
		//write_usart2((uint8_t*)("\r\n.....................\r\n"));
		//write_usart2((uint8_t*)("\r\n\r\n"));

	}
}
