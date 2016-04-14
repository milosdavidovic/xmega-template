/*
 * gsm.c
 *
 * Created: 4/8/2015 3:44:14 PM
 *  Author: milos.davidovic
 */ 
#include "gsm.h"

#define MTU 1500 //Maximum Transmission Unit  for Ethernet is 1500 bytes
#define MAX_HEADER_SIZE 100
#define MTU_GSM (MTU - MAX_HEADER_SIZE)	// MTU-100bytes for header 

const char HELLO_SERVER [] PROGMEM = "HELLO FROM DJUBROV!\r\n";

const char SENDING_DATA_FAILED_MSG [] PROGMEM = "GSM send data through socket operation failed, data sending canceled.\n";
const char DISCONNECTING_FROM_GPRS_FAILED_MSG [] PROGMEM = "GSM disconnect from gprs operation failed, data sending canceled.\n";
const char FATAL_ERROR_MSG [] PROGMEM = "FATAL ERROR!\n";
const char RETRYING_MSG [] PROGMEM = "Retrying...\n";
const char INIT_FAILED_MSG [] PROGMEM = "Initialization failed.\n";
const char CONNECTING_FAILED_MSG [] PROGMEM = "Connecting failed.\n";
const char CONNECTING_TO_GPRS_FAILED_MSG [] PROGMEM = "GSM connect to gprs operation failed, data sending canceled.\n";
const char CHECKING_NET_REG_FAILED_MSG [] PROGMEM = "GSM checking network registration operation failed.\n";
const char CONNECTING_TO_SOCKET_FAILED_MSG [] PROGMEM = "Connecting to server failed.\n";
const char CONNECTING_TO_SOCKET_OPERATION_FAILED_MSG [] PROGMEM = "GSM connect to socket operation failed, data sending canceled.\n";
const char DISCONNECTING_FAILED_MSG [] PROGMEM = "Disconnecting failed.\n";

T_telit *telit_gl865_gsm;
static T_gsm gsm;

//Initialize gsm variables
void init_gsm()
{
	//init telit GSM
	telit_gl865_gsm = get_telit_struct();
	initTelit();
	gsm.gsm_state = IDLE;
	gsm.operation_attempts_counter=0;
	gsm.abort_state = 0;
}

/*
 Operation delay timeout function for gsm module.
*/
uint8_t gsm_delay_expired()
{
	if (getTCO_counter(GSM_DELAY_TIMER_COUNTER))
	{
		return 0;
	} 
	else
	{
		return 1;
	}
}

void set_gsm_initial_delay()
{
	setTCO_counter(GSM_DELAY_TIMER_COUNTER, GSM_STARTUP_DELAY);
}

void gsm_start_data_sending()
{
	gsm.gsm_state = SENDING_MESSURMENTS;
}

void gsm_start_time_sync()
{
	gsm.gsm_state = SYNCING_TIME_WITH_SERVER;
}

void gsm_start_comm_with_user()
{
	gsm.gsm_state = ANSWERING_USER_REQUESTS;
}

void gsm_stop_comm_with_user()
{
	gsm.gsm_state = IDLE;
}

T_gsm * get_gsm_struct()
{
	return &gsm;
}


