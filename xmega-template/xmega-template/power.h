/*
 * power.h
 *
 * Created: 23/9/2015 11:18:04 AM
 *  Author: milos.davidovic
 */ 

#ifndef POWER_H_
#define POWER_H_

#include "avr/sleep.h"
#include "RTC.h"

#define LED_TIMEOUT_COUNTER 3

//**** LED MODES ****//
#define LED5050 1


//! Convenience macro for enabling pull-ups on specified pins on any port.
#define __PORT_PULLUP(port, mask) { \
	PORTCFG.MPCMASK = mask ; \
	port.PIN0CTRL = PORT_OPC_PULLUP_gc; \
}

//! Convenience macro for enabling pull-downs on specified pins on any port.
#define __PORT_PULLDOWN(port, mask) { \
	PORTCFG.MPCMASK = mask ; \
	port.PIN0CTRL = PORT_OPC_PULLDOWN_gc; \
}

void DISABLE_JTAG(void);

void DISABLE_GENERAL(void);
void ENABLE_GENERAL(void);

void DISABLE_TIMERS( void);
void ENABLE_TIMERS(void);

void DISABLE_COMMUNICATION(void);
void ENABLE_COMMUNICATION(void);

void DISABLE_ANLG(void);
void ENABLE_ANLG(void);


void gsm_power_on();

void gsm_power_off();

void ultrasound_power_on();

void ultrasound_power_off();

void temp_power_on();

void temp_power_off();

void power_boost_on();

void power_boost_off();

void led_on(uint8_t mode);

#endif /* POWER_H_ */