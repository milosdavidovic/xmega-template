/*
 * AT_COMMANDS.c
 *
 * Created: 7/14/2015 11:39:58
 *  Author: marko.tikvic
 */ 
 
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include "AT_xmega.h"

const char  AT_RES_error_AT[]       PROGMEM = "ERROR\r";

const char  AT_STR_check_AT[]       PROGMEM = "AT\r";                     /* Test AT command. */
const char  AT_RES_check_AT[]       PROGMEM = "OK";
#define     AT_ID_check_AT          ((uint8_t)0)

const char  AT_STR_firmware_v[]     PROGMEM = "AT&V?\r";                /* Check firmware version. */
const char  AT_RES_firmware_v[]     PROGMEM = "AT&V=";
#define     AT_ID_firmware_v        ((uint8_t)1)

const char  AT_STR_company_i[]      PROGMEM = "AT&F?\r";                /* Check company information. */	
const char  AT_RES_company_i[]      PROGMEM = "AT&F= TO-NET DOO\r";
#define     AT_ID_company_i         ((uint8_t)2)

const char  AT_STR_RTC_get[]        PROGMEM = "AT&RTC?\r";              /* Check RTC time. */
const char  AT_RES_RTC_get[]        PROGMEM = "AT&RTC=";
#define     AT_ID_RTC_get           ((uint8_t)3)

const char  AT_STR_RTC_set[]        PROGMEM = "AT&RTC=";                /* Set RTC time. */	
const char  AT_RES_RTC_set[]        PROGMEM = "AT&RTC=";
#define     AT_ID_RTC_set           ((uint8_t)4)

const char  AT_STR_time_get[]       PROGMEM = "AT&TIME?\r";             /* Get time. */
const char  AT_RES_time_get[]       PROGMEM = "AT&TIME=";
#define     AT_ID_time_get          ((uint8_t)5)

const char  AT_STR_telit_time_get[] PROGMEM = "AT&TELIT_TIME?\r";       /* Get telit time. */
const char  AT_RES_telit_time_get[] PROGMEM = "AT&TELIT_TIME=";
#define     AT_ID_telit_time_get    ((uint8_t)6)

const char  AT_STR_telit_time_set[] PROGMEM = "AT&TELIT_TIME=";         /* Set telit time. */
const char  AT_RES_telit_time_set[] PROGMEM = "AT&TELIT_TIME=";
#define     AT_ID_telit_time_set    ((uint8_t)7)

const char  AT_STR_battery_get[]    PROGMEM = "AT&BAT?\r";              /* Check battery alarm. */
const char  AT_RES_battery_get[]    PROGMEM = "AT&BAT=";
#define     AT_ID_battery_get       ((uint8_t)8)

const char  AT_STR_battery_reset[]  PROGMEM = "AT&BAT_RESET\r";         /* Reset battery alarm. */
const char  AT_RES_battery_reset[]  PROGMEM = "BAT_RESET OK!\r";
#define     AT_ID_battery_reset     ((uint8_t)9)

const char  AT_STR_IPP_get[]        PROGMEM = "AT&IPP?\r";              /* Get IP address and port. */
const char  AT_RES_IPP_get[]        PROGMEM = "AT&IPP=";
#define     AT_ID_IPP_get           ((uint8_t)10)

const char  AT_STR_IPP_set[]        PROGMEM = "AT&IPP=";                /* Set IP address and port. */
const char  AT_RES_IPP_set[]        PROGMEM = "AT&IPP=";
#define     AT_ID_IPP_set           ((uint8_t)11)

const char  AT_STR_SMSS1_get[]      PROGMEM = "AT&SMSS_1?\r";           /* Get SMS server number #1. */
const char  AT_RES_SMSS1_get[]      PROGMEM = "AT&SMSS_1=";
#define     AT_ID_SMSS1_get         ((uint8_t)12)

