/*
 * clock.h
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 */

#ifndef HEADERS_CLOCK_H_
#define HEADERS_CLOCK_H_

#include <avr/io.h>

/*#define OSC_2MHZ	    ((uint8_t)(0x00))
#define OSC_32MHZ	    ((uint8_t)(0x01))
#define OSC_32KHZ	    ((uint8_t)(0x02))
#define OSC_EXT	        ((uint8_t)(0x03))

#define EXT_FRQ_04_2    ((uint8_t)0x00 << 6)
#define EXT_FRQ_2_9     ((uint8_t)0x01 << 6)
#define EXT_FRQ_9_12    ((uint8_t)0x02 << 6)
#define EXT_FRQ_12_16   ((uint8_t)0x03 << 6)

#define PRSC_A_1	    ((uint8_t)(0x00 << 2))
#define PRSC_A_2	    ((uint8_t)(0x01 << 2))
#define PRSC_A_4	    ((uint8_t)(0x03 << 2))
#define PRSC_A_8	    ((uint8_t)(0x05 << 2))

#define PRSC_BC_1	    ((uint8_t)0x00)
#define PRSC_BC_2	    ((uint8_t)0x01)
#define PRSC_BC_4	    ((uint8_t)0x03)

typedef struct {
	uint8_t Oscillator;
    uint8_t ExtFrqRange;
	uint8_t PrScA;		// Not being registered when using 32MHz internal oscillator.
	uint8_t PrScBC;
} Clock_Struct;*/

//void init_clock(Clock_Struct *Clk);

/*
Enables desired oscillator and sets system clock
@oscillator-> choose between internal 2MHz, 32Mhz, 32KHz or external
@freq_range-> only for external oscillator, for all other settings this will be ignored
*/
void init_clock(uint8_t oscillator, uint8_t freq_range);

#endif /* HEADERS_CLOCK_H_ */
