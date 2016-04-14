/*
 * power.c
 *
 * Created: 23/9/2015 11:17:52 AM
 *  Author: milos.davidovic
 */ 

#include "power.h"
#include "peripherals.h"

// Disabling the JTAG interface.
void DISABLE_JTAG(void) 
{ 
	CCP = CCP_IOREG_gc; 
	MCU_MCUCR = MCU_JTAGD_bm; 
}

// Disconnecting general modules from clock.
void DISABLE_GENERAL(void) 
{ 
	PR.PRGEN |= PR_AES_bm | PR_DMA_bm | PR_EBI_bm |  0x40; 
}

// Connecting general modules to clock.
void ENABLE_GENERAL(void)
{
	PR.PRGEN &= PR_AES_bm & ~PR_DMA_bm & ~PR_EBI_bm & ~0x40;
}

// Disconnecting timer/counters and associated extensions from clock.
void DISABLE_TIMERS(void)
{ 
	PR.PRPC |= PR_HIRES_bm | PR_TC0_bm | PR_TC1_bm; 
	PR.PRPD |= PR_HIRES_bm | PR_TC0_bm | PR_TC1_bm; 
	PR.PRPE |= PR_HIRES_bm | PR_TC0_bm | PR_TC1_bm; 
	PR.PRPF |= PR_HIRES_bm | PR_TC0_bm | PR_TC1_bm; 
}
// Connecting timer/counters and associated extensions from clock.

void ENABLE_TIMERS(void)
{
	PR.PRPC &= PR_HIRES_bm & ~PR_TC0_bm & ~PR_TC1_bm;
	PR.PRPD &= PR_HIRES_bm & ~PR_TC0_bm & ~PR_TC1_bm;
	PR.PRPE &= PR_HIRES_bm & ~PR_TC0_bm & ~PR_TC1_bm;
	PR.PRPF &= PR_HIRES_bm & ~PR_TC0_bm & ~PR_TC1_bm;
}

// Disconnecting communication interfaces from clock.
void DISABLE_COMMUNICATION(void) 
{ 
	PR.PRPC |= PR_SPI_bm | PR_TWI_bm | PR_USART0_bm | PR_USART1_bm; 
	PR.PRPD |= PR_SPI_bm | PR_TWI_bm | PR_USART0_bm | PR_USART1_bm; 
	PR.PRPE |= PR_SPI_bm | PR_TWI_bm | PR_USART0_bm | PR_USART1_bm; 
	PR.PRPF |= PR_SPI_bm | PR_TWI_bm | PR_USART0_bm | PR_USART1_bm; 
}

// Connecting communication interfaces from clock.
void ENABLE_COMMUNICATION(void)
{
	PR.PRPC &= ~PR_SPI_bm & ~PR_TWI_bm & ~PR_USART0_bm & ~PR_USART1_bm;
	PR.PRPD &= ~PR_SPI_bm & ~PR_TWI_bm & ~PR_USART0_bm & ~PR_USART1_bm;
	PR.PRPE &= ~PR_SPI_bm & ~PR_TWI_bm & ~PR_USART0_bm & ~PR_USART1_bm;
	PR.PRPF &= ~PR_SPI_bm & ~PR_TWI_bm & ~PR_USART0_bm & ~PR_USART1_bm;
}

// Disconnecting analog modules from clock.
void DISABLE_ANLG(void) 
{ 
	PR.PRPA |= PR_AC_bm | PR_ADC_bm | PR_DAC_bm;
	PR.PRPB |= PR_AC_bm | PR_ADC_bm | PR_DAC_bm;
}

// Connecting analog modules from clock.
void ENABLE_ANLG(void)
{
	PR.PRPA &= PR_AC_bm & ~PR_ADC_bm & ~PR_DAC_bm;
	PR.PRPB &= PR_AC_bm & ~PR_ADC_bm & ~PR_DAC_bm;
}

// Putting controller to POWER_SAVE mode for desired period of time.
void WM_Sleep(uint16_t seconds)
{
	DISABLE_JTAG();
	DISABLE_GENERAL();
	DISABLE_TIMERS();
	DISABLE_COMMUNICATION();
	DISABLE_ANLG();

	persleep();
	
	watchdog_disable();
	
	RTC_change_period(seconds); 
	
	//after these instructions mcu clock stops
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	sleep_cpu();
	sleep_disable();
	
	WM_Wake();
	
	RTC_change_period(1); // TODO RTC period in active mode
}

// Connection all used modules to external clock source
void WM_Wake(void)
{
	//init ports for Telit module, Ultrasound distance sensor,
	//Temperature sensor, Flash memory, Additional Port and LED
	perinitonwake();
	
	ENABLE_GENERAL();
	ENABLE_TIMERS();
	ENABLE_COMMUNICATION();
	ENABLE_ANLG();
	
	watchdog_enable();
	
	USARTD0_init(9600);
	USARTE0_init(9600);

	init_gsm();
}

void gsm_power_on()
{
	perclear(TELIT_PWR_PORT,TELIT_PWR_PIN);
}

void gsm_power_off()
{
	perset(TELIT_PWR_PORT,TELIT_PWR_PIN);
}

void ultrasound_power_on()
{
	perset(ULTRASOUND_PORT,ULTRASOUND_PWR_PIN);
}

void ultrasound_power_off()
{
	perclear(ULTRASOUND_PORT,ULTRASOUND_PWR_PIN);
}

void temp_power_on()
{
	perset(TEMPERATURE_PORT,TEMPERATURE_PWR_PIN);
}

void temp_power_off()
{
	perclear(TEMPERATURE_PORT,TEMPERATURE_PWR_PIN);
}

void power_boost_on()
{
	perset(POWER_BOOST_PORT,POWER_BOOST_PIN);
}

void power_boost_off()
{
	perclear(POWER_BOOST_PORT,POWER_BOOST_PIN);
}

void led_on(uint8_t mode)
{
	switch(mode)
	{
		case 1:
		{
			if(!getTCO_counter(LED_TIMEOUT_COUNTER))
			{
				setTCO_counter(LED_TIMEOUT_COUNTER, 500);
				pertgl(LED_PORT, LED_PIN);
			}break;
		}
	}
}




