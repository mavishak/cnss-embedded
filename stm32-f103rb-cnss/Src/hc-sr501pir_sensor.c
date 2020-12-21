/*
 * hc-sr501pir_sensor.c
 *
 *  Created on: 21 December  2020
 *      Author: Mayan and Naomi
 *
 *  Notes:
 *  21.12.2020: Sensor works using D6 interrupt and LD2 as response.
 *  			TBD: Change D6 to D5 (PB4) and change response to UART, and play with sensor.
 */


#include "stm32f103xb.h"
#include "hc-sr501pir_sensor.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ()*/


void init_sensor_with_interrupt(){

	/*~~~USING D6 (PB10)~~~*/

	/*Enabla RCC for GPIO Port B*/
	RCC->APB2ENR |= 0x00000008; //enable reset and control clock for port B (see reference manual 8.3.7)

	/*Enable RCC for AFIO (alternative function - interrupt)*/
	RCC->APB2ENR |= 0x00000001; //enable AFIO clk for interrupts (see reference manual 7.3.7 and 9.4)

	/*Configure sensor input pin (When motion detected senser pinout switches from low to high*/
	GPIOB->CRH &= 0xFFFFF0FF; //reset CNF and MODE only for bit 10, leave all the rest as they are
	GPIOB->CRH |= 0x00000800; //PB10 configured to Input with pull-up / pull-down | Input mode (reset state) (see refrence manual 9.2.2)
	GPIOB->ODR |= 0x00000400; //PB10 configures as pull-up (see reference manual 9.2.4 and Table 20 pg. 161)

	/*Enable external interrupt*/
	__disable_irq();
	//NOTE: EXTICR3 is reachable via the second index
	AFIO->EXTICR[2] |= 0x00000100; // Allow interupts for line_10 (see reference manual 9.4.6 and pg. 210)
	EXTI->IMR |= 0x00000400; //Enable iterrupt with mask for port B pin 10 (see reference manual 10.3.1)
	EXTI->RTSR |= 0x00000400 ; //rising trigger selection register - to anable full button press before reacting ((see reference manual 10.3.3)
	NVIC_EnableIRQ(EXTI15_10_IRQn); //enable handler
	__enable_irq();

	/*~~~______________~~~*/



	/*~~~USING D5 (PB4)~~~*/

	/*Enabla RCC for GPIO*/

	/*Enable RCC for AFIO (alternative function - interrupt)*/

	/*Configure sensor input pin (When motion detected senser pinout switches from low to high*/

	/*Enable external interrupt*/

	/*~~~______________~~~*/


}


void init_sensor_led_response(){

	/*~~~USING LD2 (PA5) as output~~~*/

	/*Enabla RCC for GPIO Port A*/
	RCC->APB2ENR |= 0x00000004; //enable reset and control clock for port A (see reference manual 8.3.7)

	/*Configure led PA5*/
	GPIOA->CRL &= 0xFF0FFFFF; //reset CNF and MODE only for bit 5, leave all the rest as they are
	GPIOA->CRL |= 0x00200000; //PA5 configured to General purpose output push-pull | Output mode, max speed 2 MHz

	/*Init state*/
	state = OFF;


}


void toggle_led()
{

	if(state == OFF)
	{
		GPIOA->ODR |= 0x0020;
		state=ON;
	}
	else
	{
		GPIOA->ODR &= ~(0x0020);
		state=OFF;
	}
}