const char  AT_STR_SMSS1_set[]      PROGMEM = "AT&SMSS_1=";             /* Set SMS server number #1. */
const char  AT_RES_SMSS1_set[]      PROGMEM = "AT&SMSS_1=";
#define     AT_ID_SMSS1_set         ((uint8_t)13)

const char  AT_STR_SMSS2_get[]      PROGMEM = "AT&SMSS_2?\r";           /* Get SMS server number #2. */
const char  AT_RES_SMSS2_get[]      PROGMEM = "AT&SMSS_2=";
#define     AT_ID_SMSS2_get         ((uint8_t)14)

const char  AT_STR_SMSS2_set[]      PROGMEM = "AT&SMSS_2?=";            /* Set SMS server number #2. */
const char  AT_RES_SMSS2_set[]      PROGMEM = "AT&SMSS_2=";
#define     AT_ID_SMSS2_set         ((uint8_t)15)

const char  AT_STR_APN_get[]        PROGMEM = "AT&APN?\r";              /* Get APN string. */
const char  AT_RES_APN_get[]        PROGMEM = "AT&APN=";
#define     AT_ID_APN_get           ((uint8_t)16)

const char  AT_STR_APN_set[]        PROGMEM = "AT&APN=";                /* Set APN string. */
const char  AT_RES_APN_set[]        PROGMEM = "AT&APN=";
#define     AT_ID_APN_set           ((uint8_t)17)

const char  AT_STR_serial_get[]     PROGMEM = "AT&SERIAL?\r";           /* Get serial number. */
const char  AT_RES_serial_get[]     PROGMEM = "AT&SERIAL=";
#define     AT_ID_serial_get        ((uint8_t)18)

const char  AT_STR_serial_set[]     PROGMEM = "AT&SERIAL=";             /* Set serial number. */
const char  AT_RES_serial_set[]     PROGMEM = "AT&SERIAL=";
#define     AT_ID_serial_set        ((uint8_t)19)

const char  AT_STR_IMEI_get[]       PROGMEM = "AT&IMEI?\r";             /* Get modem IMEI. */
const char  AT_RES_IMEI_get[]       PROGMEM = "AT&IMEI=";
#define     AT_ID_IMEI_get          ((uint8_t)20)

const char  AT_STR_telit_at[]       PROGMEM = "AT&T_INIT?\r";           /* Get telit response on 'AT' command. */
const char  AT_RES_telit_at[]       PROGMEM = "AT&T_INIT=";
#define     AT_ID_telit_at          ((uint8_t)21)

const char  AT_STR_telit_init[]     PROGMEM = "AT&T_INIT!";             /* Request new telit response on 'AT' command. */
const char  AT_RES_telit_init[]     PROGMEM = "AT&T_INIT=";
#define     AT_ID_telit_init        ((uint8_t)22)

const char  AT_STR_GPRS_get[]       PROGMEM = "AT&T_GRPS?\r";           /* Get telit connection status to GPRS. */
const char  AT_RES_GPRS_get[]       PROGMEM = "AT&GPRS=";
#define     AT_ID_GPRS_get          ((uint8_t)23)

const char  AT_STR_GPRS_set[]       PROGMEM = "AT&GPRS=";               /* Set telit connection status to GPRS. 0 - disconnect / 1 - connect */
const char  AT_RES_GPRS_set[]       PROGMEM = "AT&GPRS=";
#define     AT_ID_GPRS_set          ((uint8_t)24)

const char  AT_STR_server_get[]     PROGMEM = "AT&T_SRV?\r";            /* Get telit connection status to server. */
const char  AT_RES_server_get[]     PROGMEM = "AT&T_SRV=";
#define     AT_ID_server_get        ((uint8_t)25)

const char  AT_STR_server_set[]     PROGMEM = "AT&T_SRV=";              /* Get telit connection status to server. */
const char  AT_RES_server_set[]     PROGMEM = "AT&T_SRV=";
#define     AT_ID_server_set        ((uint8_t)26)

