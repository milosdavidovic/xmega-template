/*
 * ds18b20.c
 *
 * Created: 6/7/2015 12:04:01 PM
 *  Author: milos.davidovic
 */ 

/*****************************************************************************\
 Example:

s_ds18b20 ds18b20_1;
oneWireDevice onewiredevice_1;

int main(void)
{
	
	initVar();	//initialise global variables
	initXmega();	//initialise mcu
	init_usart_F0();	//initialise usart F0
	oneWireInit(&onewiredevice_1, PIN4_bm);		//initialise 1wire interface
	init_ds1820(&ds18b20_1, &onewiredevice_1);		//initialise ds1820 sensor

	while(1)
	{
		_delay_ms(1000);
		read_temp_ds18b20_struct(&ds18b20_1);	//Temperature is now exesible with: ds18b20_1.temperature
	}

\*****************************************************************************/


#include "ds18b20.h"


#define WAIT_CONST_TIME_FOR_TEMP_CALC
void  init_ds1820(s_ds18b20 * p_ds18b20, oneWireDevice * owd)
{
	p_ds18b20 -> oWireDevice = owd; 
}

/*
 Read the temperature from sensor
 Parameter bus is bitmask of the bus that the device is on
 Returns DS18B20_ERROR (-1000) if there was an error while reading temp. 
*/
float read_temp_ds18b20(uint8_t bus)
{
		int16_t temperature=0;
		unsigned char device_id [8];
		unsigned char * p_device_id = device_id;
			
		//Send reset, detect presence	
		if(!oneWireDetectPresence(bus))
		{
			return DS18B20_ERROR;
		}
		//Read device ID
		oneWireReadRom(p_device_id, bus );
		//Send reset, detect presence	
		if(!oneWireDetectPresence(bus))
		{
			return DS18B20_ERROR;
		}
		// Match the id found earlier.
		oneWireMatchRom(p_device_id, bus);
		// Send start conversion command.
		oneWireSendByte(DS18B20_START_CONVERSION, bus);
		//Wait until conversion finish
		#ifdef ONEWIRE_USE_PARASITIC_POWER
		ONEWIRE_RELEASE_BUS(bus);
		_delay_ms(850);
		#else
		while (!oneWireReadBit(bus))
		;
		#endif	
		//Send reset, detect presence	
		if(!oneWireDetectPresence(bus))
		{
			return DS18B20_ERROR;
		}
		// Match the id found earlier.
		oneWireMatchRom(p_device_id, bus);		
		// Send READ SCRATCHPAD command.
		oneWireSendByte(DS18B20_READ_SCRATCHPAD, bus);
		// Read only two first bytes (temperature low, temperature high)
		temperature = oneWireReceiveByte(bus);
		temperature |= (oneWireReceiveByte(bus) << 8);
		//Calculate temperature according to ds18b20 datasheet and return value
		return 0.0625* (float)temperature;
}
/*
 Read the temperature from sensor
 Parameter temp_sensor is s_ds18b20 stuct  for temp sensor
 Returns DS18B20_ERROR (-1000) if there was an error while reading temp. 
*/
float read_temp_ds18b20_s(s_ds18b20 * temp_sensor)
{
	int16_t temp=0;
	unsigned char * p_device_id = temp_sensor->oWireDevice->id;
	unsigned char bus = temp_sensor->oWireDevice->bus;
	
	// Reset, presence.
	if(!oneWireDetectPresence(temp_sensor->oWireDevice->bus))
	{
		return DS18B20_ERROR;
	}
	//Read device ID
	oneWireReadRom(p_device_id, bus );
	//Send reset, detect presence	
	if(!oneWireDetectPresence(bus))
	{
		return DS18B20_ERROR;
	}
	// Match the id found earlier.
	oneWireMatchRom(p_device_id, bus);
	// Send start conversion command.
	oneWireSendByte(DS18B20_START_CONVERSION, bus);
	//Wait until conversion finish
	#ifdef WAIT_CONST_TIME_FOR_TEMP_CALC
	ONEWIRE_RELEASE_BUS(bus);
	_delay_ms(850);
	#else
	while (!oneWireReadBit(bus))
	;
	#endif
	//Send reset, detect presence
	if(!oneWireDetectPresence(bus))
	{
		return DS18B20_ERROR;
	}
	// Match the id found earlier.
	oneWireMatchRom(p_device_id, bus);
	// Send READ SCRATCHPAD command.
	oneWireSendByte(DS18B20_READ_SCRATCHPAD, bus);
	// Read only two first bytes (temperature low, temperature high)
	temp = oneWireReceiveByte(bus);
	temp |= (oneWireReceiveByte(bus) << 8);
	//Calculate temperature according to ds18b20 datasheet and return value
	temp_sensor -> temperature = 0.0625* (float)temp;
	return 0.0625* (float)temp;
}