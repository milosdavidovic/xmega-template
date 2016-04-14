/*
 * wm10.h
 *
 * Created: 4/8/2015 3:50:56 PM
 *  Author: milos.davidovic
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/io.h>
#include "system.h"
#include "trash_sonar_mb7137.h"

#define FIRMWARE_VERSION "0.2"

//device protocols
#define INIT_AT 0
#define MODBUS 1
#define WMBUS 2

//device communication lines
#define SMS 0
#define GPRS 1

//Operation timeouts
#define TIMEOUT_10 10
#define TIMEOUT_50 50
#define TIMEOUT_100 100
#define TIMEOUT_200 200
#define TIMEOUT_500 500
#define TIMEOUT_1000 1000
#define TIMEOUT_2000 2000
#define TIMEOUT_3000 3000
#define TIMEOUT_5000 5000
#define ENABLE_CONGIN_MOD_TIME_SPAN 5000
#define GSM_OPERATIONS_TIMOUT_TIME_SPAN 120000

//Number of attempts to connect to server
#define SERVER_COMMUNICATION_MAX_RETRY_NUMBER 5


#define TRUE 1
#define FALSE !TRUE

#endif /* SYSTEM_H_ */