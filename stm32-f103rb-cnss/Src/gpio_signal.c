
#include "gpio_signal.h"
#include "cmsis_gcc.h"/*for __disable/enable_irq()*/
#include "core_cm3.h" /*for NVIC_enableIRQ()*/

/*FOR TESTING*/
#include "event_queue.h"
/*FOR TESTING*/


/*This function enables user button B1 and led LD2*/
void init()
{
	/*RCC enabled for user button PC13*/
	RCC->APB2ENR |= 0x0010; //enable reset and control clock for port c (see reference manual 8.3.7)

	/*RCC enabled for led PA5*/
	RCC->APB2ENR |= 0x0004; //enable reset and control clock for port A (see reference manual 8.3.7)

	/*Configure user button PC13*/
	GPIOC->CRH &= 0xFF0FFFFF; //reset CNF and MODE only for bit 13, leave all the rest as they are
	GPIOC->CRH |= 0x00400000; //enable input with floating input reset IDR bit 13 (see reference manual 9.2.2)

	/*Configure led PA5*/
	GPIOA->CRL &= 0xFF0FFFFF; //reset CNF and MODE only for bit 5, leave all the rest as they are
	GPIOA->CRL |= 0x00200000; //PA5 configured to General purpose output push-pull | Output mode, max speed 2 MHz

}

/*This function enables user button B1 and led LD2 and interrupts*/
void init_interrupt()
{
	state = OFF;

	/*RCC enabled for user button PC13*/
	RCC->APB2ENR |= 0x00000010; //enable reset and control clock for port c (see reference manual 8.3.7)

	/*RCC enabled for led PA5*/
	RCC->APB2ENR |= 0x00000004; //enable reset and control clock for port A (see reference manual 8.3.7)

	/*RCC enable AFIO*/
	RCC->APB2ENR |= 0x00000001; //enable AFIO clk for interrupts (see reference manual 8.3.7 and 9.4)

	/*Configure user button PC13*/
	GPIOC->CRH &= 0xFF0FFFFF; //reset CNF and MODE only for bit 13, leave all the rest as they are
	GPIOC->CRH |= 0x00800000; //enable input with pullup/pulldown reset IDR bit 13 (see reference manual 9.2.2)
	GPIOC->ODR |= 0x00002000; // pull up

	/*Configure led PA5*/
	GPIOA->CRL &= 0xFF0FFFFF; //reset CNF and MODE only for bit 5, leave all the rest as they are
	GPIOA->CRL |= 0x00200000; //PA5 configured to General purpose output push-pull | Output mode, max speed 2 MHz

	/*Enable interrupts for PC13 rising triger*/
	__disable_irq();
	/*NOTE: It seems that EXTICR4 is reachable via the third index*/
	AFIO->EXTICR[3] |= 0x00000020; //Set interrupts for port c in line_13(see reference manual 9.4.6)
	EXTI->IMR |= 0x00002000; //Enable iterrupt with mask (see reference manual 10.3.1)
	EXTI->RTSR |= 0x00002000 ; //rising trigger selection register ((see reference manual 10.3.3)
	NVIC_EnableIRQ(EXTI15_10_IRQn); //enable handler
	__enable_irq();


}

/*This function Initiolizes D6 (PB10) as input with interrupt and LD2 (PA5) as output*/
void init_interrupt_D6(){

	state = OFF;

	/*RCC enabled for led PA5*/
	RCC->APB2ENR |= 0x00000004; //enable reset and control clock for port A (see reference manual 8.3.7)

	/*RCC enabled for D6 GPIO - (PB10) (external button)*/
	RCC->APB2ENR |= 0x00000008; //enable reset and control clock for port B (see reference manual 8.3.7)

	/*RCC enable AFIO*/
	RCC->APB2ENR |= 0x00000001; //enable AFIO clk for interrupts (see reference manual 7.3.7 and 9.4)

	/*Configure led PA5*/
	GPIOA->CRL &= 0xFF0FFFFF; //reset CNF and MODE only for bit 5, leave all the rest as they are
	GPIOA->CRL |= 0x00200000; //PA5 configured to General purpose output push-pull | Output mode, max speed 2 MHz

	/*Configure external button PB10*/
	GPIOB->CRH &= 0xFFFFF0FF; //reset CNF and MODE only for bit 10, leave all the rest as they are
	GPIOB->CRH |= 0x00000800; //PB10 configured to Input with pull-up / pull-down | Input mode (reset state)
	GPIOB->ODR |= 0x00000400; //PB10 configures as pull-up (see reference manual 9.2.4 and Table 20 pg. 161)

	/*Enable interrupts for PB10 rising triger*/
	__disable_irq();
	/*NOTE: It seems that EXTICR3 is reachable via the second index*/
	AFIO->EXTICR[2] |= 0x00000100; // Allow interupts for line_10 (see reference manual 9.4.6 and pg. 210)
	EXTI->IMR |= 0x00000400; //Enable iterrupt with mask for port B pin 10 (see reference manual 10.3.1)
	EXTI->RTSR |= 0x00000400 ; //rising trigger selection register - to anable full button press before reacting ((see reference manual 10.3.3)
	NVIC_EnableIRQ(EXTI15_10_IRQn); //enable handler
	__enable_irq();


}

/*This function returns 1 if user button B1 was pressed*/
uint32_t was_pressed()
{

	return (GPIOC->IDR & 0x2000) == 0x2000;
}



void toggle_ld2()
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


/*The main and interrupts we used*/
/*

 // This program blinks led LD2 when button B1 is pushed
 int main(void)
{


	state = OFF;
	//init();
	init_interrupt();
	//init_interrupt_D6();
	//SysTick_Config(15000000);

	while(1)
	{
	//		if(was_pressed())
	//		{
	//			i=5;
	//			toggle_led();
	//		}

	}
}
*/

// Interrupt service routine for user button
void EXTI15_10_IRQHandler(void) //EXTI4_IRQHandler(void)
{

	EXTI->PR = 0x00002000; //reset flag by writing 1 to bit 13 (reference manual 10.3.6)
	EXTI->PR |= 0x00000400; //reset flag by writing 1 to bit 10 (reference manual 10.3.6)

	add_event(interrupt_handler);
	//interrupt_handler();
	//	if(state == OFF)
	//	{
	//		GPIOA->ODR |= 0x0020;
	//		state=ON;
	//	}
	//	else
	//	{
	//		GPIOA->ODR &= ~(0x0020);
	//		state=OFF;
	//	}
}

void *interrupt_handler(void){

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

	return NULL;

}




