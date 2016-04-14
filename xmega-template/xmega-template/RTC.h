/*
 * RTC.h
 *
 * Created: 01-Jul-15 09:30:44 AM
 *  Author: luka
 */ 


#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>
#include <avr/interrupt.h>

//#define READ_DATA_TIMEOUT   30
//#define SEND_DATA_TIMEOUT   90

//extern volatile uint8_t time_to_read_data;
//extern volatile uint8_t time_to_send_data;

/*
Initialize real time clock
Note: RTC settings are fixed. clock @ 32.768kHz on TOSC and tick of 1s.
*/
void RTC_init(uint16_t period);

/*
Gets Unix timestamp form RTC
*/
uint32_t RTC_get_time();

/*
Sets Unix timestamp of RTC
@unix_timestamp-> number of seconds from January 1st, 1970
*/
void RTC_set_time(uint32_t unix_timestamp);

/*
Gets RTC tick period
*/
uint16_t RTC_get_period();

/*
Change tick period
@change_period-> number of seconds (1-65536)
*/
void RTC_change_period(uint16_t change_period);

#endif /* RTC_H_ */