const char  AT_STR_soft_reset[]     PROGMEM = "AT&SOFT_RESET!\r";       /* Reset by software. */
const char  AT_RES_soft_reset[]     PROGMEM = "SOFT_RESET OK!\r";
#define     AT_ID_soft_reset        ((uint8_t)27)

const char  AT_STR_make_log[]  PROGMEM = "AT&MAKE_LOG!\r";          /* Test command for logging data  */
const char  AT_RES_make_log[]  PROGMEM = "MAKE_LOG OK\r";
#define     AT_ID_make_log     ((uint8_t)30)

const char  AT_STR_send_logs[]  PROGMEM = "AT&SEND_LOG!\r";          /* Test command for sending data */
const char  AT_RES_send_logs[]  PROGMEM = "SEND_LOG OK\r";
#define     AT_ID_send_logs     ((uint8_t)31)

const char  AT_STR_sleep[]  PROGMEM = "AT&SLEEP!\r";          /* Test Sleep command */
const char  AT_RES_sleep[]  PROGMEM = "SLEEP OK\r";
#define     AT_ID_sleep     ((uint8_t)32)

const char  AT_STR_gsm_abort[]  PROGMEM = "AT&GSM_ABORT!\r";          /*Test Abort gsm operation command */
const char  AT_RES_gsm_abort[]  PROGMEM = "GSM ABORT STARTED\r";
#define     AT_ID_gsm_abort     ((uint8_t)33)

const char  AT_STR_measurement_period_set[]  PROGMEM = "AT&MSP=";          /* Set measurement period in seconds*/
const char  AT_RES_measurement_period_set[]  PROGMEM = "AT&MSP=";
#define     AT_ID_measurement_period_set     ((uint8_t)34)

const char  AT_STR_measurement_period_get[]  PROGMEM = "AT&MSP?\r";          /* Get measurement period in seconds*/
const char  AT_RES_measurement_period_get[]  PROGMEM = "AT&MSP=";
#define     AT_ID_measurement_period_get     ((uint8_t)35)

const char  AT_STR_data_send_period_set[]  PROGMEM = "AT&SCP=";          /* Set data send period in seconds*/
const char  AT_RES_data_send_period_set[]  PROGMEM = "AT&SCP=";
#define     AT_ID_data_send_period_set     ((uint8_t)36)

const char  AT_STR_data_send_period_get[]  PROGMEM = "AT&SCP?\r";          /* Get data send period in seconds*/
const char  AT_RES_data_send_period_get[]  PROGMEM = "AT&SCP=";
#define     AT_ID_data_send_period_get     ((uint8_t)37)

const char  AT_STR_config_on[]  PROGMEM = "AT&CONFIG_ON!\r";          /*Enable configuring state*/
const char  AT_RES_config_on[]  PROGMEM = "CONFIG_ON OK!";
#define     AT_ID_config_on     ((uint8_t)38)

const char  AT_STR_config_off[]  PROGMEM = "AT&CONFIG_OFF!\r";          /*Enable configuring state*/
const char  AT_RES_config_off[]  PROGMEM = "CONFIG_OFF OK!";
#define     AT_ID_config_off     ((uint8_t)39)

const char  AT_STR_pin_set[]    PROGMEM = "AT&PIN=";                /* Set APN string. */
const char  AT_RES_pin_set[]    PROGMEM = "AT&PIN=";
#define     AT_ID_pin_set		((uint8_t)40)

const char  AT_STR_read_sensors[]   PROGMEM = "AT&READ_SENSORS!\r";                /* Read data from sensors */
const char  AT_RES_read_sensors[]   PROGMEM = "AT&READ_SENSORS OK\r";
#define     AT_ID_read_sensors      ((uint8_t)41)

const char  AT_STR_sleep_for[]        PROGMEM = "AT&SLEEP=";                /* Sleep for given number of seconds */
const char  AT_RES_sleep_for[]        PROGMEM = "AT&SLEEP OK\r";
#define     AT_ID_sleep_for           ((uint8_t)42)

