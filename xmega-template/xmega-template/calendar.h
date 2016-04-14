/*
 * calendar.h
 *
 * Created: 8/4/2015 10:31:06
 *  Author: marko.tikvic
 */ 


#ifndef CALENDAR_H_
#define CALENDAR_H_

#include <avr/io.h>

typedef struct {
    uint8_t second;         // 1B
    uint8_t minute;         // 1B
    uint8_t hour;           // 1B
    uint8_t day;            // 1B
    uint8_t month;          // 1B
    uint16_t year;          // 2B
    char date_time[23];     // 50B
} Kalendar_S;

void date_time_string_from_unix(uint32_t unix_timestamp, char *dest);
void int_to_string(uint32_t num, char *target);

#endif /* KALENDAR_H_ */