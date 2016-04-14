/*
 * timers.c
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 *
 *      This library is intended for ATxmega128A1-AU family of microcontrollers.
 *      It's purpose is to help the user with timer initialization.
 *
 *	    To initialize a 16-bit timer of type 0 call the function
 *
 *     		initTimerType0('TimerYouWishToSet', 'TimerInitalizationStruct');
 *
 *     	Where 'TimerYouWishToSet' is one of the TCx0 timers, and 'TimerInitalizationStruct'
 *     	is data structure of type Timer_Struct with the following attributes:
 *
 *     		Mode - operating mode of the timer:
 *      		* TIM_MODE_NORMAL / TIM_MODE_SINGLE_SLOPE_PWM / TIM_MODE_DOUBLE_SLOPE_PWM
 *
 *     		InterruptLvl - priority level of timer's overflow/underflow interrupt routine:
 *     			* TIM_INT_LVL_LOW / TIM_INT_LVL_MID / TIM_INT_LVL_HIGH
 *
 *     		Prescaler - timer prescaler for the CLKper clock source:
 *     			* TIM_PRSC_1 / TIM_PRSC_2 / TIM_PRSC_4 / TIM_PRSC_8 / TIM_PRSC_64
 *              * NOTE: Timer clock source is already scaled down with oscillator prescalers A, B and C.
 *                      Refer to clock.h and clock.c for more details.
 *
 *      	Period - number of clock cycles before and between interrupt routines.
 *      		* uint16_t (unsigned 16-bit data type)
 *
 *
 *      To initialize a 16-bit timer of type 1 call the function
 *
 *     		initTimerType0('TimerYouWishToSet', 'TimerInitalizationStruct');
 *
 *     	Where 'TimerYouWishToSet' is one of the TCx1 timers, and 'TimerInitalizationStruct'
 *     	is data structure of type Timer_Struct with the following attributes:
 *
 *     		Mode - operating mode of the timer:
 *      		* TIM_MODE_NORMAL / TIM_MODE_SINGLE_SLOPE_PWM / TIM_MODE_DOUBLE_SLOPE_PWM
 *
 *     		InterruptLvl - priority level of timer's overflow/underflow interrupt routine:
 *     			* TIM_INT_LVL_LOW / TIM_INT_LVL_MID / TIM_INT_LVL_HIGH
 *
 *     		Prescaler - timer prescaler for the CLKper clock source:
 *     			* TIM_PRSC_1 / TIM_PRSC_2 / TIM_PRSC_4 / TIM_PRSC_8 / TIM_PRSC_64
 *              * NOTE: Timer clock source is already scaled down with oscillator prescalers A, B and C.
 *                      Refer to clock.h and clock.c for more details.
 *
 *      	Period - number of clock cycles before and between interrupt routines.
 *      		* uint16_t (unsigned 16-bit data type)
 *
 *
 *      Additional functions:
 *
 *       	getTimeoutDiff('SomeTime');
 *       	- Return value: number of counts that timeout timer (enabled by default) will have after
 *       	'SomeTime' number of interrupt routines.
 *
 *       	getTimeoutValue();
 *       	- Return value: current number of counts of timeout timer.
 *
 *       NOTE: Timeout timer can only calculate timeouts of under MAX_TIMEOUT number of interrupts.
 */

#include "timers.h"
#include <avr/interrupt.h>

#define MAX_TIMEOUT ((uint32_t)10000)

typedef struct {
	volatile uint32_t TimerC0_counter0;
	volatile uint32_t TimerC0_counter1;
	volatile uint32_t TimerC0_counter2;
	volatile uint32_t TimerC0_counter3;
	volatile uint32_t TimerC1_counter0;
	volatile uint32_t TimerC1_counter1;
	volatile uint32_t TimerC1_counter2;
	volatile uint32_t TimerC1_counter3;
	volatile uint32_t TimerD0;
	volatile uint32_t TimerD1;
	volatile uint32_t TimerE0;
	volatile uint32_t TimerE1;
	volatile uint32_t TimerF0;
	volatile uint32_t TimerF1;
} Counters_S;

static Counters_S Counters;
static volatile uint32_t pause;


void init_timer_type_0(TC0_t *t, Timer_Struct *ts)
{
	/* Set interrupt level. */
	t->INTCTRLA = ts->InterruptLvl;

	/* Set timer period. */
	t->PER = ts->Period;

	/* Set timer mode. */
	t->CTRLA = ts->Mode;

	/* Set prescaler. */
	t->CTRLA = ts->Prescaler;
	
	Counters.TimerC0_counter0 = 0;
	Counters.TimerC0_counter1 = 0;
	Counters.TimerC0_counter2 = 0;
	Counters.TimerC0_counter3 = 0;
}