const char  AT_STR_per_on[]   PROGMEM = "AT&PER_ON!\r";                /* Turn on all peripherals */
const char  AT_RES_per_on[]   PROGMEM = "AT&PER_ON OK\r";
#define     AT_ID_per_on      ((uint8_t)43)

const char  AT_STR_per_off[]   PROGMEM = "AT&PER_OFF!\r";                /* Turn off all peripherals */
const char  AT_RES_per_off[]   PROGMEM = "AT&PER_OFF OK\r";
#define     AT_ID_per_off      ((uint8_t)44)

const char  AT_STR_clear_logs[]   PROGMEM = "AT&CLEAR_LOGS!\r";                /* Delete all logs */
const char  AT_RES_clear_logs[]   PROGMEM = "AT&CLEAR_LOGS OK\r";
#define     AT_ID_clear_logs      ((uint8_t)45)

const char  AT_STR_telit_mode_on[]   PROGMEM = "AT&TELIT_MODE_ON!\r";                /* Enter telit mode */
const char  AT_RES_telit_mode_on[]   PROGMEM = "AT&TELIT_MODE ON OK\r";
#define     AT_ID_telit_mode_on      ((uint8_t)46)

const char  AT_STR_telit_mode_off[]   PROGMEM = "AT&TELIT_MODE_OFF!\r";                /* Exit telit mode */
const char  AT_RES_telit_mode_off[]   PROGMEM = "AT&TELIT_MODE OFF OK\r";
#define     AT_ID_telit_mode_off      ((uint8_t)47)

static char *at_buffer;
//static char test[30] = {'\0'};

void at_init(char uart_port)
{
    switch(uart_port) {
        case 'c':
            AT_UART_send_str = &USARTC0_send_string;
            AT_UART_clear_buffer = &USARTC0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTC0_send_new_line;
            at_buffer = (char*)usartc0_receive_buffer;
            break;   
        case 'C':
            AT_UART_send_str = &USARTC0_send_string;
            AT_UART_clear_buffer = &USARTC0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTC0_send_new_line;
            at_buffer = (char*)usartc0_receive_buffer;
            break;
        case 'd':
            AT_UART_send_str = &USARTD0_send_string;
            AT_UART_clear_buffer = &USARTD0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTD0_send_new_line;
            at_buffer = (char*)usartd0_receive_buffer;
            break;
        case 'D':
            AT_UART_send_str = &USARTD0_send_string;
            AT_UART_clear_buffer = &USARTD0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTD0_send_new_line;
            at_buffer = (char*)usartd0_receive_buffer;
            break;
        case 'e':
            AT_UART_send_str = &USARTE0_send_string;
            AT_UART_clear_buffer = &USARTE0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTE0_send_new_line;
            at_buffer = (char*)usarte0_receive_buffer;
            break;
        case 'E':
            AT_UART_send_str = &USARTE0_send_string;
            AT_UART_clear_buffer = &USARTE0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTE0_send_new_line;
            at_buffer = (char*)usarte0_receive_buffer;
            break;
        case 'f':
            AT_UART_send_str = &USARTF0_send_string;
            AT_UART_clear_buffer = &USARTF0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTF0_send_new_line;
            at_buffer = (char*)usartf0_receive_buffer;
            break;
        case 'F':
            AT_UART_send_str = &USARTF0_send_string;
            AT_UART_clear_buffer = &USARTF0_clear_receiver_buffer;
			AT_UART_send_new_line = &USARTF0_send_new_line;
            at_buffer = (char*)usartf0_receive_buffer;
            break;
    }
}