uint8_t gsm_ready()
{
	if (gsm.gsm_state == IDLE && telit_gl865_gsm->init_ok && telit_gl865_gsm->network_ok)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// WM10 Main process - managing gsm operations
void manage_gsm()
{
	//Checking if gsm module start-up delay has expired
	if (!gsm_delay_expired())
	{
		return;
	}
	
	update_gsm_power_status(); //checking power supply
		
	//if power off
	if (!(telit_gl865_gsm->power_on))
	{
		telit_gl865_gsm->init_ok = 0;

		//Clean up in case of power failiture during telit operation execution
		if (telit_gl865_gsm->state_status != IDLE)
		{	
			telit_gl865_gsm->state_status= IDLE;
		}	
	}
	
	//else if not initalised
	else if (!(telit_gl865_gsm->init_ok))
	{
		gsm_init_operation();
	}
	else if (!(telit_gl865_gsm->network_ok))
	{
		gsm_check_network_registration_operation();
	}
	else
	{
	//all ok, manage gsm states
		switch(gsm.gsm_state)
		{
			case IDLE:
			{
				
			}break;	
			
			
			case SYNCING_TIME_WITH_SERVER:
			{
				switch(telit_gl865_gsm->state)
					{
						case IDLE:
						{
							telit_gl865_gsm->state = CONNECTING_TO_GPRS;
						}
						case CONNECTING_TO_GPRS:
						{
							/*  
								This operation will try to execute 3 times before failing. 
								Failing leads to telit re-initialisation
								Finishing leads to telit CONNECTING_TO_SERVER state	
							*/
							gsm_connect_to_gprs_operation();
						}break;
						case CONNECTING_TO_SERVER:
						{
							/*  
								This operation will try to execute 3 times before failing.
								Failing leads to telit re-initialisation
								Finishing leads to telit SENDING_DATA_VIA_GPRS state
							*/
							gsm_connect_open_socket_operation();
						}break;
						case SENDING_DATA_VIA_GPRS:
						{
							/*  
								This operation will try to execute only once 
								Failing leads to DISCONNECTING_FROM_SERVER
							*/
							gsm_send_data_socket_operation_time_sync();
						}break;
						case DISCONNECTING_FROM_SERVER:
						{
							/*  
								This operation will try to execute 3 times before failing. 
								Failing leads to telit re-initialisation
								Finishing leads to telit IDLE state
							*/
							gsm_disconnect_from_server_operation();	
						}break;
						case DISCONNECTING_FROM_GPRS:
						{
							/*  
								This operation will try to execute 3 times before failing. 
								Failing leads to telit re-initialisation
								Finishing leads to telit IDLE state
							*/
							gsm_disconnect_from_gprs_operation();
						}break;
					}
				
			}break;
			
			
			case SENDING_MESSURMENTS:
			{
				//case SMS
					
					
				//case GPRS
					switch(telit_gl865_gsm->state)
					{
						case IDLE:
						{
							//calculate number of repeats, bytes, logs,...
							gsm_calculate_communication_params_for_telit();
							// telit state is IDLE, so begin with CONNECTING_TO_GPRS
							telit_gl865_gsm->state = CONNECTING_TO_GPRS;
						}
						case CONNECTING_TO_GPRS:
						{
							/*  
								This operation will try to execute 3 times before failing. 
								Failing leads to telit re-initialisation
								Finishing leads to telit CONNECTING_TO_SERVER state	
							*/
							gsm_connect_to_gprs_operation();
						}break;
						case CONNECTING_TO_SERVER:
						{
							/*  
								This operation will try to execute 3 times before failing.
								Failing leads to telit re-initialisation
								Finishing leads to telit SENDING_DATA_VIA_GPRS state
							*/
							gsm_connect_open_socket_operation();
						}break;
						case SENDING_DATA_VIA_GPRS:
						{
							/*  
								This operation will try to execute only once 
								Failing leads to DISCONNECTING_FROM_SERVER
							*/
							gsm_send_data_socket_operation_using_logger();
						}break;
						case DISCONNECTING_FROM_SERVER:
						{
							/*  
								This operation will try to execute 3 times before failing. 
								Failing leads to telit re-initialisation
								Finishing leads to telit IDLE state
							*/
							gsm_disconnect_from_server_operation();	
						}break;
						case DISCONNECTING_FROM_GPRS:
						{
							/*  
								This operation will try to execute 3 times before failing. 
								Failing leads to telit re-initialisation
								Finishing leads to telit IDLE state
							*/
							gsm_disconnect_from_gprs_operation();
						}break;
					}
			}break;
			case REPORTING_ON_SERVER_REQUEST:
			{
					
			}break;
			
			case ANSWERING_USER_REQUESTS:
			{
				//gsm_communicate_with_user();
			}break;
		}
	}
}

void gsm_communicate_with_user()
{
	if(USARTE0_get_receiver_status())
	{
		USARTD0_send_string(USARTE0_get_received_message());
		USARTE0_clear_receiver_buffer();
	}
	if(USARTD0_get_receiver_status())
	{
		USARTE0_send_string(USARTD0_get_received_message());
		USARTD0_clear_receiver_buffer();
	}
}

/*
Maximum number of bytes that could be sent to server in one go is around 1500. If amount of data
that should be sent to server is bigger then 1500 bytes then data must be split up in packets.
This function calculates number of packets, number of bytes in each packet and number of logs in 
each packet and stores those informations in array "communication_session_info". Every element of
that array is struct "T_comm_param" which has two members: number of bytes and number of logs.
*/
void gsm_calculate_communication_params_for_telit()
{
	uint32_t number_of_bytes_to_send; // total number of bytes to send
	uint32_t number_of_bytes_to_send_per_packet; // number of bytes in a packet
	uint32_t number_of_logs_to_send; // total number of logs to send
	uint32_t number_of_logs_to_send_per_packet; // // number of logs in a packet
	uint32_t current_packet_index; //
	uint32_t last_sent_log_id; // id of last sent log (from logger)
	uint8_t log_size; // size of a log (from logger)
	uint16_t number_of_repeats; // counter
	uint16_t max_number_of_bytes_per_packet; // number of bytes in a packet (used when logs have fixed size)
	uint16_t max_number_of_logs_per_packet; // number of logs in a packet (used when logs have fixed size)
	uint16_t number_of_left_bytes; // counter
	uint16_t number_of_left_logs; // counter
	
	// if logs have fixed size
	if (logger_get_is_log_size_fixed()) 
	{
		// get total number of logs that should be sent
		number_of_bytes_to_send = logger_get_number_of_bytes_to_send();
		
		// get log size (same for all logs)
		log_size = logger_get_max_log_size(); 
		
		// calculate maximum number of bytes that can fit in one packet based on log size 
		max_number_of_bytes_per_packet = (uint16_t)(MTU_GSM/log_size) * log_size;
		
		// calculate maximum number of logs that can fit in one packet based on log size
		max_number_of_logs_per_packet = (uint16_t)(MTU_GSM/log_size);
		
		// calculate number of fully populated packet
		number_of_repeats = (uint16_t)(number_of_bytes_to_send / max_number_of_bytes_per_packet);
		
		// calculate number of remaining bytes, these will be put in last packet
		number_of_left_bytes = number_of_bytes_to_send % max_number_of_bytes_per_packet;
		
		// calculate number of remaining logs, these will be put in last packet
		number_of_left_logs = number_of_left_bytes / log_size;
		
		// populate "communication_session_info" array with full packets
		for(uint16_t i=0;i<number_of_repeats;i++)
		{
			telit_gl865_set_communication_session_info(i, max_number_of_bytes_per_packet,max_number_of_logs_per_packet);
		}
		
		if(number_of_left_logs > 0)
		{
			// add last packet to "communication_session_info" array
			telit_gl865_set_communication_session_info(number_of_repeats, number_of_left_bytes,number_of_left_logs);
		}
		
		// set number of packets to sent in telit lib
		telit_gl865_set_last_packet_index(number_of_repeats);
	}
	// if logs do not have fixed size calculations are little bit more complicated
	else
	{
		//params reset
		number_of_bytes_to_send_per_packet = 0;
		number_of_logs_to_send_per_packet = 0;
		current_packet_index = 0;
		
		// get total number of logs that should be sent
		number_of_logs_to_send = logger_get_number_of_logs_to_send();
		
		// get id of last sent log
		last_sent_log_id = logger_get_last_sent_log_id();
		
		for (uint32_t i=0; i<number_of_logs_to_send; i++) 
		{
			// get log size (different for each log)
			log_size = logger_get_log_size_by_id( last_sent_log_id + i );
			
			// if current log can fit in current packet
			if ( number_of_bytes_to_send_per_packet + log_size < MTU_GSM )
			{
				// increment the number of bytes to send by the size of the next log that can fit inside this packet
				number_of_bytes_to_send_per_packet += log_size;	
				
				// increment the number of logs in this packet
				number_of_logs_to_send_per_packet++;	
			} 
			// if current log can't fit in current packet
			else 
			{
				// add current packet to "communication_session_info" array
				telit_gl865_set_communication_session_info(current_packet_index, number_of_bytes_to_send_per_packet, number_of_logs_to_send_per_packet);
				
				// create new packet and put current log in it
				current_packet_index++;
				number_of_bytes_to_send_per_packet = log_size;
				number_of_logs_to_send_per_packet = 1;
			}
		}
		
		// add last packet to "communication_session_info" array
		telit_gl865_set_communication_session_info(current_packet_index, number_of_bytes_to_send_per_packet, number_of_logs_to_send_per_packet);
		
		// set number of packets to sent in telit lib
		telit_gl865_set_last_packet_index( current_packet_index );
	}
}

//Polling gsm power status
void update_gsm_power_status()
{
	//check gsm power 
	if (TELIT_PWR_MONITOR_PORT->IN & TELIT_PWR_MONITOR_BITMASK)
	{
		telit_gl865_gsm->power_on = 0;
	} 
	else
	{
		telit_gl865_gsm->power_on = 1;
	}
}

//Just for testing gsm commands using PC debug uart
void gsm_testing()
{
	//PC
	if (USARTD0_get_receiver_status())
	{
		USARTE0_send_string((char*)USARTD0_get_received_message());
		USARTD0_clear_receiver_buffer();
	}
	//telit
	if (USARTE0_get_receiver_status())
	{
		USARTD0_send_string(USARTE0_get_received_message());
		USARTE0_clear_receiver_buffer();
	}
}
void gsm_operation_variables_cleanup()
{
	gsm.operation_attempts_counter=0;
}

//Initialization of gsm module, after power on
void gsm_init_operation()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_init();// licno
		}break;
		case FINISHED:
		{
			telit_gl865_gsm->state_status = IDLE;
			telit_gl865_gsm->state = IDLE;
			telit_gl865_gsm->init_ok = 1;
			gsm_operation_variables_cleanup();
		}break;
		case FAILED:
		{
			if (gsm.abort_state)
			{
				gsm_init_operation_abort_cleanup();
			}
			else
			{
				USARTD0_send_string_P((PGM_P)INIT_FAILED_MSG);
				USARTD0_send_string_P((PGM_P)RETRYING_MSG);
				telit_gl865_gsm->state_status = RUNNING;
				//retry
			}
			
		}break;
	}
}
void gsm_init_operation_failed_cleanup()
{
	//assert(!"The method or operation is not implemented.");
}

