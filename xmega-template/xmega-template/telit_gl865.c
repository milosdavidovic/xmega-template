/*
 * telit_gl865.c
 *
 * Created: 22/7/2015 1:06:36 PM
 *  Author: milos.davidovic
 */ 
#include "telit_gl865.h"
#include "RTC.h"

//*************COMMANDS***********//

//INIT
const char TELIT_AT_CMD[]PROGMEM = "AT\r";
const char TELIT_SELINT_CMD[]PROGMEM ="AT#SELINT=2\r";
const char TELIT_ECHOOFF_CMD[]PROGMEM ="ATE0\r";
const char TELIT_IMEI_CMD[] PROGMEM = "AT#CGSN\r";
const char TELIT_STATLED_ON_CMD[] PROGMEM = "AT#SLED=2\r"; //Turn status led on
const char TELIT_NO_FLOW_CTRL_CMD[] PROGMEM = "AT&K0"; //set no flow control
const char TELIT_PIN_STATE_CMD[] PROGMEM = "AT+CPIN?\r"; //ask for pin state command
const char TELIT_ENTER_PIN_CMD_FIRST_PART[] PROGMEM = "AT+CPIN="; //enter pin command
const char TELIT_ENTER_PIN_CMD_SECOND_PART[] PROGMEM = "\r"; //enter pin command

//Initialize telit for SMS
const char TELIT_SMSMEM_CMD[]PROGMEM = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";		//MD: Store new SMS on SIM card (SM)
const char TELIT_TXTMD_CMD[]PROGMEM = "AT+CMGF=1\r";			//MD: SMS msgs in text mode
const char TELIT_DLSMS_CMD[]PROGMEM = "AT+CMGD=1,3\r";			 //MD: Ignore SMS index and delete all messages UNDONE const char TELIT_DLSMS[]PROGMEM = "AT+CMGD=1,4\r\n";
const char TELIT_SMS_IND_CMD[]PROGMEM = "AT+CNMI=2,1\r";
//Init gprs
//const char TELIT_GPRS_SETTINGS_CMD[]PROGMEM ="AT+CGDCONT=1,\"IP\",\"CORPMTS\",\"0.0.0.0\",0,0\r";
const char TELIT_GPRS_SETTINGS_CMD_FIRST_PART[]PROGMEM ="AT+CGDCONT=1,\"IP\",\""; //gprs settings
const char TELIT_GPRS_SETTINGS_CMD_SECOND_PART[]PROGMEM ="\",\"0.0.0.0\",0,0\r"; //gprs settings
const char TELIT_PASSW_CMD[]PROGMEM = "AT#PASSW=\"\"\r";
const char TELIT_USER_CMD[]PROGMEM = "AT#USERID=\"\"\r";
const char TELIT_EXTEND_ERR_CMD[]PROGMEM = "AT#CMEEMODE=1"; //extended set of error codes for gprs
//???
const char TELIT_CGQMIN_CMD[]PROGMEM ="AT+CGQMIN=1,0,0,0,0,0\r";
const char TELIT_CGQREQ_CMD[]PROGMEM ="AT+CGQREQ=1,0,0,3,0,0\r";
const char TELIT_SCFG_CMd[]PROGMEM = "AT#SCFG=1,1,512,30,300,100\r";
//Module time
const char TELIT_SYNC_TIME_CMD[]PROGMEM="AT#NITZ=1,0\r"; //Sync telit time with network time
const char TELIT_CFUN_FULL_OPERATION_CMD []PROGMEM="AT+CFUN=1\r"; //Work modes : 1- mobile full functionality with power saving disabled (factory default)
//GPRS
const char TELIT_GPRS_ON_CMD[]PROGMEM = "AT#GPRS=1\r";
const char TELIT_GPRS_OFF_CMD[]PROGMEM = "AT#GPRS=0\r";
const char TELIT_GPRS_CHECK_CMD[]PROGMEM = "AT#GPRS?\r";
//TCP SOCKET
//const char TELIT_SKT_CON_ON_CMD[] PROGMEM = "AT#SD=1,0,50677,\"109.92.19.72\",0,0,1\r"; //Open socket
const char TELIT_SKT_CON_ON_CMD_FIRST_PART[] PROGMEM = "AT#SD=1,0,"; //Open socket
const char TELIT_SKT_CON_ON_CMD_SECOND_PART[] PROGMEM = ",\""; //Open socket
const char TELIT_SKT_CON_ON_CMD_THIRD_PART[] PROGMEM = "\",0,0,1\r"; //Open socket
const char TELIT_SKT_OPEN_TIMEOUT_CMD[]PROGMEM = "AT#SKTCT=28\r";	//Sets the TCP connection time-out for the first CONNECT answer from the TCP peer to be received (in 100ms units). Default is 600.
const char TELIT_SKT_SEND_CMD[]PROGMEM = "AT#SSENDEXT=1,30\r";	//Send Data In Command Mode /"AT#SSEND=1\r\n	
const char TELIT_SKT_SEND_CMD_FIRST_PART[]PROGMEM = "AT#SSENDEXT=1,";	//Send Data In Command Mode   30\r\n
const char TELIT_SKT_SEND_CMD_SECOND_PART[]PROGMEM = "\r";	//Send Data In Command Mode   30\r\n
const char TELIT_SKT_RCV_CMD[]PROGMEM = "AT#SRECV=1,100\r";//Receive data from socket
const char TELIT_SKT_CONN_OFF_CMD[]PROGMEM = "AT#SH=1\r";//Close socket
//NETWORK
const char TELIT_NET_REG_CMD[]PROGMEM = "AT+CREG?\r";	//Check if registrated on the network

