/*
 * telit_gl865.h
 *
 * Created: 22/7/2015 1:06:53 PM
 *  Author: milos.davidovic
 */ 


#ifndef TELIT_GL865_H_
#define TELIT_GL865_H_


#include <avr/pgmspace.h>
#include <avr/io.h>
#include "Usart.h"
#include <util/delay.h>
#include "timers.h"
#include <stdlib.h>
#include <string.h>

//Telit command timeouts
#define TIMEOUT_10 10
#define TIMEOUT_50 50
#define TIMEOUT_100 100
#define TIMEOUT_200 200
#define TIMEOUT_500 500
#define TIMEOUT_1000 1000
#define TIMEOUT_2000 2000
#define TIMEOUT_3000 3000
#define TIMEOUT_5000 5000
#define TIMEOUT_10000 10000

//Telit operation states
#define IDLE 0
#define INITIALIZING 1
#define CONNECTING_TO_GPRS 2
#define DISCONNECTING_FROM_SERVER 3
#define CONNECTING_TO_SERVER 4
#define SENDING_DATA_VIA_GPRS 5
#define DISCONNECTING_FROM_GPRS 6
#define CHECKING_NETWORK_REGISTRATION 7

#define SENDING_SMS 7
#define READING_SMS 8

#define GPRS_ON 1
#define GPRS_OFF 0

//Telit status states
#define IDLE 0
#define RUNNING 1
#define FINISHED 2
#define FAILED 3
//Number of attempts to execut AT command
#define TELIT_MAX_RETRY_NUMBER 5

#define DEVICE_TYPE_SIZE 8
#define DEVICE_ID_SIZE 11
#define MESSAGE_TYPE_SIZE 4
#define NULL_TERMINATOR_SIZE 1
#define DATA_LENGTH_INFO_SIZE 2

typedef struct{
	uint8_t psm;//radi u stedljivom modu
	uint8_t power_present;//trenutno je prisutno napajanje
	uint8_t power_on;//trenutno treba da je ukljucen telit
	uint8_t init_ok;//prosao inicijalizaciju i spreman za koriscenje
	
	uint8_t state;//Current telit operation state
	uint8_t state_status;//Current status of the state
	
	uint8_t waiting_for_answer;		//depricated
	
	uint8_t grps_status;
	uint8_t network_ok;

}T_telit;

typedef struct
{
	uint16_t number_of_bytes;
	uint8_t number_of_logs;
	
}T_comm_param;

uint8_t action_timeout_elapsed();

T_telit * get_telit_struct();
void initTelit();
uint8_t telit_check_power();

//high level functions
void telit_init();
void telit_connect_to_gprs();
void telit_disconnect_from_gprs();
void telit_connect_to_server();
void telit_send_data_throught_socket(char *data);
void telit_send_data_throught_socket_using_logger();
void telit_disconnect_from_server();

//low level functions
void send_telit_test_cmd();
void send_selint_cmd();
void turn_echo_off();
void get_telit_imei();
void set_sms_mem();
void set_sms_text_mode();
void delete_all_sms();
void set_sms_ind();
void set_gprs_settings();
void set_gprs_user();
void set_gprs_pass();
void set_cgq_req();
void set_cgq_min();
void set_telit_full_mode();
void set_auto_network_datetime();
void send_gprs_on();
void send_gprs_off();
void open_server_socket();
void set_open_socket_timeout();
void socket_send_request();
void socket_send_request_using_logger();
void send_data_gprs(char * data_to_send);
void send_data_gprs_using_logger();
void receive_socket_data();
void close_server_socket();
void check_network();
void send_gprs_check();
void telit_check_network_reg();
void send_data_gprs_time_sync();
void socket_send_request_time_sync();
void telit_send_data_throught_socket_time_sync();


uint8_t analyse_telit_answer(const char *  string_to_search);
uint8_t analyse_telit_answer_and_sync_time(const char * string_to_search);
void manage_cmd_timeout();
void manage_cmd_timeout_using_logger();
void cmd_formalities(int timeout_lenght);
void initialise_telit_cmd_variables();

uint8_t manage_telit_response_default(const char *expected_response);
uint8_t manage_telit_response_time_sync(const char *expected_response);
uint8_t manage_telit_response_grps_chk();

uint8_t telit_status(void);

//void telit_gl865_set_number_of_bytes_to_send(uint32_t num_of_bytes);
void telit_gl865_set_last_packet_index(uint32_t last_pack_ind);
void telit_gl865_set_communication_session_info(uint8_t index, uint16_t num_of_bytes, uint16_t num_of_logs);

void clear_data_for_server(uint8_t* data_for_server, uint8_t size);

void   telit_set_server_port(char* port);
char * telit_get_server_port();
void   telit_set_server_ip(char* ip);
char * telit_get_server_ip();
void   telit_set_apn(char* apn);
char * telit_get_apn();
void   telit_set_pin(char* pin);
char * telit_get_pin();
void   telit_set_sms_server1(char* number);
char * telit_get_sms_server1();
void   telit_set_sms_server2(char* number);
char * telit_get_sms_server2();
void   telit_set_imei(char* imei);
char * telit_get_imei();
void   telit_set_pin(char* pin);
char * telit_get_pin();
uint8_t manage_telit_response_pin_state();

void get_pin_state();
void enter_sim_pin();

#endif /* TELIT_GL865_H_ */