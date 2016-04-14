/*
 * ADC.h
 *
 * Created: 13-Jul-15 09:27:49 AM
 *  Author: luka
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include "util/delay.h"

/*
Initialize ADC converter on port A
@_ref_voltage-> 0 for 1.0V, 1 for VCC/1.6V 
@_input_mode-> 0 for single-ended, 1 for differential mode
Note: ADC is always in signed mode. This is fixed.
*/
void ADCA_init(uint8_t _ref_voltage, uint8_t _input_mode);

/*
Reads ADC conversion result on desired pin of port A
@pin_number-> desired pin (0-7)
RETURN-> int value from ADC conversion
*/
int16_t ADCA_get_value(uint8_t pin_number);

/*
Returns voltage on desired pin
@pin-> Pin from which to read voltage input
RETURN-> float value, voltage on desired pin
*/
float ADCA_get_voltage(uint8_t pin);

/*
Sets gain on desired pin
@pin-> desired pin
@gain-> gain to be set 
valid values for gain are 1, 2, 4, 8, 16, 32 and 64
RETURN-> float value, voltage on desired pin
*/
void ADCA_set_pin_gain(uint8_t pin, uint8_t gain);




/*
Initialize ADC converter on port B
@_ref_voltage-> 0 for 1.0V, 1 for VCC/1.6V
@_input_mode-> 0 for single-ended, 1 for differential mode
Note: ADC is always in signed mode. This is fixed.
*/
void ADCB_init(uint8_t _ref_voltage, uint8_t _input_mode);

/*
Reads ADC conversion result on desired pin of port A
@pin_number-> desired pin (0-7)
RETURN-> int value from ADC conversion
*/
int16_t ADCB_get_value(uint8_t pin_number);

/*
Returns voltage on desired pin
@pin-> Pin from which to read voltage input
RETURN-> float value, voltage on desired pin
*/
float ADCB_get_voltage(uint8_t pin);

/*
Sets gain on desired pin
@pin-> desired pin
@gain-> gain to be set
valid values for gain are 1, 2, 4, 8, 16, 32 and 64
RETURN-> float value, voltage on desired pin
*/
void ADCB_set_pin_gain(uint8_t pin, uint8_t gain);

#endif /* ADC_H_ */