//RESPONSES
const char TELIT_MSG_OK_RESP[]PROGMEM  = "OK";
const char TELIT_GPRS_ON_RESP []PROGMEM = "+IP:";
const char TELIT_GPRS_IS_ON []PROGMEM = "#GPRS: 1";
const char TELIT_GPRS_IS_OFF []PROGMEM = "#GPRS: 0";
const char TELIT_GPRS_ON_PENDING []PROGMEM = "#GPRS: 2";
const char TELIT_SOCKET_OPEN_RESP []PROGMEM = "CONNECT";
const char TELIT_SOCKET_READY_TO_SEND_DATA_RESP []PROGMEM = ">";
const char TELIT_MSG_ERROR_RESP[]PROGMEM  = "ERROR";
const char TELIT_SERVER_RECIEVED_DATA_RESP []PROGMEM = "ALFA SERVER";
const char TELIT_MSG_ETST_RESP[]PROGMEM  = "O1K";
const char TELIT_MSG_FROM_SRV_RESP[]PROGMEM  = "SRING";
const char TELIT_NET_REG_OK_RESP[]PROGMEM  = "+CREG: 0,1";
const char TELIT_PIN_REQUIRED []PROGMEM = "SIM PIN";
const char TELIT_PIN_NOT_REQUIRED []PROGMEM = "READY";

const char TELIT_DEVICE_TYPE []PROGMEM = "AT&WM10=";
const char TELIT_MESSAGE_TYPE_LOG []PROGMEM = "LOG_";
const char TELIT_MESSAGE_TYPE_TIME_SYNC []PROGMEM = "TIME";
const char TELIT_MESSAGE_TYPE_ERROR_REPORT []PROGMEM = "ERR_";
//const char TELIT_TEST_ID []PROGMEM = "WM10-XXXXXX";

