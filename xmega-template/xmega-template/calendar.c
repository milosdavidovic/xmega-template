/*
 * kalendar.c
 *
 * Created: 8/4/2015 10:30:54
 *  Author: marko.tikvic
 */ 

#include "calendar.h"
#include <math.h>
#include <string.h>

static uint32_t jan_days;
static uint32_t feb_days;
static uint32_t mar_days;
static uint32_t apr_days;
static uint32_t may_days;
static uint32_t jun_days;
static uint32_t jul_days;
static uint32_t avg_days;
static uint32_t sep_days;
static uint32_t oct_days;
static uint32_t nov_days;
static uint32_t dec_days;

static uint32_t sec_in_min = 60;
static uint32_t sec_in_hour = 3600;
static uint32_t sec_in_day = 86400;

static uint32_t sec_in_year = 365*86400 + 5*3600 + 48*60 + 47 ;

static char *zone = "+01";

void date_time_string_from_unix(uint32_t unix_timestamp, char *dest)
{
    *dest = '\0';

    char sec[3];
    char min[3];
    char hour[3];
    char year[5];
    char month[3];
    char day[3];

    uint32_t s = unix_timestamp % sec_in_min;
    uint32_t m = floor((double)((unix_timestamp%sec_in_hour)/sec_in_min));
    uint32_t h = floor((double)((unix_timestamp%sec_in_day)/sec_in_hour));
    uint32_t yy = floor((double)(unix_timestamp/sec_in_year)) + 1970;
    uint32_t mm = 0;
    uint32_t dd = 0;

    jan_days = 31;

    if(!yy%4) feb_days = jan_days + 29;
    else feb_days = jan_days + 28;

    mar_days = feb_days + 31;
    apr_days = mar_days + 30;
    may_days = apr_days + 31;
    jun_days = may_days + 30;
    jul_days = jun_days + 31;
    avg_days = jul_days + 31;
    sep_days = avg_days + 30;
    oct_days = sep_days + 31;
    nov_days = oct_days + 30;
    dec_days = nov_days + 31;

    uint32_t rem = floor((unix_timestamp%sec_in_year)/sec_in_day) + 1;
    if(rem <= jan_days) {
        mm = 1;
        dd = rem;
    } else if(rem <= feb_days) {
        mm = 2;
        dd = rem - jan_days;
    } else if(rem <= mar_days) {
        mm = 3;
        dd = rem - feb_days;
    } else if(rem <= apr_days) {
        mm = 4;
        dd = rem - mar_days;
    } else if(rem <= may_days) {
        mm = 5;
        dd = rem - apr_days;
    } else if(rem <= jun_days) {
        mm = 6;
        dd = rem - may_days;
    } else if(rem <= jul_days) {
        mm = 7;
        dd = rem - jun_days;
    } else if(rem <= avg_days) {
        mm = 8;
        dd = rem - jul_days;
    } else if(rem <= sep_days) {
        mm = 9;
        dd = rem - avg_days;
    } else if(rem <= oct_days) {
        mm = 10;
        dd = rem - sep_days;
    } else if(rem <= nov_days) {
        mm = 11;
        dd = rem - oct_days;
    } else if(rem <= dec_days) {
        mm = 12;
        dd = rem - nov_days;
    }

    int_to_string(s, sec);
    int_to_string(m, min);
    int_to_string(h, hour);
    int_to_string(yy, year);
    int_to_string(mm, month);
    int_to_string(dd, day);

    strcat(dest, year);
    strcat(dest, "/");
    strcat(dest, month);
    strcat(dest, "/");
    strcat(dest, day);
    strcat(dest, " ");
    strcat(dest, hour);
    strcat(dest, ":");
    strcat(dest, min);
    strcat(dest, ":");
    strcat(dest, sec);
    strcat(dest, zone);
}

void int_to_string(uint32_t num, char *target)
{
	uint32_t tmp = 0;
	uint32_t c = 0;
	uint32_t digits = 0;

	while (num >= tmp) {
		digits++;
		tmp = (tmp == 0) ? 10 : tmp * 10;
	}

	for (c = 0; c < digits; c++) {
		if(digits > 1) {
			/* Case: Number has more than one digit. */
			tmp = num % 10;                             /* Get the modulus of the LSD (Lest Significant Digit). */
			target[digits - 1 - c] = tmp + 48;          /* Get the ASCII character for the LSD. Write it in the last required byte of the string */
			target[digits * sizeof(char)] = '\0';       /* Terminate the string. */
			num = (num - tmp) / 10;                     /* Divide number by 10 - move on to the next digit. */
			} else {
			/* Case: Number has one digit and it need to be formated as '0x'. */
			tmp = num % 10;                             /* Get the modulus of the LSD (Lest Significant Digit). */
			target[digits - 1 - c + 1] = tmp + 48;      /* Get the ASCII character for the LSD. Write it in the last required byte of the string */
			target[digits - 1 - c] = 0 + 48;            /* Get the ASCII character for the 0. */
			target[digits * sizeof(char) + 1] = '\0';   /* Terminate the string. */
		}
	}
}
