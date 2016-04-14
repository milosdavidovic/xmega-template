/*
 * watchdog.h
 *
 * Created: 03-Jul-15 10:03:15 AM
 *  Author: luka
 */ 


#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "avr/io.h"
#include <avr/interrupt.h>

/*
Enable watchdog timer with desired timeout period
Note: Currently supported timeouts are 1s, 2s, 4s and 8s
@timeout-> timeout period in secunds
*/
void watchdog_init(uint8_t timeout);

/*
Reset watchdog timer
Note: This function should be called in the main loop
*/
void watchdog_reset();

void watchdog_enable();
void watchdog_disable();

#endif /* WATCHDOG_H_ */