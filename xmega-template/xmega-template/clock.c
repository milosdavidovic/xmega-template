/*
 * clock.c
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 *
 *      This library is intended for ATxmega128A1 family of microcontrollers.
 *      It's purpose is to help the user with clock initialization.
 *
 *      To initialize  controllers clock call function
 *
 *      	initClock('ClockInitializationStruct');
 *
 *      Where 'ClockInitializationStruct' is data structure of type Clock_Struct with the following attributes:
 *
 *      	Oscillator - type of oscillator to be used on the chip:
 *      		* OSC_2MHZ / OSC_32MHZ / OSC_32KHZ / OSC_EXT
 *              If external oscillator is used (OSCT_EXT) then ExtFrqRange attribute must be set to
 *              one of the following value:
 *                  - EXT_FRQ_04_2 for oscillators ranging from 0.4 to 2 MHz
 *                  - EXT_FRQ_2_9 for oscillators ranging from 2 to 9 MHz
 *                  - EXT_FRQ_9_12 for oscillators ranging from 9 to 12 MHz
 *                  - EXT_FRQ_12_16 for oscillators ranging from 12 to 16 MHz
 *
 *      	PrScA - prescaler A for the periphery clock:
 *      		* PRSC_A_1 / PRSC_A_2 / PRSC_A_4 / PRSC_A_8
 *
 *      	PrScBC - prescalers B and C for the periphery clocks:
 *      		* PRSC_BC_1 / PRSC_BC_4
 *
 */

#include "clock.h"

/*void init_clock(Clock_Struct *Clk)
{

    if(Clk->Oscillator == OSC_EXT) {
        OSC.XOSCCTRL |= Clk->ExtFrqRange;
    }
	
	// Enable oscillator. 
	OSC.CTRL = (1 << Clk->Oscillator);

	// Wait for oscillator to stabilize. 
	while( !(OSC.STATUS & (1 << Clk->Oscillator)) );
	
	// Periphery prescalers. 
	CLK.PSCTRL = (Clk->PrScA | Clk->PrScBC);
	
	CCP = CCP_IOREG_gc;
	// Select oscillator as clock source. 
	CLK.CTRL = Clk->Oscillator;
}*/

void init_clock(uint8_t oscillator, uint8_t freq_range)
{
	//if extern oscillator then set frequency range   
	if (oscillator == OSC_XOSCEN_bm)
	{
		OSC_XOSCCTRL = freq_range;
	}
	
	//Enable oscillator
	OSC_CTRL = oscillator;
	
	//Wait for clock stabilization
	while(!(OSC_STATUS & oscillator));
	
	// Selects clock system
	// through change protection mechanism
	if(oscillator == OSC_XOSCEN_bm)
	{
		//External Crystal Oscillator or Clock
		CCP = CCP_IOREG_gc;
		CLK_CTRL = CLK_SCLKSEL_XOSC_gc;
		
		//Disable default clock source (internal 2MHz)
		OSC_CTRL &= ~(1 << 0);
	}
	else if(oscillator == OSC_RC32KEN_bm)
	{
		//Internal 32.768 kHz RC Oscillator
		CCP = CCP_IOREG_gc;
		CLK_CTRL = CLK_SCLKSEL_RC32K_gc;
		
		//Disable default clock source (internal 2MHz)
		OSC_CTRL &= ~(1 << 0);
	}
	else if(oscillator == OSC_RC32MEN_bm)
	{
		//Internal 32 MHz RC Oscillator
		CCP = CCP_IOREG_gc;
		CLK_CTRL = CLK_SCLKSEL_RC32M_gc;
		
		//Disable default clock source (internal 2MHz)
		OSC_CTRL &= ~(1 << 0);
	}
	else if(oscillator == OSC_RC2MEN_bm)
	{
		//Internal 2 MHz RC Oscillator
		CCP = CCP_IOREG_gc;
		CLK_CTRL = CLK_SCLKSEL_RC2M_gc;
	}
}

