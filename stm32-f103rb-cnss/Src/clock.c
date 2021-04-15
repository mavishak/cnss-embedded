/*
 * clock.c
 *
 *  Created on: 15 April 2021
 *      Author: mayan and naomi
 */

#include "stm32f103xb.h"
//#include "system_stm32f1xx.h"


/*This function should be called before any other function in this project*/
void set_sys_clock_to_32MHz(void){


	/*

	[reference manual p. 129 section 8.2.3]::PLL

	 The configuration of each PLL:
	 - selection of clock source
	 - predivision factor
	 - multiplication factor
	 must be done before enabling the PLL

	 PLL should be enabled
	 after its input clock becomes stable (ready flag)

	 Once the PLL is enabled, these parameters
	 can not be changed.

	 When changing the entry clock source of the main PLL, the original clock source must be
	 switched off only after the selection of the new clock source (done through bit PLLSRC in
	 the Clock configuration register (RCC_CFGR)).

	 [reference manual p. 130 section 8.2.6]::System clock

	 Status bits in the Clock
	 control register (RCC_CR) indicate which clock(s) is (are) ready and which clock is currently
	 used as system clock.

	 */

	//HSION, HSIRDY, HSITRIM and HSICAL are set to default values

	/*Enable HSI clock [refernce manual p. 134 section 8.3.4]*/
	//RCC->CR |= 0x00000001; // default value, set HSION

	while((RCC->CR & 0x00000002) != 0x00000002); // wait while HSI is not ready: HSIRDY = 0

	/*set PLL entry clock src*/
	//By default RCC_CFGR[PLLSRC] = HSI oscillator clock/2

	/*set PLL multiplication factor x8 [reference manual p. 135 section 8.3.2]*/
	RCC->CFGR &= 0xFFC3FFFF; //reset PLLMUL
	RCC->CFGR |= 0x00180000; //0110: PLL input clock x 8

	/*Configur AHB prescaler [referance manual p. 137 section 8.3.2]*/
	RCC->CFGR &= 0xFFFFFF0F; //reset HPRE
	RCC->CFGR |= 0x00000090; //1001: SYSCLK divided by 4 -> 32/4 = 8MHz

	/*Enable PLL [refernace manual p. 133 section 8.3.1]*/
	RCC->CR |= 0x01000000;//set PLLON

	while((RCC->CR & 0x02000000) != 0x02000000); // wait while PLL not ready: PLLRDY = 0

	/*Set system clock [referance manual p. 137 section 8.3.2]*/
	RCC->CFGR &= 0xFFFFFFF0;// reset SW and SWS, just in case
	RCC->CFGR |= 0x00000002; //Configer system clock as PLL

	while((RCC->CFGR & 0x00000008) != 0x00000008); // wait while system clock is not PLL: SWS != 10


	//THIS FUNCTION IS NOT ALLOWING BUILD - WE DON"T KNOW IF IT IS REALY NEEDED
	//SystemCoreClockUpdate();//must be called whenever the core clock is changed during program execution.
	//The function evaluates the clock register settings and calculates the current core clock.


}


void init_MCO(void){

	/*Enable Port A RCC clock (PA8 corresponds to MCO) [Datasheet p. 31 Table 5.] */
	RCC->APB2ENR |= 0x00000004; // set IOPAEN [reference manual section 8.3.7]

	/*Clock enable AFIO*/
	RCC->APB2ENR |= 0x00000001; // set AFIOEN [reference manual section 8.3.7]

	/*Configure PA8 as Alternate function mode push pull [reference manual p. 170 Table 33.]*/
	GPIOA->CRH &= 0xFFFFFFF0; // reset CNF8 and MODE8 [reference manual 9.2.2]
	GPIOA->CRH |= 0x0000000A; // set PA8 as Alternate function push-pull | frequency of 2 MHz

	/*Set clock src for Microcontorller clock output (MCO) [refernce manual section 8.3.2 p. 135]*/
	RCC->CFGR &= F0FFFFFF; // reset MCO
	RCC->CFGR |= 04000000; // 0100: System clock (SYSCLK) selected

}

