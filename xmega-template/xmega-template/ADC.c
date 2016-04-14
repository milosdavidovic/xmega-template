/*
 * ADC.c
 *
 * Created: 13-Jul-15 09:27:40 AM
 *  Author: luka
 */ 

/*
The ADC is either in signed or unsigned mode. This setting is global for the ADC and all ADC channels.
In signed mode, negative and positive results are generated. Signed mode must be used when any of the ADC channels
are set up for differential measurements. In unsigned mode, only single-ended or internal signals can be measured.

For differential measurements, GAIN is 1/2 to 64. For single-ended and internal measurements, GAIN is always 1 and
VINP is the internal ground.
*/

#include "ADC.h"

#define SAMPLE_SIZE 8
#define ADC_TOP_VALUE_UNSIGNED 4095
#define ADC_TOP_VALUE_SIGNED 2047
#define REF_VOLT_INT_1V 1
#define REF_VOLT_INT_VCC 2.0625
#define SINGLE_ENDED_MODE 0
#define DIFFERENTIAL_MODE 1

static float adca_ref_voltage = 0;
static uint8_t adca_input_mode = 0;
static uint8_t porta_pin_gains[8] = {1, 1, 1, 1, 1, 1, 1, 1};

static float adcb_ref_voltage = 0;
static uint8_t adcb_input_mode = 0;
static uint8_t portb_pin_gains[8] = {1, 1, 1, 1, 1, 1, 1, 1};

void ADCA_init(uint8_t _ref_voltage, uint8_t _input_mode)
{
	if(_input_mode == 0)
	{
		adca_input_mode = SINGLE_ENDED_MODE;
	}
	else if(_input_mode == 1)
	{
		adca_input_mode = DIFFERENTIAL_MODE;
	}
	
	ADCA_CTRLB |= ADC_CONMODE_bm; // signed mod
	
	if(_ref_voltage == 0)
	{
		ADCA_REFCTRL |= ADC_BANDGAP_bm; // Bandgap Enable (must be enabled when using internal 1.0V as ref)
		ADCA_REFCTRL |= ADC_REFSEL_INT1V_gc; // set reference voltage to internal 1.0V 
		adca_ref_voltage = REF_VOLT_INT_1V;
	}
	if(_ref_voltage == 1) 
	{
		ADCA_REFCTRL |= ADC_REFSEL_INTVCC_gc; // set reference voltage to internal VCC/1.6V
		adca_ref_voltage = REF_VOLT_INT_VCC;
	}
	
	ADCA_CTRLB |= ADC_RESOLUTION_12BIT_gc; //12-bit ADC resolution 
	ADCA_CTRLA |= ADC_ENABLE_bm; // Enable ADCA converter
	
	 _delay_us(500); // Wait at least 25 clocks for ADC to be stable
}

int16_t ADCA_get_value(uint8_t pin_number)
{
	uint8_t sample_number = 0;
	int16_t adc_sum = 0;
	int16_t adc_result = 0;
	
	PORTA.DIRCLR = (1 << pin_number);
	_delay_ms(200); //TODO ako onde nema delay adc nece da radi. Zasto?
	
	if(adca_input_mode == DIFFERENTIAL_MODE)
	{
		ADCA_CH0_CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_DIFF_gc; // input gain x1, differential mode
		ADCA_CH0_MUXCTRL |= ADC_CH_MUXNEG_PIN2_gc; // Vneg on pin2
	}
	else if(adca_input_mode == SINGLE_ENDED_MODE)
	{
		ADCA_CH0_CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc; // input gain x1, single-ended mode
	}
	
	if(pin_number == 0)	ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN0_gc; //pin0
	else if(pin_number == 1) ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN1_gc; //pin1
	else if(pin_number == 2) ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN2_gc; //pin2
	else if(pin_number == 3) ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN3_gc; //pin3
	else if(pin_number == 4) ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN4_gc; //pin4
	else if(pin_number == 5) ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN5_gc; //pin5
	else if(pin_number == 6) ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN6_gc; //pin6
	else if(pin_number == 7) ADCA_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN7_gc; //pin7
	
	while(sample_number < SAMPLE_SIZE)
	{
		ADCA_CH0_CTRL |= ADC_CH_START_bm; // start adc conversion
		while(ADCA_CH0_INTFLAGS == 0){} //wait for conversion to be done
		ADCA_CH0_INTFLAGS = 0x01; //reset flag
		adc_sum += ADCA_CH0_RES;
		sample_number++;
	}
	
	adc_result = adc_sum / SAMPLE_SIZE;
	
	return adc_result;
}

