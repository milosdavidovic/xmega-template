/*
 * trash_sonar_mb7137.h
 *
 * Created: 27/7/2015 12:26:00 PM
 *  Author: milos.davidovic
 */ 



#ifndef TRASH_SONAR_MB7137_H_
#define TRASH_SONAR_MB7137_H_

#include "twi.h"

#define MIN_RANGE_VALUE 20
#define MAX_RANGE_VALUE 350

//This value defines area for searching the most common value in range samples array 
#define CLOSEST_NEIGHBORS_RANGE 3

//Number of samples taken from sensor for each measurment
#define NUMBER_OF_SAMPLES 20

/*! Defining an example twi slave address. */
#define TWI_SLAVE_ADDRESS    0xE0

/*! Defining number of bytes in buffer. */
#define NUM_BYTES   2

/*! Defining error range value */
#define TRASH_SONAR_ERROR 0

#define TRASH_SONAR_BELOW_RANGE 5
#define TRASH_SONAR_EXIDED_RANGE 10

/*! Defining timer counter used  */
#define TRASH_TIMEOUT_COUNTER 2

/*! Defining default timeout values */
#define TRASH_TIMEOUT_100 100
#define TRASH_TIMEOUT_500 500
#define TRASH_TIMEOUT_1000 1000

/*! CPU speed 2MHz, BAUDRATE 100kHz and Baudrate Register Settings */
#define CPU_SPEED   7372800
#define BAUDRATE     100000
#define TWI_BAUDSETTING TWI_BAUD(CPU_SPEED, BAUDRATE)

/*****************************************************************************\
|* MB7137 functions
\*****************************************************************************/
#define MB7137_WRITE_RANGE        0x51
#define MB7137_ADDR_UNLOCK_1      0xAA
#define MB7137_ADDR_UNLOCK_2      0xA5

typedef struct
{
	uint8_t address;		// Address of the device
	uint16_t range;		//Last reported range
	uint16_t range_filtered;	//Last reported range taken from n samples
} Trash_Sonar_MB7137_t;


void read_range(Trash_Sonar_MB7137_t *sensor, TWI_Master_t *twiMaster);
void read_range_mcv(Trash_Sonar_MB7137_t *sensor, TWI_Master_t *twiMaster);
void Init_mb7137_sensor(Trash_Sonar_MB7137_t *sensor);
uint8_t trash_timeout();

#endif /* TRASH_SONAR_MB7137_H_ */