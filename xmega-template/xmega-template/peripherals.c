/*
 * peripherals.c
 *
 * Created: 7/29/2015 14:41:09
 *  Author: marko.tikvic
 */ 

#include <avr/io.h>
#include "peripherals.h"
#include "power.h"

//To be called upon device restart. Function initializes PORT configuration,
//and high power consumption device state is on (all switches are on)
void perinit(void)
{
	//Telit module
	perdirset(TELIT_PWR_PORT, TELIT_PWR_PIN, 1);
	perclear(TELIT_PWR_PORT, TELIT_PWR_PIN);
    perdirset(TELIT_PWR_MONITOR_PORT, TELIT_PWR_MONITOR_PIN, 0);
	__PORT_PULLUP((*TELIT_PWR_MONITOR_PORT),TELIT_PWR_MONITOR_BITMASK);
	perdirset(TELIT_RST_PORT,TELIT_RST_PIN,1);
	perclear(TELIT_RST_PORT,TELIT_RST_PIN);
	
	//Ultrasound distance sensor
	perdirset(ULTRASOUND_PORT, ULTRASOUND_PWR_PIN, 1);
	perset(ULTRASOUND_PORT,ULTRASOUND_PWR_PIN);
	
	//Temperature sensor
    perdirset(TEMPERATURE_PORT, TEMPERATURE_PWR_PIN, 1);
	perset(TEMPERATURE_PORT,TEMPERATURE_PWR_PIN);
	
	//Power boost control
	perdirset(POWER_BOOST_PORT,POWER_BOOST_PIN,1);
	perset(POWER_BOOST_PORT, POWER_BOOST_PIN);
	
	//PORTA
	perdirset(ADDITIONAL_PORT, ADDITIONAL_PWR_PIN, 1);
	perclear(ADDITIONAL_PORT, ADDITIONAL_PWR_PIN);
	
	//LED
	perdirset(LED_PORT,LED_PIN,1);
	perclear(LED_PORT, LED_PIN);
}

//To be called upon wake event. Function initializes PORT configuration,
//but all power switches remain of and device is in low power consumption mode. 
//It is up to user to power up power supply for peripheral he is using.
void perinitonwake(void)
{
	//Telit module
	perdirset(TELIT_PWR_PORT, TELIT_PWR_PIN, 1);
	perset(TELIT_PWR_PORT, TELIT_PWR_PIN);
	perdirset(TELIT_PWR_MONITOR_PORT, TELIT_PWR_MONITOR_PIN, 0);
	__PORT_PULLUP((*TELIT_PWR_MONITOR_PORT),TELIT_PWR_MONITOR_BITMASK);
	
	//Ultrasound distance sensor
	perdirset(ULTRASOUND_PORT, ULTRASOUND_PWR_PIN, 1);
	perclear(ULTRASOUND_PORT,ULTRASOUND_PWR_PIN);
	
	//Temperature sensor
	perdirset(TEMPERATURE_PORT, TEMPERATURE_PWR_PIN, 1);
	perclear(TEMPERATURE_PORT,TEMPERATURE_PWR_PIN);
	
	//Power boost control
	perdirset(POWER_BOOST_PORT,POWER_BOOST_PIN,1);
	perclear(POWER_BOOST_PORT, POWER_BOOST_PIN);
	
	//PORTA
	perdirset(ADDITIONAL_PORT, ADDITIONAL_PWR_PIN, 1);
	perclear(ADDITIONAL_PORT, ADDITIONAL_PWR_PIN);
	
	//LED
	perdirset(LED_PORT,LED_PIN,1);
	perclear(LED_PORT, LED_PIN);
}

void persleep(void)
{
	//Set all pins output low
	ALL_OUT();
	ALL_LOW();
	
	//TELIT POWER OFF
	perset(TELIT_PWR_PORT, TELIT_PWR_PIN);
	perset(FLASH_PORT,FLASH_CHIPS_PIN);
	
	
}

void perset(PORT_t *port, uint8_t pin)
{
    port->OUT |= (1 << pin);
}

void perclear(PORT_t *port, uint8_t pin)
{
    port->OUT &= ~(1 << pin);
}

void pertgl(PORT_t *port, uint8_t pin)
{
	port->OUTTGL = (1 << pin);
}