int at_compare()
{
    if(!strcasecmp_P(at_buffer, AT_STR_check_AT))					return AT_ID_check_AT;

    if(!strcasecmp_P(at_buffer, AT_STR_firmware_v))					return AT_ID_firmware_v;

    if(!strcasecmp_P(at_buffer, AT_STR_company_i))					return AT_ID_company_i;

    if(!strcasecmp_P(at_buffer, AT_STR_RTC_get))					return AT_ID_RTC_get;
    
    if(strstr_P(at_buffer, AT_STR_RTC_set) != NULL)					return AT_ID_RTC_set;

    if(!strcasecmp_P(at_buffer, AT_STR_time_get))					return AT_ID_time_get;

    if(!strcasecmp_P(at_buffer, AT_STR_telit_time_get))				return AT_ID_telit_time_get;

    if(strstr_P(at_buffer, AT_STR_telit_time_set) != NULL)			return AT_ID_telit_time_set;

    if(!strcasecmp_P(at_buffer, AT_STR_battery_get))				return AT_ID_battery_get;

    if(!strcasecmp_P(at_buffer, AT_STR_battery_reset))				return AT_ID_battery_reset;

    if(!strcasecmp_P(at_buffer, AT_STR_IPP_get))					return AT_ID_IPP_get;

    if(strstr_P(at_buffer, AT_STR_IPP_set) != NULL)					return AT_ID_IPP_set;

    if(!strcasecmp_P(at_buffer, AT_STR_SMSS1_get))					return AT_ID_SMSS1_get;

    if(strstr_P(at_buffer, AT_STR_SMSS1_set) != NULL)				return AT_ID_SMSS1_set;

    if(!strcasecmp_P(at_buffer, AT_STR_SMSS2_get))					return AT_ID_SMSS2_get;

    if(strstr_P(at_buffer, AT_STR_SMSS2_set) != NULL)				return AT_ID_SMSS2_set;

    if(!strcasecmp_P(at_buffer, AT_STR_APN_get))					return AT_ID_APN_get;

    if(strstr_P(at_buffer, AT_STR_APN_set) != NULL)					return AT_ID_APN_set;

    if(!strcasecmp_P(at_buffer, AT_STR_serial_get))					return AT_ID_serial_get;

    if(strstr_P(at_buffer, AT_STR_serial_set) != NULL)				return AT_ID_serial_set;

    if(!strcasecmp_P(at_buffer, AT_STR_IMEI_get))					return AT_ID_IMEI_get;

    if(!strcasecmp_P(at_buffer, AT_STR_telit_at))					return AT_ID_telit_at;

    if(!strcasecmp_P(at_buffer, AT_STR_telit_init))					return AT_ID_telit_init;

    if(!strcasecmp_P(at_buffer, AT_STR_GPRS_get))					return AT_ID_GPRS_get;

    if(strstr_P(at_buffer, AT_STR_GPRS_set) != NULL)				return AT_ID_GPRS_set;

    if(!strcasecmp_P(at_buffer, AT_STR_server_get))					return AT_ID_server_get;

    if(strstr_P(at_buffer, AT_STR_serial_set) != NULL)				return AT_ID_server_set;

    if(!strcasecmp_P(at_buffer, AT_STR_soft_reset))					return AT_ID_soft_reset;
	
	if(!strcasecmp_P(at_buffer, AT_STR_make_log))					return AT_ID_make_log;
	
	if(!strcasecmp_P(at_buffer, AT_STR_send_logs))					return AT_ID_send_logs;
	
	if(!strcasecmp_P(at_buffer, AT_STR_sleep))						return AT_ID_sleep;
	
	if(!strcasecmp_P(at_buffer, AT_STR_gsm_abort))					return AT_ID_gsm_abort;
	
	if(strstr_P(at_buffer, AT_STR_measurement_period_set) != NULL)	return AT_ID_measurement_period_set;
	
	if(!strcasecmp_P(at_buffer, AT_STR_measurement_period_get))		return AT_ID_measurement_period_get;
	
	if(strstr_P(at_buffer, AT_STR_data_send_period_set) != NULL)	return AT_ID_data_send_period_set;
	
	if(!strcasecmp_P(at_buffer, AT_STR_data_send_period_get))		return AT_ID_data_send_period_get;
	
	if(!strcasecmp_P(at_buffer, AT_STR_config_on))					return AT_ID_config_on;
	 
	if(!strcasecmp_P(at_buffer, AT_STR_config_off))					return AT_ID_config_off;
	
	if(strstr_P(at_buffer, AT_STR_pin_set) != NULL)					return AT_ID_pin_set;
	
	if(!strcasecmp_P(at_buffer, AT_STR_read_sensors))				return AT_ID_read_sensors;
	
	if(strstr_P(at_buffer, AT_STR_sleep_for) != NULL)				return AT_ID_sleep_for;
	
	if(!strcasecmp_P(at_buffer, AT_STR_per_on))						return AT_ID_per_on;
	
	if(!strcasecmp_P(at_buffer, AT_STR_per_off))					return AT_ID_per_off;
	
	if(!strcasecmp_P(at_buffer, AT_STR_clear_logs))					return AT_ID_clear_logs;
	
	if(!strcasecmp_P(at_buffer, AT_STR_telit_mode_on))					return AT_ID_telit_mode_on;
	
	if(!strcasecmp_P(at_buffer, AT_STR_telit_mode_off))					return AT_ID_telit_mode_off;
	
    return ERR_VAL;
}

