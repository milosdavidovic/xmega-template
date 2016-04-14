/*
 * timers.h
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 */

#include <avr/io.h>

#ifndef HEADERS_TIMERS_H_
#define HEADERS_TIMERS_H_

#define TIMER_MODE_NORMAL			((uint8_t)0x00)
#define TIMER_MODE_SINGLE_SLOPE_PWM	((uint8_t)0x03)
#define TIMER_MODE_DOUBLE_SLOPE_PWM	((uint8_t)0x05)

#define TIMER_INT_LVL_LOW		    ((uint8_t)0x00)
#define TIMER_INT_LVL_MID		    ((uint8_t)0x01)
#define TIMER_INT_LVL_HIGH	        ((uint8_t)0x02)

#define TIMER_PRSC_1	            ((uint8_t)0x01)
#define TIMER_PRSC_2	            ((uint8_t)0x02)
#define TIMER_PRSC_4	            ((uint8_t)0x03)
#define TIMER_PRSC_8	            ((uint8_t)0x04)
#define TIMER_PRSC_64	            ((uint8_t)0x05)

#define TIMER_C0_TIMEOUT            ((uint8_t)0x00)
#define TIMER_C1_TIMEOUT            ((uint8_t)0x01)
#define TIMER_D0_TIMEOUT            ((uint8_t)0x02)
#define TIMER_D1_TIMEOUT            ((uint8_t)0x03)
#define TIMER_E0_TIMEOUT            ((uint8_t)0x04)
#define TIMER_E1_TIMEOUT            ((uint8_t)0x05)
#define TIMER_F0_TIMEOUT            ((uint8_t)0x06)
#define TIMER_F1_TIMEOUT            ((uint8_t)0x07)

typedef struct {
	uint8_t Mode;
	uint8_t InterruptLvl;
	uint8_t Prescaler;
	uint16_t Period;
} Timer_Struct;

void init_timer_type_0(TC0_t *t, Timer_Struct *ts);
void init_timer_type_1(TC1_t *t, Timer_Struct *ts);

void pause_time(uint32_t p);

uint32_t getTCO_counter(uint8_t counter_number);
uint32_t getTC1_counter(uint8_t counter_number);

void setTCO_counter(uint8_t counter_number, uint32_t timeout_value);
void setTC1_counter(uint8_t counter_number, uint32_t timeout_value);

#endif /* HEADERS_TIMERS_H_ */
