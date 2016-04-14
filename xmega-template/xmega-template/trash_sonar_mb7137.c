/*
 * trash_sonar_mb7137.c
 *
 * Created: 27/7/2015 12:25:52 PM
 *  Author: milos.davidovic
 */ 

/*
** Sensor notes **

Works with 3V to 5.5V supply. Typically, adding a 100uF capacitor at the sensor between
the V+ and GND pins will correct most power related electrical noise issues.

Sonar range information from 20-cm out to 350-cm with 1-cm resolution

Sensor works with clocks up to 100kHz.

When power is supplied on V+ pin, sensor will take <135mS to get ready for reading.
After the sensor is commanded to take a range reading it sends an ultrasonic pulse,
waits between ~15ms to ~70ms to detect a target, and determines the range.
Then the sensor will resume I2C communications. If the sensor is addressed while
in the middle of a range reading, all requests for communication will be responded
with a NACK (not acknowledge). It is best to allow 100ms between readings to allow for
proper acoustic dissipation.

For best operation, it is recommended that end users conduct filtering of the sensor data
to achieve the most reliable results.
Filtering Examples
• History Filter — Verifies the reading is accurate when compared to the previous ranges.
• Mode Filter — Output the sensors reported range that is shown the most.
• Median filter — Output the reported range that is in the center of a range set.

*/

#include "system.h"
#include "trash_sonar_mb7137.h"

/*
 Read the range from ultrasonic sensor.
 Parameter sensor is Trash_Sonar_MB7137_t struct pointer and parameter twiMaster 
 is TWI_Master_t struct pointer for ultrasonic sensor sensor.
 Returns TRASH_SONAR_ERROR (0) if there was an error while reading range.
*/
void read_range(Trash_Sonar_MB7137_t *sensor, TWI_Master_t *twiMaster)
{
	uint8_t bites_to_write[1];
	TWI_MasterFlushReadData(twiMaster);
	bites_to_write[0]=MB7137_WRITE_RANGE;	//writing read range command in bites_to_write
	TWI_MasterWriteRead(twiMaster, sensor->address , bites_to_write, 1, 0); //Sending read range command
	_delay_ms(80); // Waiting for sensor to finish measurement
	TWI_MasterWriteRead(twiMaster, sensor->address, 0, 0, 2);	//Reading new range value
	setTCO_counter(TRASH_TIMEOUT_COUNTER, TRASH_TIMEOUT_500);		//set timeout counter 
	while (twiMaster->status != TWIM_STATUS_READY)	// Wait until transaction is complete.
	{
		if (trash_timeout())
		{
			sensor->range = (uint16_t)TRASH_SONAR_ERROR;	//Reading range timeouted, set error value for range
			return;
		}
	}
	//	Calculation sensor range in cm
	sensor->range = twiMaster->readData[0];
	sensor->range = sensor->range << 8;
	sensor->range += twiMaster->readData[1];
}

/*
 Read the range from ultrasonic sensor.
 This function makes n range samples and returns most common value.
 Time between samples is 100ms and this could be less in required, 
 but the minimum time is described in sensor datasheet.
*/
void read_range_mcv(Trash_Sonar_MB7137_t *sensor, TWI_Master_t *twiMaster)
{
    uint16_t samples_number=NUMBER_OF_SAMPLES;	//number of samples to read from sesor
	uint16_t most_frequent_array [MAX_RANGE_VALUE+1+CLOSEST_NEIGHBORS_RANGE];	//Array for finding most common read value (index of array is read value)

    uint16_t most_frequent_index_temp=0;	//Current candidate's index (range value) for 
	uint16_t most_frequent_sum_temp=0;	//Current candidate's number of repetitions
	uint16_t most_frequent_sum_neighbors_temp=0;	//Current candidate's neighbors sum of repetitions
	
	uint16_t most_frequent_sum_neighbors_current=0;		//Current index's neighbors sum of repetitions

	uint16_t range = 0;	

	//Clear most frequent array
	for (int i =0;i<MAX_RANGE_VALUE+1+CLOSEST_NEIGHBORS_RANGE;i++)
	{
		most_frequent_array[i]=0;
	}
	//Take range samples
	while(samples_number)
	{
		read_range(sensor, twiMaster);
		range = sensor->range;
		
		if (range>=MIN_RANGE_VALUE && range<=MAX_RANGE_VALUE)
		{
			//Reading ok
			most_frequent_array[range]++;
		} 
		else if (range<MIN_RANGE_VALUE)
		{
			//Sensor failure (reports below range)
			most_frequent_array[TRASH_SONAR_BELOW_RANGE]++;  
		}
		else
		{
			//Sensor exides range
			most_frequent_array[TRASH_SONAR_EXIDED_RANGE]++;  
		}
		samples_number--;
		_delay_ms(10);
	}
	
	//***********FOR TESTING*********************
	/*for (int i =0;i<MAX_RANGE_VALUE+1+CLOSEST_NEIGHBORS;i++)
	{
		most_frequent_array[i]=0;
	}
	most_frequent_array[20] = 100;
	most_frequent_array[21] = 100;
	most_frequent_array[22] = 101;
	most_frequent_array[23] = 100;
	most_frequent_array[103] = 1;
	most_frequent_array[200] = 10;
	most_frequent_array[250] = 1;
	most_frequent_array[300] = 4;
	most_frequent_array[320] = 3;
	most_frequent_array[MAX_RANGE_VALUE-1] = 120;*/
	//**********************************
	
	//Find most common range value from taken samples. 
	//Functions also checks neighbors values, for more accurate measurement. 
	//For example if samples are as follows: {50,50,51,51,52,52,350,350,350,350}, 
	//reported most common value is 51 (not 350).
	for (int i = CLOSEST_NEIGHBORS_RANGE;i<MAX_RANGE_VALUE+1;i++)
	{
		most_frequent_sum_neighbors_current = 0;
		
		for (int j= i - CLOSEST_NEIGHBORS_RANGE;j< i + CLOSEST_NEIGHBORS_RANGE + 1;j++)
		{
			most_frequent_sum_neighbors_current += most_frequent_array[j];
		}
		
		if (most_frequent_sum_neighbors_current > most_frequent_sum_neighbors_temp)
		{
			most_frequent_index_temp=i;
			most_frequent_sum_neighbors_temp=most_frequent_sum_neighbors_current;
			most_frequent_sum_temp=most_frequent_array[i];
		}
		else if(most_frequent_sum_neighbors_current == most_frequent_sum_neighbors_temp)
		{
			if (most_frequent_array[i] > most_frequent_sum_temp)
			{
				most_frequent_index_temp=i;
				most_frequent_sum_neighbors_temp=most_frequent_sum_neighbors_current;
				most_frequent_sum_temp=most_frequent_array[i];
			}
		}
	}
	//Set most common value sa trash sensor fileted value
	//trashSensor.range_filtered = most_frequent_index_temp;
	
}

/*
 Init function for ultrasonic sensor.
*/
void Init_mb7137_sensor(Trash_Sonar_MB7137_t *sensor)
{
	sensor->address = TWI_SLAVE_ADDRESS;
	sensor->range = MAX_RANGE_VALUE;
	sensor->range_filtered = MAX_RANGE_VALUE;
}

/*
 Operation timeout function for ultrasonic sensor.
*/
uint8_t trash_timeout()
{
	if (!getTCO_counter(TRASH_TIMEOUT_COUNTER))
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}