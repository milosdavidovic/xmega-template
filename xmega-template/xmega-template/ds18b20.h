/*
 * ds18b20.h
 *
 * Created: 6/7/2015 12:04:19 PM
 *  Author: milos.davidovic
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_


/*****************************************************************************\
|* No support for the interrupt-driven UART interface.
|* Read notes in onewire library
\*****************************************************************************/

#include "1wire.h"

/*****************************************************************************\
|* DS18b20 functions 
\*****************************************************************************/
#define DS18B20_START_CONVERSION         0x44
#define DS18B20_READ_SCRATCHPAD          0xbe

/*****************************************************************************\
|* DS18b20 error
\*****************************************************************************/
#define DS18B20_ERROR                    -1000

/*****************************************************************************\
|* DS18b20 family ID
\*****************************************************************************/
#define DS18B20_FAMILY_ID                0x28


typedef struct {
	float temperature;
	oneWireDevice * oWireDevice;
} s_ds18b20;

float ds18b20ReadTemperature(uint8_t bus, uint8_t * id);
float read_temp_ds18b20_s(s_ds18b20 *temp_sensor);
float read_temp_ds18b20(uint8_t bus);
void  init_ds1820(s_ds18b20 * p_ds18b20, oneWireDevice * owd);


#endif /* DS18B20_H_ */