const char TELIT_INIT_START_MSG []PROGMEM = "\nTelit init started.";
const char TELIT_INIT_FINISHED_MSG []PROGMEM = "\nTelit init finished.";
const char TELIT_STEP_MSG []PROGMEM = "\nStep ";
const char TELIT_SENT_MSG []PROGMEM = " sent.\n";
const char TELIT_CONNECTING_TO_GPRS_START_MSG []PROGMEM = "\nConnecting to gprs started.";
const char TELIT_CONNECTING_TO_GPRS_FINISH_MSG []PROGMEM = "\nConnecting to gprs finished.";
const char TELIT_DISCONNECTING_TO_GPRS_START_MSG []PROGMEM = "\nDisconnecting from gprs started.";
const char TELIT_DISCONNECTING_TO_GPRS_FINISH_MSG []PROGMEM = "\nDisconnecting from gprs finished.";
const char TELIT_CONNECTING_TO_SERVER_START_MSG []PROGMEM = "\nConnecting to server started.";
const char TELIT_CONNECTING_TO_SERVER_FINISH_MSG []PROGMEM = "\nConnecting to server finished.";
const char TELIT_SENDING_DATA_START_MSG []PROGMEM = "\nSending data started.";
const char TELIT_SENDING_DATA_FINISH_MSG []PROGMEM = "\nSending data finished.";
const char TELIT_TIME_SYNC_FINISH_MSG []PROGMEM = "\nTime synchronization finished.";
const char TELIT_CHECK_NET_REG_START_MSG []PROGMEM = "\nChecking network registration started.";
const char TELIT_CHECK_NET_REG_FINISH_MSG []PROGMEM = "\nChecking network registration finished.";
const char TELIT_TIME_UPDATED_MSG []PROGMEM = "\nTime updated.";
const char NUM_OF_LOGS_MSG []PROGMEM = "\nNumber of logs to send in this packet: ";
const char NUM_OF_BYTES_MSG []PROGMEM = "\nNumber of bytes to send in this packet: ";
const char NUM_OF_LOGS_REMAINING_MSG []PROGMEM = "\nNumber of logs remaining to send: ";
const char OK_MSG []PROGMEM = "\nOK.";
const char GPRS_IS_ON_MSG []PROGMEM = "\nGPRS state is ON.";
const char GPRS_IS_OFF_MSG []PROGMEM = "\nGPRS state is OFF.";
const char MANAGING_TIMEOUT_MSG []PROGMEM = "\nManaging timeout. Telit responded with: ";
const char NEW_LINE_MSG []PROGMEM = "\n";
const char STEP_RETRYING_MSG []PROGMEM = "\nCurrent step retrying.";
const char STEP_FAILED_MSG []PROGMEM = "\nCurrent step failed.";

static T_telit telit_gl865;

uint8_t telit_cmd_attempts_counter=0;

//params for communication with server
static T_comm_param communication_session_info[10];
uint16_t current_packet_index = 0;
uint32_t last_packet_index = 0;

// Current state for telit operations (telit_init, connect_to_gprs, ...)
uint8_t init_at_state = 0;

char current_telit_cmd [100];

char server_port[6] = "\0";
char server_ip[16] = "\0";
char sim_pin[5] = "\0";
char sim_apn [20] = "\0";
char SMS_server_1[13];
char SMS_server_2[13];
char IMEI[16];

void telit_set_server_port(char* port)
{
	strcpy(server_port, port);
}

void telit_set_server_ip(char* ip)
{
	strcpy(server_ip, ip);
}

char * telit_get_server_port()
{
	return (char*)&(server_port[0]);
}

char * telit_get_server_ip()
{
	return (char*)&(server_ip[0]);
}

void telit_set_apn(char* apn)
{
	strcpy(sim_apn, apn);
}

char * telit_get_apn()
{
	return (char*)&(sim_apn[0]);
}

void telit_set_pin(char* pin)
{
	strcpy(sim_pin, pin);
}

char * telit_get_pin()
{
	return (char*)&(sim_pin[0]);
}

void telit_set_sms_server1(char* number)
{
	strcpy(SMS_server_1, number);
}

char * telit_get_sms_server1()
{
	return (char*)&(SMS_server_1[0]);
}

void telit_set_sms_server2(char* number)
{
	strcpy(SMS_server_2, number);
}

char * telit_get_sms_server2()
{
	return (char*)&(SMS_server_2[0]);
}

void telit_set_imei(char* imei)
{
	strcpy(IMEI, imei);
}

char * telit_get_imei()
{
	return (char*)&(IMEI[0]);
}

void telit_gl865_set_communication_session_info(uint8_t index, uint16_t num_of_bytes, uint16_t num_of_logs)
{
	communication_session_info[index].number_of_bytes = num_of_bytes;
	communication_session_info[index].number_of_logs = num_of_logs;
}

void telit_gl865_clear_communication_session_info()
{
	for(uint8_t i=0;i<10;i++)
	{
		communication_session_info[i].number_of_bytes = 0;
		communication_session_info[i].number_of_logs = 0;
	}
}

uint32_t telit_gl865_get_number_of_packets_to_send()
{
	return last_packet_index;
}

void initTelit()
{
	telit_gl865.init_ok = 0;
		
	telit_gl865.power_on = 1;//uvek je power on zato sto nije power saving mode
	telit_gl865.power_present = 0;
	telit_gl865.psm = 0;
	
	telit_gl865.state = IDLE;
	telit_gl865.state_status=IDLE;
	telit_gl865.waiting_for_answer = 0;
	
	telit_gl865.grps_status = GPRS_OFF; //not connected to gprs
	telit_gl865.network_ok=0;
	
}

