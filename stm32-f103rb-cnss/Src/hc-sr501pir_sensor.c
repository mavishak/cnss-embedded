/*
 * hc-sr501pir_sensor.c
 *
 *  Created on: 21 December  2020
 *      Author: Mayan and Naomi
 *
 *  Notes:
 *  21.12.2020: Sensor works using D6 interrupt and LD2 as response.
 *  			TBD: Change D6 (PB10) to D5 (PB4) and change response to UART, and play with sensor.
 *
 *  22.12.2020: D5 (PB4) is set as input pin.
 *  			TBD: Change response to UART, and play with sensor.
 *
 *  03.01.2021: USART2 is now configured to respond to the sensor interrupt.
 *  			TBD: Implement triggring of USART with event_queue.
 *  			Question: Should the handler init the uart() and then trigger write() or should UART be initiolized in advance.
 *  					  My gut tells me that the handler should do both...
 */


#include "stm32f103xb.h"
#include "hc-sr501pir_sensor.h"
#include "usart.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ() and NVIC_SetPriority()*/

/*
 * This functions intiolizes pin D5
 * to listen for sensors change in mode.
 * If motion is sensed an iterrupt occurs and triggers EXTI4_IRQHandler(void)
 */
void init_sensor_with_interrupt(){

//	/*~~~USING D6 (PB10)~~~*/
// ---WORKS--DO-NOT-ERASE!!! (keep to check that eventqueue is working and for later perpuses)---
//
//	/*Enabla RCC for GPIO Port B*/
//	RCC->APB2ENR |= 0x00000008; //enable reset and control clock for port B (see reference manual 8.3.7)
//
//	/*Enable RCC for AFIO (alternative function - interrupt)*/
//	RCC->APB2ENR |= 0x00000001; //enable AFIO clk for interrupts (see reference manual 7.3.7 and 9.4)
//
//	/*Configure sensor input pin (When motion detected senser pinout switches from low to high*/
//	GPIOB->CRH &= 0xFFFFF0FF; //reset CNF and MODE only for bit 10, leave all the rest as they are
//	GPIOB->CRH |= 0x00000800; //PB10 configured to Input with pull-up / pull-down | Input mode (reset state) (see refrence manual 9.2.2)
//	GPIOB->ODR |= 0x00000400; //PB10 configures as pull-up (see reference manual 9.2.4 and Table 20 pg. 161)
//
//	/*Enable external interrupt*/
//	__disable_irq();
//	//NOTE: EXTICR3 is reachable via the index 2
//	AFIO->EXTICR[2] |= 0x00000100; // Allow interupts for line_10 (see reference manual 9.4.6 and pg. 210)
//	EXTI->IMR |= 0x00000400; //Enable iterrupt with mask for port B pin 10 (see reference manual 10.3.1)
//	EXTI->RTSR |= 0x00000400 ; //rising trigger selection register - to anable full button press before reacting ((see reference manual 10.3.3)
//	NVIC_SetPriority(EXTI15_10_IRQn,0); //set all interrupt priotity to zero so that no preemption uccors.
//	NVIC_EnableIRQ(EXTI15_10_IRQn); //enable handler
//	__enable_irq();
//
// ---WORKS--DO-NOT-ERASE!!! (keep to check that eventqueue is working and for later perpuses)---
//	/*~~~______________~~~*/



	/*~~~USING D5 (PB4)~~~*/

	/*Enabla RCC for GPIO Port B*/
	RCC->APB2ENR |= 0x00000008; //enable reset and control clock for port B (see reference manual 8.3.7)

	/*Enable RCC for AFIO (alternative function - interrupt)*/
	RCC->APB2ENR |= 0x00000001; //enable AFIO clk for interrupts (see reference manual 7.3.7 and 9.4)

	/*Configure sensor input pin (When motion detected senser pinout switches from low to high*/
	GPIOB->CRL &= 0xFFF0FFFF; //reset CNF and MODE only for bit 4, leave all the rest as they are
	GPIOB->CRL |= 0x00080000; //PB4 configured to Input with pull-up / pull-down | Input mode (reset state) (see refrence manual 9.2.2)
	GPIOB->ODR |= 0x00000010; //PB4 configures as pull-up (see reference manual 9.2.4 and Table 20 pg. 161)

	/*Enable external interrupt*/
	__disable_irq();
	//NOTE: EXTICR2 is reachable via the index 1
	AFIO->EXTICR[1] |= 0x00000001; // Allow interupts for line_4 (see reference manual 9.4.4 and pg. 210)
	EXTI->IMR |= 0x00000010; //Enable iterrupt with mask for port B pin 4 (see reference manual 10.3.1)
	EXTI->RTSR |= 0x00000010 ; //rising trigger selection register - to anable full button press before reacting ((see reference manual 10.3.3)
	NVIC_SetPriority(EXTI4_IRQn,0); //set all interrupt priotity to zero so that no preemption uccors.
	NVIC_EnableIRQ(EXTI4_IRQn); //enable handler
	__enable_irq();

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

/*Interrupt service routine for sensor using pin D5 (PB4) as input mode*/
void EXTI4_IRQHandler(void)
{

	EXTI->PR |= 0x00000010; //reset flag by writing 1 to bit 4 (reference manual 10.3.6)

	toggle_led(); //This is temporary for testing.
	write(); //This chould be executed using the event_queue

}

/*Interrupt service routine for sensor using pin D6 (PB10) as input mode.
 *Note: Not in use.*/
void EXTI15_10_IRQHandler(void)
{

	EXTI->PR |= 0x00000400; //reset flag by writing 1 to bit 10 (reference manual 10.3.6)
	toggle_led();

}