void init_timer_type_1(TC1_t *t, Timer_Struct *ts)
{
	/* Set interrupt level. */
	t->INTCTRLA = ts->InterruptLvl;

	/* Set timer period. */
	t->PERL = ts->Period;
    t->PERH = (ts->Period >> 8);

	/* Set timer mode. */
	t->CTRLA = ts->Mode;

	/* Set prescaler. */
	t->CTRLA = ts->Prescaler;
	
	Counters.TimerC1_counter0 = 0;
	Counters.TimerC1_counter1 = 0;
	Counters.TimerC1_counter2 = 0;
	Counters.TimerC1_counter3 = 0;
}

void pause_time(uint32_t p)
{
    pause = p;
    while(pause);
}

uint32_t getTCO_counter(uint8_t counter_number)
{
	switch (counter_number)
	{
		case 0:
		{
			return Counters.TimerC0_counter0;
		}break;
		case 1:
		{
			return Counters.TimerC0_counter1;
		}break;
		case 2:
		{
			return Counters.TimerC0_counter2;
		}break;
		case 3:
		{
			return Counters.TimerC0_counter3;
		}break;
		default:
		{
			return 0;
		}break;
	}
}

uint32_t getTC1_counter(uint8_t counter_number)
{
	switch (counter_number)
	{
		case 0:
		{
			return Counters.TimerC1_counter0;
		}break;
		case 1:
		{
			return Counters.TimerC1_counter1;
		}break;
		case 2:
		{
			return Counters.TimerC1_counter2;
		}break;
		case 3:
		{
			return Counters.TimerC1_counter3;
		}break;
		default:
		{
			return 0;
		}break;
	}
}

void setTCO_counter(uint8_t counter_number, uint32_t timeout_value)
{
	switch (counter_number)
	{
		case 0:
		{
			Counters.TimerC0_counter0 = timeout_value;
		}break;
		case 1:
		{
			Counters.TimerC0_counter1 = timeout_value;
		}break;
		case 2:
		{
			Counters.TimerC0_counter2 = timeout_value;
		}break;
		case 3:
		{
			Counters.TimerC0_counter3 = timeout_value;
		}break;
		default:
		{
			
		}break;
	}
}

void setTC1_counter(uint8_t counter_number, uint32_t timeout_value)
{
	switch (counter_number)
	{
		case 0:
		{
			Counters.TimerC1_counter0 = timeout_value;
		}break;
		case 1:
		{
			Counters.TimerC1_counter1 = timeout_value;
		}break;
		case 2:
		{
			Counters.TimerC1_counter2 = timeout_value;
		}break;
		case 3:
		{
			Counters.TimerC1_counter3 = timeout_value;
		}break;
		default:
		{
			
		}break;
	}
}

ISR(TCC0_OVF_vect)
{
    if(pause) pause--;

	/* This is part of TO-NET d.o.o. XMEGA timers library and should not be changed by any user. */
	if(Counters.TimerC0_counter0) {
		Counters.TimerC0_counter0--;
	} 
	
	if(Counters.TimerC0_counter1) {
		Counters.TimerC0_counter1--;
	}
	
	if(Counters.TimerC0_counter2) {
		Counters.TimerC0_counter2--;
	}
	
	if(Counters.TimerC0_counter3) {
		Counters.TimerC0_counter3--;
	}
}

ISR(TCC1_OVF_vect)
{
		if(Counters.TimerC1_counter0) {
			Counters.TimerC1_counter0--;
		}
		
		if(Counters.TimerC1_counter1) {
			Counters.TimerC1_counter1--;
		}
		
		if(Counters.TimerC1_counter2) {
			Counters.TimerC1_counter2--;
		}
		
		if(Counters.TimerC1_counter3) {
			Counters.TimerC1_counter3--;
		}
}

ISR(TCD0_OVF_vect)
{
	if(Counters.TimerD0 < MAX_TIMEOUT) {
		Counters.TimerD0++;
	} else {
		Counters.TimerD0 = 0;
	}
}

ISR(TCD1_OVF_vect)
{
	if(Counters.TimerD1 < MAX_TIMEOUT) {
		Counters.TimerD1++;
	} else {
		Counters.TimerD1 = 0;
	}
}

ISR(TCE0_OVF_vect)
{
	if(Counters.TimerE0 < MAX_TIMEOUT) {
		Counters.TimerE0++;
	} else {
		Counters.TimerE0 = 0;
	}
}

ISR(TCE1_OVF_vect)
{
	if(Counters.TimerE1 < MAX_TIMEOUT) {
		Counters.TimerE1++;
	} else {
		Counters.TimerE1 = 0;
	}
}

ISR(TCF0_OVF_vect)
{
	if(Counters.TimerF0 < MAX_TIMEOUT) {
		Counters.TimerF0++;
	} else {
		Counters.TimerF0 = 0;
	}
}

ISR(TCF1_OVF_vect)
{
	if(Counters.TimerF1 < MAX_TIMEOUT) {
		Counters.TimerF1++;
	} else {
		Counters.TimerF1 = 0;
	}
}