void gsm_init_operation_abort_cleanup()
{
	    gsm.gsm_state = IDLE;
		telit_gl865_gsm->state_status = IDLE;
		telit_gl865_gsm->state = IDLE;
		gsm_operation_variables_cleanup();
		USARTD0_send_string("\nOperation aborted.\n");
		//gsm.abort_state=0; //TODO mislim da bi bilo lakse da se ovo setuje u glavnoj petlji
}

//************************ Operations for sending messurments*************************//
/*
  These functions are mutually dependent, so order in which they are called is crucial. 
  They all have corresponding functions in telit_gl865 library (or other gsm modem library).
  There is max number of retries defined, and after reaching that number, operation fails. 
  Functions are written in pairs, for every operation there is function to be 
  called (xxx_failed_cleanup) if operation fails, to take care of all the states and flags.
  These functions-operations are written for specified process (eg. sending measurements) and 
  should not be used in other processes. 
 */

//Conecting to gprs operation
void gsm_connect_to_gprs_operation()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_connect_to_gprs();// licno
		}break;
		case FINISHED:
		{
			telit_gl865_gsm->state_status = IDLE;
			// Current operation is finished, switch to next one
			telit_gl865_gsm->state = CONNECTING_TO_SERVER;
			gsm_operation_variables_cleanup();
			telit_gl865_gsm->grps_status = GPRS_ON;
		}break;
		case FAILED:
		{
			gsm.operation_attempts_counter++;
			USARTD0_send_string_P((PGM_P)CONNECTING_FAILED_MSG);
			
			if(gsm.abort_state)
			{
				gsm_connect_to_gprs_operation_abort_cleanup();
			}
			else if (gsm.operation_attempts_counter>GSM_RETRY_NUMBER_2)
			{
				//All attempts to finish operation failed, abort operation
				USARTD0_send_string_P((PGM_P)CONNECTING_TO_GPRS_FAILED_MSG);
				gsm_connect_to_gprs_operation_failed_cleanup();
				error_manager_connecting_to_gprs_failed();
			}
			else
			{
				//retry
				USARTD0_send_string_P((PGM_P)RETRYING_MSG);
				telit_gl865_gsm->state_status = RUNNING;
			}
		}break;
	}
}