uint8_t telit_check_power()
{
	telit_gl865.power_present = 1;
	return telit_gl865.power_present;
}

T_telit * get_telit_struct()
{
	return &telit_gl865;
}


#pragma region TELIT_MAIN_OPERATIONS
/*
All main operations have corresponding OPERATION STATE which are defined in telit_gl865.h.
During execution each operation will pass trough defined STATE_STATES which are also defined
in  telit_gl865.h. When operation is completed successfully it's STATE_STATE becomes "FINNISHED". 
In case of failure STATE_STATE becomes "FAILED". At this moment operation execution is over, 
and it is up to user to decide what to do (operations won't auto retry execution). It is most 
important to check if telit_gl865.state_status is IDLE before calling each of these functions, parallel 
execution is not possible and will most likely cause failure
*/

void telit_init()
{
	telit_gl865.state = INITIALIZING;
	switch(init_at_state)
	{
		case 0:
		{
			
			if(!telit_gl865.waiting_for_answer)
			{
				USARTD0_send_string_P((PGM_P)TELIT_INIT_START_MSG);
				send_telit_test_cmd();
				cmd_formalities(TIMEOUT_2000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		case 1:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				send_selint_cmd();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
									
				}
			}
		}break;	
		
		case 2:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				turn_echo_off();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		case 3:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				get_telit_imei();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		case 4:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				get_pin_state();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_pin_state();
				}
				else
				{
					if (!manage_telit_response_pin_state())
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		case 5:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				enter_sim_pin();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						//manage_cmd_timeout();
						USARTD0_send_string("GRESKA.");
					}
				}
			}
		}break;
		
		case 6:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_sms_mem();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 7:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_sms_text_mode();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 8:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				delete_all_sms();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 9:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_sms_ind();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 10:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_gprs_settings();
				cmd_formalities(TIMEOUT_1000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 11:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_gprs_user();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 12:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_gprs_pass();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 13:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_cgq_req();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 14:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_cgq_min();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 15:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_telit_full_mode();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 16:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				set_auto_network_datetime();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 17:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				send_gprs_off();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		//all states are finished
		default:
		{
			telit_gl865.state_status = FINISHED;
			init_at_state=0;
			USARTD0_send_string_P((PGM_P)TELIT_INIT_FINISHED_MSG);
		}break;
	}
}