void at_handle(int id)
{
    char t1[50] = {0x00};
    char t2[50] = {0x00};
	char *temp;

	switch(id)
	{
		case AT_ID_serial_get:
			AT_UART_send_str("\n\rSERIAL GET\n\r");
			strcpy_P(t1, (char*)AT_RES_serial_get);
			AT_UART_send_str(t1);
			AT_UART_send_str(get_metering_station_id());
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_serial_set:
			AT_UART_send_str("\n\rSERIAL SET\n\r");
			strcpy(t1, at_buffer + strlen_P(AT_STR_serial_set));
			memcpy(t2, t1, strlen(t1)-1);
			if(strlen(t2) == 11)
			{
				set_metering_station_id(t2);
				ee_set_serial(t2);
				ee_set_serial_crc(CRC_calculte_checksum((uint8_t*)t2, strlen(t2)));
				AT_UART_send_str(t2);
			}
			else
			{
				AT_UART_send_str("\n\rserial size error\n\r");
			}
			
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
    	case AT_ID_check_AT:
            AT_UART_send_str("\n\r");
            strcpy_P(t1, (char*)AT_RES_check_AT);
            AT_UART_send_str(t1);
            AT_UART_clear_buffer();
			AT_UART_send_new_line();
    	    break;
			
    	case AT_ID_firmware_v:
            strcpy_P(t1, (char*)AT_RES_firmware_v);
            AT_UART_send_str(t1);
            AT_UART_send_str(FIRMWARE_VERSION);
            AT_UART_clear_buffer();
			AT_UART_send_new_line();
    	    break;
			
    	case AT_ID_RTC_get:
            strcpy_P(t1, (char*)AT_RES_RTC_get);
            AT_UART_send_str(t1);
            ltoa(RTC_get_time(),t1,10);
            AT_UART_send_str(t1);
            AT_UART_clear_buffer();
			AT_UART_send_new_line();
    	    break;
			
    	case AT_ID_RTC_set:
            strcpy_P(t1, (char*)AT_RES_RTC_set);
            strcpy(t2, at_buffer + strlen_P(AT_STR_RTC_set));
            uint32_t temp_timestamp = (uint32_t)(atol(t2));
            RTC_set_time(temp_timestamp);
            AT_UART_send_str(t1);
			AT_UART_send_str(t2);
            AT_UART_clear_buffer();
			AT_UART_send_new_line();
		    break;
			
        case AT_ID_IPP_get:
            AT_UART_send_str("\n\rIPP GET\n\r");
            strcpy_P(t1, (char*)AT_RES_IPP_get);
			strcat(t1, telit_get_server_ip());
			strcat(t1, ":");
			strcat(t1, telit_get_server_port());
            //ee_get_ipp(t2, 20);
            //strcat(t1, t2);
            AT_UART_send_str(t1);
            AT_UART_clear_buffer();
			AT_UART_send_new_line();
            break;
			
        case AT_ID_IPP_set:
            AT_UART_send_str("\n\rIPP SET\n\r");
            strcpy(t1, at_buffer + strlen_P(AT_STR_IPP_set));
            //ee_set_ipp(t1);
			temp = strchr(t1,':');
			//strcpy(t2, temp+1);
			memcpy(t2, (temp+1), strlen((temp+1))-1);
			telit_set_server_port(t2);
			ee_set_port(t2);
			ee_set_port_crc(CRC_calculte_checksum((uint8_t*)t2, strlen(t2)));
			strncpy(t2,t1, (strlen(t1)-strlen(t2)-2));
			telit_set_server_ip(t2);
			ee_set_ip(t2);
			ee_set_ip_crc(CRC_calculte_checksum((uint8_t*)t2, strlen(t2)));
            AT_UART_send_str(t1);
            AT_UART_clear_buffer();
			AT_UART_send_new_line();
            break;
			
		case AT_ID_APN_set:
			AT_UART_send_str("\n\rAPN SET\n\r");
			strcpy(t1, at_buffer + strlen_P(AT_RES_APN_set));
			memcpy(t2, t1, strlen(t1)-1);
			telit_set_apn(t2);
			ee_set_apn(t2);
			ee_set_apn_crc(CRC_calculte_checksum((uint8_t*)t2, strlen(t2)));
			AT_UART_send_str(t2);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_pin_set:
		AT_UART_send_str("\n\rPIN SET\n\r");
			strcpy(t1, at_buffer + strlen_P(AT_RES_APN_set));
			memcpy(t2, t1, strlen(t1)-1);
			telit_set_pin(t2);
			ee_set_sim_pin(t2);
			ee_set_sim_pin_crc(CRC_calculte_checksum((uint8_t*)t2, strlen(t2)));
			AT_UART_send_str(t2);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_APN_get:
			AT_UART_send_str("\n\rAPN GET\n\r");
			strcpy_P(t1, (char*)AT_RES_APN_get);
			strcat(t1, telit_get_apn());
			AT_UART_send_str(t1);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_make_log:
			AT_UART_send_str("\n\rMAKE LOG\n\r");
			//time_to_log_data = TRUE;
			log_data();
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_send_logs:
			AT_UART_send_str("\n\rSEND LOG\n\r");
			gsm_start_data_sending();
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_sleep:
			AT_UART_send_str("\n\rSLEEP\n\r");
			should_sleep = TRUE;
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_gsm_abort:
			get_gsm_struct()->abort_state=1;
			AT_UART_send_str("\n\r");
			strcpy_P(t1, (char*)AT_RES_gsm_abort);
			AT_UART_send_str(t1);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_measurement_period_set:
			strcpy_P(t1, (char*)AT_RES_measurement_period_set);
			strcpy(t2, at_buffer + strlen_P(AT_STR_measurement_period_set));
			uint32_t temp_msp = (uint32_t)(atol(t2));
			ee_set_msp(temp_msp);
			ee_set_msp_crc(CRC_calculte_checksum((uint8_t*)&temp_msp, sizeof(temp_msp)));
			data_measurement_period = temp_msp;
			AT_UART_send_str(t1);
			AT_UART_send_str(t2);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_measurement_period_get:
			strcpy_P(t1, (char*)AT_RES_measurement_period_get);
			ltoa(data_measurement_period, t2, 10);
			AT_UART_send_str(t1);
			AT_UART_send_str(t2);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_data_send_period_set:
			strcpy_P(t1, (char*)AT_RES_data_send_period_set);
			strcpy(t2, at_buffer + strlen_P(AT_STR_data_send_period_set));
			uint32_t temp_scp = (uint32_t)(atol(t2));
			ee_set_scp(temp_scp);
			ee_set_scp_crc(CRC_calculte_checksum((uint8_t*)&temp_scp, sizeof(temp_scp)));
			data_sending_period = temp_scp;
			AT_UART_send_str(t1);
			AT_UART_send_str(t2);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_data_send_period_get:
			strcpy_P(t1, (char*)AT_RES_measurement_period_get);
			ltoa(data_sending_period, t2, 10);
			AT_UART_send_str(t1);
			AT_UART_send_str(t2);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_config_on:
			strcpy_P(t1, (char*)AT_RES_config_on);
			AT_UART_send_str(t1);
			config_mod_enabled = 1;
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_config_off:
			strcpy_P(t1, (char*)AT_RES_config_off);
			AT_UART_send_str(t1);
			config_mod_enabled = 0;
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_read_sensors:
			strcpy_P(t1, (char*)AT_RES_read_sensors);
			AT_UART_send_str(t1);
			wake_up_for_data_collect();
			read_data();
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_sleep_for:
			strcpy_P(t1, (char*)AT_RES_sleep_for);
			strcpy(t2, at_buffer + strlen_P(AT_STR_sleep_for));
			uint16_t seconds = (uint16_t)(atoi(t2));
			AT_UART_send_str(t1);
			AT_UART_send_str(t2);
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			WM_Sleep(seconds);
			AT_UART_send_str("\nAwake. Please continue...\n");
			break;
			
		case AT_ID_per_on:
			strcpy_P(t1, (char*)AT_RES_per_on);
			AT_UART_send_str(t1);
			gsm_power_on();
			ultrasound_power_on();
			temp_power_on();
			power_boost_on();
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_per_off:
			strcpy_P(t1, (char*)AT_RES_per_off);
			AT_UART_send_str(t1);
			gsm_power_off();
			ultrasound_power_off();
			temp_power_off();
			power_boost_off();
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_clear_logs:
			strcpy_P(t1, (char*)AT_RES_clear_logs);
			AT_UART_send_str(t1);
			logger_reset_params_and_logs();
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
		
		case AT_ID_telit_mode_on:
			strcpy_P(t1, (char*)AT_RES_telit_mode_on);
			AT_UART_send_str(t1);
			AT_UART_send_str("\nAll communication is now rerouted to telit modem.\n");
			//gsm_start_comm_with_user();
			telit_test_mode_enabled = 1;
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
		case AT_ID_telit_mode_off:
			strcpy_P(t1, (char*)AT_RES_telit_mode_off);
			AT_UART_send_str(t1);
			//gsm_stop_comm_with_user();
			telit_test_mode_enabled = 0;
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
			
    	default:
    		// An error has occurred.
			AT_UART_send_str("\n\r");
			strcpy_P(t1, (char*)AT_RES_error_AT);
			AT_UART_send_str(t1);
            AT_UART_clear_buffer();
			AT_UART_send_new_line();
    	    break;
	}
}

//ovi special su samo da bi mogli da izadjemo iz telit moda
int special_at_compare()
{
	if(!strcasecmp_P(at_buffer, AT_STR_telit_mode_off))					return AT_ID_telit_mode_off;
	
	return ERR_VAL;
}

void special_at_handle(int id)
{
	char t1[50] = {0x00};
	char t2[50] = {0x00};
	char *temp;
	
	switch(id)
	{
		case AT_ID_telit_mode_off:
			strcpy_P(t1, (char*)AT_RES_telit_mode_off);
			AT_UART_send_str(t1);
			//gsm_stop_comm_with_user();
			telit_test_mode_enabled = 0;
			AT_UART_clear_buffer();
			AT_UART_send_new_line();
			break;
	}
}