void gsm_connect_to_gprs_operation_failed_cleanup()
{
		telit_gl865_gsm->state_status = IDLE;
		telit_gl865_gsm->state = IDLE;
		gsm_operation_variables_cleanup();
		telit_gl865_gsm -> init_ok = 0;
}

void gsm_connect_to_gprs_operation_abort_cleanup()
{
	gsm.gsm_state = IDLE;
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = IDLE;
	gsm_operation_variables_cleanup();
	USARTD0_send_string("\nOperation aborted.\n");
	//gsm.abort_state=0;
}

//Checking network registration
void gsm_check_network_registration_operation()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_check_network_reg();// licno
		}break;
		case FINISHED:
		{
			telit_gl865_gsm->state_status = IDLE;
			// Current operation is finished, switch to next one
			telit_gl865_gsm->state = IDLE;
			gsm_operation_variables_cleanup();
			telit_gl865_gsm->network_ok = 1;
		}break;
		case FAILED:
		{
			gsm.operation_attempts_counter++;
			USARTD0_send_string_P((PGM_P)CONNECTING_FAILED_MSG);
			
			if(gsm.abort_state)
			{
				gsm_check_network_registration_operation_abort_cleanup();
			}
			else if (gsm.operation_attempts_counter>GSM_RETRY_NUMBER_2)
			{
				//All attempts to finish operation failed, abort operation
				USARTD0_send_string_P((PGM_P)CHECKING_NET_REG_FAILED_MSG);
				gsm_check_network_registration_operation_failed_cleanup();
			}
			else
			{
				//retry
				USARTD0_send_string_P((PGM_P)RETRYING_MSG);
				telit_gl865_gsm->state_status = RUNNING;
			}
		}break;
	}
}

