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


#include "esp8266_WiFi.h" /*for testing usart1...*/



int main(void)
{

	//init_sensor_with_interrupt();
	//init_sensor_led_response();
	init_usart2(); // for dbugging
	init_usart1(); // for ESP8266
	//write_usart2((uint8_t*)AT_COMMAND);

	write_usart2((uint8_t*)("\r\n_______________\r\n"));//For test

	//connectFirbase();
//	write_usart2((uint8_t*)"testing delay\r\n");
//	delay(10);
//	write_usart2((uint8_t*)"testing delay\r\n");

//	recordAlert();

	//TestWifiConnection(); //THE FUNCTION NAME HAS NOTHING TO DO WITH IT"S CONTENT...! :)~

	while(1)
	{
		recordAlert();
//		write_usart2((uint8_t*)"0 sec\r\n");
//		delay();
//		//delay();
//		write_usart2((uint8_t*)"1 sec\r\n");
//		delay();
//		//delay();
//		write_usart2((uint8_t*)"2 sec\r\n");
//		delay();
//		//delay();
//		write_usart2((uint8_t*)"3 sec\r\n");
//		delay();
//		//delay();
//		write_usart2((uint8_t*)"5 sec\r\n");
//		delay();
//		//delay();

		//write_usart2((uint8_t*)MSG);//Used only when Testing USART2 without sensor
		//write_usart1((uint8_t*)AT_COMMAND); // WRITE TEST COMAND
		//uint32_t found = search_usart1_buffer_Rx((uint8_t *)AT_OK, (uint8_t *)AT_ERROR);
		//if(found)
		//{
		//	write_usart2((uint8_t*)"TEST PASSED\r\n");
		//}
		//else{ //RECEIVING A RESPONSE TAKES TIME IT DOES NOT MEEN THE TEST FAILED
		//	write_usart2((uint8_t*)"TEST FAILED\r\n");
		//}

	}
}



/*Interrupt service routine for sensor*/
/*void EXTI15_10_IRQHandler(void)
{

	EXTI->PR |= 0x00000400; //reset flag by writing 1 to bit 10 (reference manual 10.3.6)
	toggle_led();

}
*/