int perget(PORT_t *port, uint8_t pin)
{
    uint8_t temp = 0x00;
    temp = port->IN;

    if((temp & (1 << pin)) != 0) return 1;
    else return 0;
}

void perdirset(PORT_t *port, uint8_t pin, uint8_t direction)
{
	if (direction)
	{
		port->DIR |= (1 << pin);
	} 
	else
	{
		port->DIR &= ~(1 << pin);	
	}
}

// Enabling pull-up on all I/O pins.
void ENABLE_PULLUP(void)
{
	__PORT_PULLUP(PORTA, 0xFF);
	__PORT_PULLUP(PORTB, 0xFF);
	__PORT_PULLUP(PORTC, 0xFF);
	__PORT_PULLUP(PORTD, 0xFF);
	__PORT_PULLUP(PORTE, 0xFF);
	__PORT_PULLUP(PORTF, 0xFF);
	__PORT_PULLUP(PORTH, 0xFF);
	__PORT_PULLUP(PORTJ, 0xFF);
	__PORT_PULLUP(PORTK, 0xFF);
	__PORT_PULLUP(PORTQ, 0x0F);
	__PORT_PULLUP(PORTR, 0x03);
}

// Disabling pull-up on all I/O pins.
void ENABLE_PULLDOWN(void)
{
	__PORT_PULLDOWN(PORTA, 0xFF);
	__PORT_PULLDOWN(PORTB, 0xFF);
	__PORT_PULLDOWN(PORTC, 0xFF);
	__PORT_PULLDOWN(PORTD, 0xFF);
	__PORT_PULLDOWN(PORTE, 0xFF);
	__PORT_PULLDOWN(PORTF, 0xFF);
	__PORT_PULLDOWN(PORTH, 0xFF);
	__PORT_PULLDOWN(PORTJ, 0xFF);
	__PORT_PULLDOWN(PORTK, 0xFF);
	__PORT_PULLDOWN(PORTQ, 0x0F);
	__PORT_PULLDOWN(PORTR, 0x03);
}

void ALL_OUT(void)
{
	PORTA.DIRSET = 0xff;
	PORTB.DIRSET = 0xff;
	PORTC.DIRSET = 0xff;
	PORTD.DIRSET = 0xff;
	PORTE.DIRSET = 0xff;
	PORTF.DIRSET = 0xff;
	PORTH.DIRSET = 0xff;
	PORTJ.DIRSET = 0xff;
	PORTK.DIRSET = 0xff;
	PORTQ.DIRSET = 0xff;
	PORTR.DIRSET = 0xff;
}

void ALL_IN(void)
{
	PORTA.DIRCLR = 0xff;
	PORTB.DIRCLR = 0xff;
	PORTC.DIRCLR = 0xff;
	PORTD.DIRCLR = 0xff;
	PORTE.DIRCLR = 0xff;
	PORTF.DIRCLR = 0xff;
	PORTH.DIRCLR = 0xff;
	PORTJ.DIRCLR = 0xff;
	PORTK.DIRCLR = 0xff;
	PORTQ.DIRCLR = 0xff;
	PORTR.DIRCLR = 0xff;
}

void ALL_LOW(void)
{
	PORTA.OUTCLR = 0xff;
	PORTB.OUTCLR = 0xff;
	PORTC.OUTCLR = 0xff;
	PORTD.OUTCLR = 0xff;
	PORTE.OUTCLR = 0xff;
	PORTF.OUTCLR = 0xff;
	PORTH.OUTCLR = 0xff;
	PORTJ.OUTCLR = 0xff;
	PORTK.OUTCLR = 0xff;
	PORTQ.OUTCLR = 0xff;
	PORTR.OUTCLR = 0xff;
}

void ALL_HIGH(void)
{
	PORTA.OUTSET = 0xff;
	PORTB.OUTSET = 0xff;
	PORTC.OUTSET = 0xff;
	PORTD.OUTSET = 0xff;
	PORTE.OUTSET = 0xff;
	PORTF.OUTSET = 0xff;
	PORTH.OUTSET = 0xff;
	PORTJ.OUTSET = 0xff;
	PORTK.OUTSET = 0xff;
	PORTQ.OUTSET = 0xff;
	PORTR.OUTSET = 0xff;
}