void gsm_check_network_registration_operation_failed_cleanup()
{
		telit_gl865_gsm->state_status = IDLE;
		telit_gl865_gsm->state = IDLE;
		gsm_operation_variables_cleanup();
		telit_gl865_gsm -> init_ok = 0;
}

void gsm_check_network_registration_operation_abort_cleanup()
{
	gsm.gsm_state = IDLE;
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = IDLE;
	gsm_operation_variables_cleanup();
	USARTD0_send_string("\nOperation aborted.\n");
	//gsm.abort_state=0;
}

//Opening TCP socket operation
void gsm_connect_open_socket_operation()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_connect_to_server();// licno
		}break;
		case FINISHED:
		{
			telit_gl865_gsm->state_status = IDLE;
			telit_gl865_gsm->state = SENDING_DATA_VIA_GPRS;
			gsm_operation_variables_cleanup();
		}break;
		case FAILED:
		{
			gsm.operation_attempts_counter++;
			USARTD0_send_string_P((PGM_P)CONNECTING_TO_SOCKET_FAILED_MSG);
			
			if(gsm.abort_state)
			{
				gsm_connect_open_socket_operation_abort_cleanup();
			}
			else if (gsm.operation_attempts_counter>GSM_RETRY_NUMBER_2)
			{
				USARTD0_send_string_P((PGM_P)CONNECTING_TO_SOCKET_OPERATION_FAILED_MSG);
				gsm_connect_open_socket_operation_failed_cleanup();
				error_manager_connecting_to_server_failed();
			}
			else
			{
				//retry
				USARTD0_send_string_P((PGM_P)RETRYING_MSG);
				telit_gl865_gsm->state_status = RUNNING;
			}
			//retry
		}break;
	}
}

