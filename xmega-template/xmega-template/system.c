/*
 * sysinit.c
 *
 * Created: 8/11/2015 11:04:50
 *  Author: marko.tikvic
 */ 
 
#include "system.h"

/* Data to be sent to the server. */
static float v_bat = 0;                     // [4B]
static uint16_t trash_level = 0;            // [2B]
static float temperature = 0;               // [4B]
static uint32_t log_timestamp = 0;			// [4B]
static uint8_t output[19] = {'\0'};         // [18B] to be sent total  (15B data + 4B for log id)

uint8_t gsm_fail_safe_timer_started = 0;
uint8_t enable_cofiguration_mod_timer_started = 0;
uint8_t data_sending_in_progress = 0;
uint8_t config_mod_enabled = 0;
uint8_t telit_test_mode_enabled = 0;

uint32_t data_measurement_period = 0;
uint32_t data_sending_period = 0;

char metering_station_id[12] = "WM10-XXXXXX\0";



void init_system()
{
	/* Init system clock */
	//Extern 7.3728 MHz oscillator
	init_clock(OSC_XOSCEN_bm, (OSC_FRQRANGE_2TO9_gc | OSC_XOSCSEL_XTAL_16KCLK_gc));


}

//TWI Interupt routine
ISR(TWIF_TWIM_vect)
{
    //TWI_MasterInterruptHandler(&twiMaster);
}



