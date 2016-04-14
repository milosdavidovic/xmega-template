/*
 * gsm.h
 *
 * Created: 4/8/2015 3:44:25 PM
 *  Author: milos.davidovic
 */ 


#ifndef GSM_H_
#define GSM_H_

#include "telit_gl865.h"
#include "timers.h"
#include "peripherals.h"


/*! Defining timer counter used  */
#define GSM_DELAY_TIMER_COUNTER 3
#define GSM_STARTUP_DELAY 15000

#define GSM_RETRY_NUMBER_2 2
#define GSM_RETRY_NUMBER_5 5
#define GSM_RETRY_NUMBER_10 10

//gsm states
#define IDLE 0
#define CHECKING_FOR_NETWORK 1
#define SENDING_MESSURMENTS 2
#define SENDING_ALARM 3
#define SYNCING_TIME_WITH_SERVER 4
#define REPORTING_ON_SERVER_REQUEST 5
#define ANSWERING_USER_REQUESTS 6

typedef struct{
	uint8_t gsm_state;//radi u stedljivom modu
	uint8_t operation_attempts_counter;//trenutno je prisutno napajanje
	uint8_t abort_state;
}T_gsm;

void init_gsm();
T_gsm * get_gsm_struct();

void manage_gsm();

void update_gsm_power_status();
void gsm_testing();
void gsm_operation_variables_cleanup();
void gsm_calculate_communication_params_for_telit();
void gsm_start_data_sending();
void gsm_start_time_sync();
uint8_t gsm_ready();
void set_gsm_initial_delay();
uint8_t gsm_delay_expired();

void gsm_init_operation();
void gsm_init_operation_failed_cleanup();
void gsm_init_operation_abort_cleanup();

void gsm_connect_to_gprs_operation();
void gsm_connect_to_gprs_operation_failed_cleanup();
void gsm_connect_to_gprs_operation_abort_cleanup();

void gsm_disconnect_from_gprs_operation();
void gsm_disconnect_from_gprs_operation_failed_cleanup();
void gsm_disconnect_from_gprs_operation_abort_cleanup();

void gsm_disconnect_from_server_operation();
void gsm_disconnect_from_server_operation_failed_cleanup();
void gsm_disconnect_from_server_operation_abort_cleanup();

void gsm_connect_open_socket_operation();
void gsm_connect_open_socket_operation_failed_cleanup();
void gsm_connect_open_socket_operation_abort_cleanup();

void gsm_send_data_socket_operation();
void gsm_send_data_socket_operation_failed_cleanup();
void gsm_send_data_socket_operation_abort_cleanup();

void gsm_send_data_socket_operation_using_logger();
void gsm_send_data_socket_operation_using_logger_failed_cleanup();
void gsm_send_data_socket_operation_using_logger_abort_cleanup();

void gsm_check_network_registration_operation();
void gsm_check_network_registration_operation_failed_cleanup();
void gsm_check_network_registration_operation_abort_cleanup();

void gsm_send_data_socket_operation_time_sync();

void gsm_communicate_with_user();
void gsm_start_comm_with_user();
void gsm_stop_comm_with_user();

#endif /* GSM_H_ */