void gsm_connect_open_socket_operation_failed_cleanup() 
{
		telit_gl865_gsm->state_status = IDLE;
		telit_gl865_gsm->state = IDLE;
		gsm_operation_variables_cleanup();
		telit_gl865_gsm -> init_ok = 0;
}

void gsm_connect_open_socket_operation_abort_cleanup()
{
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = DISCONNECTING_FROM_GPRS;
	gsm_operation_variables_cleanup();
}

//Sending data to server through active TCP socket
void gsm_send_data_socket_operation()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_send_data_throught_socket("Dummy data");// licno
		}break;
		case FINISHED:
		{
			telit_gl865_gsm->state_status = IDLE;
			telit_gl865_gsm->state = DISCONNECTING_FROM_SERVER;
		}break;
		case FAILED:
		{
			gsm.operation_attempts_counter++;
			USARTD0_send_string_P((PGM_P)CONNECTING_FAILED_MSG);
			
			if(gsm.abort_state)
			{
				gsm_send_data_socket_operation_abort_cleanup();
			}
			else if (gsm.operation_attempts_counter>GSM_RETRY_NUMBER_2)
			{
				USARTD0_send_string_P((PGM_P)SENDING_DATA_FAILED_MSG);
				gsm_send_data_socket_operation_failed_cleanup();
			}
			else
			{
				//retry
				USARTD0_send_string_P((PGM_P)RETRYING_MSG);
				telit_gl865_gsm->state_status = RUNNING;
			}
			//retry
		}break;
	}
}

void gsm_send_data_socket_operation_failed_cleanup()
{
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = DISCONNECTING_FROM_SERVER;
	gsm_operation_variables_cleanup();
}

void gsm_send_data_socket_operation_abort_cleanup()
{
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = DISCONNECTING_FROM_SERVER;
	gsm_operation_variables_cleanup();
}


void gsm_send_data_socket_operation_time_sync()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_send_data_throught_socket_time_sync();
		}break;
		case FINISHED:
		{
			if (gsm.abort_state)
			{
				gsm_send_data_socket_operation_using_logger_abort_cleanup();
			}
			else
			{
				telit_gl865_gsm->state_status = IDLE;
				telit_gl865_gsm->state = DISCONNECTING_FROM_SERVER;
			}
		}break;
		case FAILED:
		{
			USARTD0_send_string_P((PGM_P)SENDING_DATA_FAILED_MSG);
			gsm_send_data_socket_operation_using_logger_failed_cleanup();
		}break;
	}
}

//Sending data to server through active TCP socket
void gsm_send_data_socket_operation_using_logger()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_send_data_throught_socket_using_logger();
		}break;
		case FINISHED:
		{
			if (gsm.abort_state)
			{
				gsm_send_data_socket_operation_using_logger_abort_cleanup();
			}
			else
			{
				telit_gl865_gsm->state_status = IDLE;
				telit_gl865_gsm->state = DISCONNECTING_FROM_SERVER;
			}
		}break;
		case FAILED:
		{
			USARTD0_send_string_P((PGM_P)SENDING_DATA_FAILED_MSG);
			gsm_send_data_socket_operation_using_logger_failed_cleanup();
		}break;
	}
}

void gsm_send_data_socket_operation_using_logger_failed_cleanup()
{
		telit_gl865_gsm->state_status = IDLE;
		telit_gl865_gsm->state = DISCONNECTING_FROM_SERVER;
		gsm_operation_variables_cleanup();
}

