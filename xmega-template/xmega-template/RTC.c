/*
 * RTC.c
 *
 * Created: 01-Jul-15 09:30:19 AM
 *  Author: luka
 */ 

/*
 * atxmegaRTC.c
 *
 * Created: 25-Jun-15 09:43:25 AM
 *  Author: luka
 */ 

#include "RTC.h"

static volatile uint32_t timestamp = 0;
static volatile uint16_t period = 0;

void RTC_init(uint16_t period_seconds)
{
	period = period_seconds;
	cli(); // global interrupt disable
	
	OSC_XOSCCTRL |= OSC_XOSCSEL1_bm;// 0b00000010; // clock @ 32.768kHz on TOSC (external)
	OSC_CTRL |= OSC_XOSCEN_bm;// 0b00001000; // Enable 32.768kHz oscillator on TOSC  (it will be used by RTC)
	while (!(OSC_STATUS & OSC_XOSCRDY_bm)); // wait for 32.768kHz oscillator on TOSC to be stable
	
	//From atmel documentation
	/*
	The reference clock is typically the 1.024kHz output from a high-accuracy crystal of 32.768kHz, 
	and this is the configuration most optimized for low power consumption
	*/
	CLK_RTCCTRL = CLK_RTCSRC0_bm | CLK_RTCEN_bm;// 0b0000011; // select 1.024kHz from 32.768kHz oscillator on TOSC for clock source and enable it (bit0 set to 1)
	RTC_CTRL = RTC_PRESCALER_DIV1024_gc; // 0b0000111; // set prescale to RTC clock / 1024 (default is 1ms tick. with prescale it's 1s) 
	while (RTC_STATUS & RTC_SYNCBUSY_bm); // wait for RTC and SCK to synchronize
	RTC_INTCTRL |= RTC_OVFINTLVL_HI_gc; //0b00000011; // enable Overflow Interrupt and set it to high level
	PMIC_CTRL |= PMIC_HILVLEN_bm; //0b00000100; //Enable high level interrupts
	RTC_PER = period -1; //set period  //because zero is one cycle, so one is two cycles and so on.
	RTC_CNT = 0; //set compare counter to 0
	
	sei(); // global interrupt enable
	
}

uint32_t RTC_get_time()
{
	return timestamp;
}

void RTC_set_time(uint32_t unix_timestamp)
{
	timestamp = unix_timestamp;
}

uint16_t RTC_get_period()
{
	return period;
}

void RTC_change_period(uint16_t change_period)
{
	while (RTC_STATUS & RTC_SYNCBUSY_bm); // wait for RTC and SCK to synchronize
	period = change_period ;
	RTC_PER = change_period-1; //set period to 0
	RTC_CNT = 0; //set compare counter to 0
}

//Overflow Interrupt (on set period)
ISR(RTC_OVF_vect)
{
	timestamp += period; 
}