void ADCA_set_pin_gain(uint8_t pin, uint8_t gain)
{
	porta_pin_gains[pin] = gain;
}


float ADCA_get_voltage(uint8_t pin)
{
	float voltage = 0;
	int16_t adc_value  = 0;
	
	adc_value = ADCA_get_value(pin);
	voltage = ((float)(adc_value) * adca_ref_voltage) / (porta_pin_gains[pin] * (ADC_TOP_VALUE_SIGNED + 1));
	
	return voltage;
}


void ADCB_init(uint8_t _ref_voltage, uint8_t _input_mode)
{
	if(_input_mode == 0)
	{
		adcb_input_mode = SINGLE_ENDED_MODE;
	}
	else if(_input_mode == 1)
	{
		adcb_input_mode = DIFFERENTIAL_MODE;
	}
	
	ADCB_CTRLB |= ADC_CONMODE_bm; // signed mod
	
	if(_ref_voltage == 0)
	{
		ADCB_REFCTRL |= ADC_BANDGAP_bm; // Bandgap Enable (must be enabled when using internal 1.0V as ref)
		ADCB_REFCTRL |= ADC_REFSEL_INT1V_gc; // set reference voltage to internal 1.0V
		adcb_ref_voltage = REF_VOLT_INT_1V;
	}
	if(_ref_voltage == 1)
	{
		ADCB_REFCTRL |= ADC_REFSEL_INTVCC_gc; // set reference voltage to internal VCC/1.6V
		adcb_ref_voltage = REF_VOLT_INT_VCC;
	}
	
	ADCB_CTRLB |= ADC_RESOLUTION_12BIT_gc; //12-bit ADC resolution
	ADCB_CTRLA |= ADC_ENABLE_bm; // Enable ADCA converter
	
	_delay_us(500); // Wait at least 25 clocks for ADC to be stable
}

int16_t ADCB_get_value(uint8_t pin_number)
{
	uint8_t sample_number = 0;
	int16_t adc_sum = 0;
	int16_t adc_result = 0;
	
	if(adca_input_mode == DIFFERENTIAL_MODE)
	{
		ADCB_CH0_CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_DIFF_gc; // input gain x1, differential mode
		ADCB_CH0_MUXCTRL |= ADC_CH_MUXNEG_PIN2_gc; // Vneg on pin2
	}
	else if(adca_input_mode == SINGLE_ENDED_MODE)
	{
		ADCB_CH0_CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc; // input gain x1, single-ended mode
	}
	
	if(pin_number == 0)	ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN0_gc; //pin0
	else if(pin_number == 1) ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN1_gc; //pin1
	else if(pin_number == 2) ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN2_gc; //pin2
	else if(pin_number == 3) ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN3_gc; //pin3
	else if(pin_number == 4) ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN4_gc; //pin4
	else if(pin_number == 5) ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN5_gc; //pin5
	else if(pin_number == 6) ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN6_gc; //pin6
	else if(pin_number == 7) ADCB_CH0_MUXCTRL |=  ADC_CH_MUXPOS_PIN7_gc; //pin7
	
	while(sample_number < SAMPLE_SIZE)
	{
		ADCB_CH0_CTRL |= ADC_CH_START_bm; // start adc conversion
		while(ADCB_CH0_INTFLAGS == 0){} //wait for conversion to be done
		ADCB_CH0_INTFLAGS = 0x01; //reset flag
		adc_sum += ADCB_CH0_RES;
		sample_number++;
	}
	
	adc_result = adc_sum / SAMPLE_SIZE;
	
	return adc_result;
}

void ADCB_set_pin_gain(uint8_t pin, uint8_t gain)
{
	portb_pin_gains[pin] = gain;
}

float ADCB_get_voltage(uint8_t pin)
{
	float voltage = 0;
	int16_t adc_value  = 0;
	
	adc_value = ADCB_get_value(pin);
	voltage = ((float)(adc_value) * adcb_ref_voltage) / (portb_pin_gains[pin] * (ADC_TOP_VALUE_SIGNED + 1));
	
	return voltage;
}