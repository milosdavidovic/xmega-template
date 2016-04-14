/*
 * watchdog.c
 *
 * Created: 03-Jul-15 10:03:01 AM
 *  Author: luka
 */ 

#include "watchdog.h"

void watchdog_init(uint8_t timeout)
{
	uint8_t watchdog_settings = 0x00;
	
	if(timeout == 1)
	{
		watchdog_settings = WDT_CTRL | WDT_CEN_bm | WDT_ENABLE_bm | WDT_PER_1KCLK_gc;
	}
	else if(timeout == 2)
	{
		watchdog_settings = WDT_CTRL | WDT_CEN_bm | WDT_ENABLE_bm | WDT_PER_2KCLK_gc;
	}
	else if(timeout == 4)
	{
		watchdog_settings = WDT_CTRL | WDT_CEN_bm | WDT_ENABLE_bm | WDT_PER_4KCLK_gc;
	}
	else if(timeout == 8)
	{
		watchdog_settings = WDT_CTRL | WDT_CEN_bm | WDT_ENABLE_bm | WDT_PER_8KCLK_gc;
	}
	
	CCP = CCP_IOREG_gc;
	WDT_CTRL = watchdog_settings;
	while(WDT_STATUS & WDT_SYNCBUSY_bm){}
}

void watchdog_reset()
{
	asm("WDR");
}

void watchdog_enable()
{
	uint8_t enable_mask = WDT_CTRL | WDT_ENABLE_bm | WDT_CEN_bm;
	CCP = CCP_IOREG_gc;
	WDT_CTRL = enable_mask;
}

void watchdog_disable()
{
	uint8_t disable_mask = WDT_CTRL & (~WDT_ENABLE_bm) | WDT_CEN_bm;
	CCP = CCP_IOREG_gc;
	WDT_CTRL = disable_mask;
}

//TODO implementiraj interapt i ubaci error manager
//MISLIM DA XMEGA NEMA MOGUCNOST IZVRSAVANJA INTERRUPT RUTINE ZA WDT