void telit_connect_to_gprs()
{
	switch(init_at_state)
	{
		case 0:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				USARTD0_send_string_P((PGM_P)TELIT_CONNECTING_TO_GPRS_START_MSG);
				send_gprs_check();
				cmd_formalities(TIMEOUT_5000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_grps_chk();
				}
				else
				{
					if(!manage_telit_response_grps_chk())
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 1:
		{
			if (telit_gl865.grps_status == GPRS_ON)
			{
				//allready connected
				init_at_state = -1;
			}
			else
			{
				init_at_state++;
			}
		}break;
		case 2:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				send_gprs_on();
				cmd_formalities(TIMEOUT_5000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_GPRS_ON_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_GPRS_ON_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		//all states are finished
		default:
		{
			telit_gl865.state_status = FINISHED;
			init_at_state=0;
			USARTD0_send_string_P((PGM_P)TELIT_CONNECTING_TO_GPRS_FINISH_MSG);
		}break;
	}
}

void telit_check_network_reg()
{
	switch(init_at_state)
	{
		case 0:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				USARTD0_send_string_P((PGM_P)TELIT_CHECK_NET_REG_START_MSG);
				check_network();
				cmd_formalities(TIMEOUT_2000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_NET_REG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_NET_REG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		//all states are finished
		default:
		{
			telit_gl865.state_status = FINISHED;
			init_at_state=0;
			USARTD0_send_string_P((PGM_P)TELIT_CHECK_NET_REG_FINISH_MSG);
		}break;
	}
}

void telit_disconnect_from_gprs()
{
	switch(init_at_state)
	{
		case 0:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				send_gprs_off();
				cmd_formalities(TIMEOUT_3000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		//all states are finished
		default:
		{
			telit_gl865.state_status = FINISHED;
			init_at_state=0;
			USARTD0_send_string_P((PGM_P)TELIT_DISCONNECTING_TO_GPRS_FINISH_MSG);
		}break;
	}
}

void telit_connect_to_server()
{
	switch(init_at_state)
	{
		case 0:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				USARTD0_send_string_P((PGM_P)TELIT_SENDING_DATA_START_MSG);
				set_open_socket_timeout();
				cmd_formalities(TIMEOUT_500);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		case 1:
		{
			
			if(!telit_gl865.waiting_for_answer)
			{
				USARTD0_send_string_P((PGM_P)TELIT_CONNECTING_TO_SERVER_START_MSG);
				open_server_socket();
				cmd_formalities(TIMEOUT_3000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_OK_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
					{
						manage_cmd_timeout();
					}
					
				}
			}
		}break;
		
		//all states are finished
		default:
		{
			telit_gl865.state_status = FINISHED;
			init_at_state=0;
			USARTD0_send_string_P((PGM_P)TELIT_CONNECTING_TO_SERVER_FINISH_MSG);
		}break;
	}
}

void telit_send_data_throught_socket( char * data)
{
	switch(init_at_state)
	{
		case 0:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				USARTD0_send_string_P((PGM_P)TELIT_SENDING_DATA_START_MSG);
				socket_send_request();
				cmd_formalities(TIMEOUT_2000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_SOCKET_READY_TO_SEND_DATA_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_SOCKET_READY_TO_SEND_DATA_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;

		case 1:
		{
			
			if(!telit_gl865.waiting_for_answer)
			{
				send_data_gprs(data);
				cmd_formalities(TIMEOUT_2000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_FROM_SRV_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_FROM_SRV_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;

		case 2:
		{
			
			if(!telit_gl865.waiting_for_answer)
			{
				receive_socket_data();
				cmd_formalities(TIMEOUT_3000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_SERVER_RECIEVED_DATA_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_SERVER_RECIEVED_DATA_RESP))
					{
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		//all states are finished
		default:
		{
			telit_gl865.state_status = FINISHED;
			init_at_state=0;
			USARTD0_send_string_P((PGM_P)TELIT_SENDING_DATA_FINISH_MSG);
		}break;
	}
}

void telit_send_data_throught_socket_time_sync()
{
	switch(init_at_state)
	{
		case 0:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				USARTD0_send_string_P((PGM_P)TELIT_SENDING_DATA_START_MSG);
				socket_send_request_time_sync();
				cmd_formalities(TIMEOUT_2000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_SOCKET_READY_TO_SEND_DATA_RESP);
				}
				else
				{
					if (!manage_telit_response_default(TELIT_SOCKET_READY_TO_SEND_DATA_RESP))
					{
						
						manage_cmd_timeout();
					}
					
				}
			}
		}break;

		case 1:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				send_data_gprs_time_sync();
				cmd_formalities(TIMEOUT_3000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_default(TELIT_MSG_FROM_SRV_RESP); //look for SRING (server responded with something)
				}
				else
				{
					if (!manage_telit_response_default(TELIT_MSG_FROM_SRV_RESP))
					{
						telit_cmd_attempts_counter = TELIT_MAX_RETRY_NUMBER;  //This step must not be executed more then once (force failure)
						manage_cmd_timeout();
					}
				}
			}
		}break;

		case 2:
		{
			if(!telit_gl865.waiting_for_answer)
			{
				receive_socket_data();
				cmd_formalities(TIMEOUT_2000);
			}
			else
			{
				if (!action_timeout_elapsed())
				{
					manage_telit_response_time_sync(TELIT_SERVER_RECIEVED_DATA_RESP);
				}
				else
				{
					if (!manage_telit_response_time_sync(TELIT_SERVER_RECIEVED_DATA_RESP))
					{
						telit_cmd_attempts_counter = TELIT_MAX_RETRY_NUMBER;  //This step must not be executed more then once (force failure)
						manage_cmd_timeout();
					}
				}
			}
		}break;
		
		//all states are finished
		default:
		{
			telit_gl865.state_status = FINISHED;
			init_at_state=0;
			//USARTD0_send_string_P((PGM_P)TELIT_SENDING_DATA_FINISH_MSG);
		}break;
	}
}

void telit_disconnect_from_server()
{
		switch(init_at_state)
		{
			case 0:
			{
				if(!telit_gl865.waiting_for_answer)
				{
					USARTD0_send_string("Disconnecting from server started.\n");
					close_server_socket();
					
					cmd_formalities(TIMEOUT_5000);
				}
				else
				{
					if (!action_timeout_elapsed())
					{
						manage_telit_response_default(TELIT_MSG_OK_RESP);
					}
					else
					{
						if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
						{
							manage_cmd_timeout();
						}
					}
				}
			}break;
			
			case 1:
			{
				
				if(!telit_gl865.waiting_for_answer)
				{
					send_gprs_off();
					cmd_formalities(TIMEOUT_3000);
				}
				else
				{
					if (!action_timeout_elapsed())
					{
						manage_telit_response_default(TELIT_MSG_OK_RESP);
					}
					else
					{
						if (!manage_telit_response_default(TELIT_MSG_OK_RESP))
						{
							manage_cmd_timeout();
						}
					}
				}
			}break;
			//all states are finished
			default:
			{
				telit_gl865.state_status = FINISHED;
				init_at_state=0;
				USARTD0_send_string("Disconnecting from server finished.\n");
			}break;
		}
}

#pragma endregion TELIT_MAIN_OPERATIONS

void telit_gl865_set_last_packet_index(uint32_t last_pack_ind)
{
	last_packet_index = last_pack_ind;
}

#pragma region TelitAtfunctions
/*These are low level functions and they execute single Telit AT command. */
void send_telit_test_cmd()
{
	strcpy_P(current_telit_cmd, TELIT_AT_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void send_selint_cmd()
{
	strcpy_P(current_telit_cmd, TELIT_SELINT_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void turn_echo_off()
{
	strcpy_P(current_telit_cmd, TELIT_ECHOOFF_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void get_telit_imei()
{
	strcpy_P(current_telit_cmd, TELIT_IMEI_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void get_pin_state()
{
	strcpy_P(current_telit_cmd, TELIT_PIN_STATE_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void enter_sim_pin()
{
	strcpy_P(current_telit_cmd, TELIT_ENTER_PIN_CMD_FIRST_PART);
	strcat(current_telit_cmd, sim_pin);
	strcat_P(current_telit_cmd, TELIT_ENTER_PIN_CMD_SECOND_PART);
	USARTE0_send_string(current_telit_cmd);
	//USARTD0_send_string(current_telit_cmd); //Debug
}

void set_sms_mem()
{
	strcpy_P(current_telit_cmd, TELIT_SMSMEM_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_sms_text_mode()
{
	strcpy_P(current_telit_cmd, TELIT_TXTMD_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void delete_all_sms()
{
	strcpy_P(current_telit_cmd, TELIT_DLSMS_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_sms_ind()
{
	strcpy_P(current_telit_cmd, TELIT_SMS_IND_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_gprs_settings()
{
	strcpy_P(current_telit_cmd, TELIT_GPRS_SETTINGS_CMD_FIRST_PART);
	strcat(current_telit_cmd, sim_apn);
	strcat_P(current_telit_cmd, TELIT_GPRS_SETTINGS_CMD_SECOND_PART);
	USARTE0_send_string(current_telit_cmd);
	//USARTD0_send_string(current_telit_cmd); //Debug
}

void set_gprs_user()
{
	strcpy_P(current_telit_cmd, TELIT_USER_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_gprs_pass()
{
	strcpy_P(current_telit_cmd, TELIT_PASSW_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_cgq_req()
{
	strcpy_P(current_telit_cmd, TELIT_CGQREQ_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_cgq_min()
{
	strcpy_P(current_telit_cmd, TELIT_CGQMIN_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_telit_full_mode()
{
	strcpy_P(current_telit_cmd, TELIT_CFUN_FULL_OPERATION_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_auto_network_datetime()
{
	strcpy_P(current_telit_cmd, TELIT_SYNC_TIME_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void send_gprs_on()
{
	strcpy_P(current_telit_cmd, TELIT_GPRS_ON_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void send_gprs_check()
{
	strcpy_P(current_telit_cmd, TELIT_GPRS_CHECK_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void open_server_socket()
{
	strcpy_P(current_telit_cmd, TELIT_SKT_CON_ON_CMD_FIRST_PART);
	strcat(current_telit_cmd, server_port);
	strcat_P(current_telit_cmd, TELIT_SKT_CON_ON_CMD_SECOND_PART);
	strcat(current_telit_cmd, server_ip);
	strcat_P(current_telit_cmd, TELIT_SKT_CON_ON_CMD_THIRD_PART);
	USARTE0_send_string(current_telit_cmd);
	//USARTD0_send_string(current_telit_cmd); // Debug
}

void set_open_socket_timeout()
{
	strcpy_P(current_telit_cmd, TELIT_SKT_OPEN_TIMEOUT_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void socket_send_request()
{
	strcpy_P(current_telit_cmd, TELIT_SKT_SEND_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_no_flow_ctrl()
{
	strcpy_P(current_telit_cmd, TELIT_NO_FLOW_CTRL_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void set_ext_err_code()
{
	strcpy_P(current_telit_cmd, TELIT_EXTEND_ERR_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void check_network()
{
	strcpy_P(current_telit_cmd, TELIT_NET_REG_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void socket_send_request_time_sync()
{
	char num_of_bytes_string[10];
	
	//form command message based on number of bytes to send
	strcpy_P(current_telit_cmd, TELIT_SKT_SEND_CMD_FIRST_PART);
	ultoa(DEVICE_TYPE_SIZE +  DEVICE_ID_SIZE + MESSAGE_TYPE_SIZE + NULL_TERMINATOR_SIZE + 
		  DATA_LENGTH_INFO_SIZE, num_of_bytes_string, 10);
	strcat(current_telit_cmd,num_of_bytes_string);
	strcat_P(current_telit_cmd, TELIT_SKT_SEND_CMD_SECOND_PART);
	
	//send command to telit
	USARTE0_send_string(current_telit_cmd);
	USARTD0_send_string(current_telit_cmd);
}


void clear_data_for_server(uint8_t* data_for_server, uint8_t size)
{
	for(uint8_t i=0;i<size;i++)
	{
		data_for_server[i] = 0x00;
	}
}

void send_data_gprs(char* data_to_send)
{
	strcpy(current_telit_cmd, data_to_send);
	USARTE0_send_data(data_to_send, 10);
	USARTE0_send_data(data_to_send + 10, 10);
	USARTE0_send_data(data_to_send + 20, 10);
	//USARTE0_send_byte(26);
}

void receive_socket_data()
{
	strcpy_P(current_telit_cmd, TELIT_SKT_RCV_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void close_server_socket()
{
	strcpy_P(current_telit_cmd, TELIT_SKT_CONN_OFF_CMD);
	USARTE0_send_string(current_telit_cmd);
}

void send_gprs_off()
{
	strcpy_P(current_telit_cmd, TELIT_GPRS_OFF_CMD);
	USARTE0_send_string(current_telit_cmd);
}

#pragma endregion TelitAtfunctions

// Searches for string in telit usart buffer
uint8_t analyse_telit_answer(const char * string_to_search)
{
		if(strstr_P((char *)USARTE0_get_received_message(), string_to_search) )
		{
			USARTD0_send_string(USARTE0_get_received_message());
			USARTE0_clear_receiver_buffer();
			return 1;
		}
		return 0;
}


// Checking if timeout occurred 
uint8_t action_timeout_elapsed()
{
	if (getTCO_counter(0))
	{
		return 0;
	}
	else
	{
		return 1;
	}
			
}

// Standard procedure when issuing telit AT command (setting timeout, incrementing retry counter, ...)
void cmd_formalities(int timeout_lenght)
{
	char temp [3];
	telit_cmd_attempts_counter++;
	setTCO_counter(0,timeout_lenght);
	telit_gl865.waiting_for_answer = 1;
	
	USARTD0_send_string_P((PGM_P)TELIT_STEP_MSG);
	itoa(init_at_state,temp,10);
	USARTD0_send_string(temp);
	USARTD0_send_string_P((PGM_P)TELIT_SENT_MSG);
}

//	checking if telit responded to AT command as expected (if response doesn't need processing)
uint8_t manage_telit_response_default(const char *expected_response)
{
	char temp [3];
	if(analyse_telit_answer((char*)expected_response))
	{
		telit_gl865.waiting_for_answer = 0;
		telit_cmd_attempts_counter=0;
		USARTD0_send_string("Step ");
		itoa(init_at_state,temp,10);
		USARTD0_send_string(temp);
		USARTD0_send_string(" OK.\n");
		init_at_state++;
		return 1;
		//timeout
	}
	return 0;
}

uint8_t manage_telit_response_time_sync(const char *expected_response)
{
	char temp [3];
	if(analyse_telit_answer_and_sync_time((char*)expected_response))
	{
		telit_gl865.waiting_for_answer = 0;
		telit_cmd_attempts_counter=0;
		USARTD0_send_string("Step ");
		itoa(init_at_state,temp,10);
		USARTD0_send_string(temp);
		USARTD0_send_string(" OK.\n");
		init_at_state++;
		return 1;
		//timeout
	}
	return 0;
}

//	checking if telit responded to AT command as expected (if response needs processing)
uint8_t manage_telit_response_grps_chk()
{
	char temp [3];
	if(analyse_telit_answer(TELIT_GPRS_IS_ON))
	{
		telit_gl865.waiting_for_answer = 0;
		telit_cmd_attempts_counter=0;
		USARTD0_send_string_P((PGM_P)TELIT_STEP_MSG);
		itoa(init_at_state,temp,10);
		USARTD0_send_string(temp);
		USARTD0_send_string_P((PGM_P)OK_MSG);
		USARTD0_send_string_P((PGM_P)GPRS_IS_ON_MSG);
		init_at_state++;
		telit_gl865.grps_status = GPRS_ON;
		return 1;
	}
	else if(analyse_telit_answer(TELIT_GPRS_IS_OFF))
	{
		telit_gl865.waiting_for_answer = 0;
		telit_cmd_attempts_counter=0;
		USARTD0_send_string_P((PGM_P)TELIT_STEP_MSG);
		itoa(init_at_state,temp,10);
		USARTD0_send_string(temp);
		USARTD0_send_string_P((PGM_P)OK_MSG);
		USARTD0_send_string_P((PGM_P)GPRS_IS_OFF_MSG);
		init_at_state++;
		telit_gl865.grps_status = GPRS_OFF;
		return 1;
	}
	return 0;
}

uint8_t manage_telit_response_pin_state()
{
	char temp [3];
	if(analyse_telit_answer(TELIT_PIN_REQUIRED))
	{
		telit_gl865.waiting_for_answer = 0;
		telit_cmd_attempts_counter=0;
		USARTD0_send_string("Step ");
		itoa(init_at_state,temp,10);
		USARTD0_send_string(temp);
		USARTD0_send_string(" OK.\n");
		init_at_state++;
		return 1;
		//timeout
	}
	else if (analyse_telit_answer(TELIT_PIN_NOT_REQUIRED))
	{
		telit_gl865.waiting_for_answer = 0;
		telit_cmd_attempts_counter=0;
		USARTD0_send_string("Step ");
		itoa(init_at_state,temp,10);
		USARTD0_send_string(temp);
		USARTD0_send_string(" OK.\n");
		init_at_state++;
		USARTD0_send_string("\nPIN is not required. Skipping entering PIN...\n");
		init_at_state++;
		return 1;
		//timeout
	}
	return 0;
}

// Cleaning up after command timeout
void manage_cmd_timeout()
{
	USARTD0_send_string_P((PGM_P)MANAGING_TIMEOUT_MSG);
	USARTD0_send_string(USARTE0_get_received_message());
    USARTD0_send_string_P((PGM_P)NEW_LINE_MSG);
	USARTE0_clear_receiver_buffer();
	if (telit_cmd_attempts_counter < TELIT_MAX_RETRY_NUMBER)
	{
		telit_gl865.waiting_for_answer = 0;
		USARTD0_send_string_P((PGM_P)STEP_RETRYING_MSG);
	}
	else
	{
		telit_cmd_attempts_counter = 0;
		telit_gl865.waiting_for_answer = 0;
		USARTD0_send_string_P((PGM_P)STEP_FAILED_MSG);
		telit_gl865.state_status=FAILED;
		init_at_state=0;
	}
}


// Initializing telit operation variables
// Must be called prior to setting state_status to running.
void initialise_telit_cmd_variables()
{
	telit_cmd_attempts_counter=0;
	init_at_state = 0;
	telit_gl865.waiting_for_answer = 0;
}

uint8_t telit_status(void)
{
    return telit_gl865.state_status;
}