void gsm_send_data_socket_operation_using_logger_abort_cleanup()
{
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = DISCONNECTING_FROM_SERVER;
	gsm_operation_variables_cleanup();
}

//Disconnectiong from server (closing socket, disabling gprs)
void gsm_disconnect_from_server_operation()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_disconnect_from_server();// licno
		}break;
		case FINISHED:
		{
			telit_gl865_gsm->state_status = IDLE;
			telit_gl865_gsm->state = IDLE;
		    gsm.gsm_state = IDLE;
			gsm_operation_variables_cleanup();
		}break;
		case FAILED:
		{
			gsm.operation_attempts_counter++;
			USARTD0_send_string_P((PGM_P)CONNECTING_FAILED_MSG);
			
			if(gsm.abort_state)
			{
				gsm_disconnect_from_server_operation_abort_cleanup();
			}
			else if (gsm.operation_attempts_counter>GSM_RETRY_NUMBER_2)
			{
				USARTD0_send_string_P((PGM_P)DISCONNECTING_FROM_GPRS_FAILED_MSG);
				gsm_disconnect_from_server_operation_failed_cleanup();
			}
			else
			{
				//retry
				USARTD0_send_string_P((PGM_P)RETRYING_MSG);
				telit_gl865_gsm->state_status = RUNNING;
			}
		}break;
	}
}
void gsm_disconnect_from_server_operation_failed_cleanup()
{
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = IDLE;
	gsm_operation_variables_cleanup();
	telit_gl865_gsm -> init_ok = 0;
}

void gsm_disconnect_from_server_operation_abort_cleanup()
{
	gsm.gsm_state = IDLE;
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = IDLE;
	gsm_operation_variables_cleanup();
	USARTD0_send_string("\nOperation aborted.\n");
	//gsm.abort_state=0;
}

//Conecting to gprs operation
void gsm_disconnect_from_gprs_operation()
{
	switch(telit_gl865_gsm->state_status)
	{
		case IDLE:
		{
			initialise_telit_cmd_variables();
			telit_gl865_gsm->state_status = RUNNING;
		}break;
		case RUNNING:
		{
			telit_disconnect_from_gprs();// licno
		}break;
		case FINISHED:
		{
			telit_gl865_gsm->state_status = IDLE;
			telit_gl865_gsm->state = IDLE;
			gsm.gsm_state = IDLE;
			gsm_operation_variables_cleanup();
			telit_gl865_gsm->grps_status = GPRS_OFF;
		}break;
		case FAILED:
		{
			gsm.operation_attempts_counter++;
			USARTD0_send_string_P((PGM_P)DISCONNECTING_FAILED_MSG);
			
			if(gsm.abort_state)
			{
				gsm_disconnect_from_gprs_operation_abort_cleanup();
			}
			else if (gsm.operation_attempts_counter>GSM_RETRY_NUMBER_2)
			{
				USARTD0_send_string_P((PGM_P)DISCONNECTING_FROM_GPRS_FAILED_MSG);
				gsm_disconnect_from_gprs_operation_failed_cleanup();
			}
			else
			{
				//retry
				USARTD0_send_string_P((PGM_P)RETRYING_MSG);
				telit_gl865_gsm->state_status = RUNNING;
			}
		}break;
	}
}

void gsm_disconnect_from_gprs_operation_failed_cleanup()
{
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = IDLE;
	gsm_operation_variables_cleanup();
	telit_gl865_gsm -> init_ok = 0;
}

void gsm_disconnect_from_gprs_operation_abort_cleanup()
{
	gsm.gsm_state = IDLE;
	telit_gl865_gsm->state_status = IDLE;
	telit_gl865_gsm->state = IDLE;
	gsm_operation_variables_cleanup();
	USARTD0_send_string("\nOperation aborted.\n");
	//gsm.abort_state=0;
}
//******************************************************************************

