/*
 * peripherals.h
 *
 * Created: 7/29/2015 14:41:22
 *  Author: marko.tikvic
 */ 


#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include <avr/io.h>

#define  ONEWIRE_PORT        		    PORTK.OUT    // 1-Wire Output register
#define  ONEWIRE_IN         	        PORTK.IN     // 1-Wire Input register
#define  ONEWIRE_DIR         		    PORTK.DIR    // 1-Wire Direction register

#define TELIT_PWR_PORT					((PORT_t*)&PORTD)
#define TELIT_PWR_PIN					PIN0_bp
#define TELIT_PWR_BITMASK				PIN0_bm

#define TELIT_PWR_MONITOR_PORT          ((PORT_t*)&PORTE)
#define TELIT_PWR_MONITOR_PIN           PIN7_bp
#define TELIT_PWR_MONITOR_BITMASK       PIN7_bm

#define TELIT_RST_PORT					((PORT_t*)&PORTE)
#define TELIT_RST_PIN					PIN5_bp
#define TELIT_RST_BITMASK				PIN5_bm

#define ULTRASOUND_PORT					((PORT_t*)&PORTH)
#define ULTRASOUND_PWR_PIN				PIN2_bp

#define TEMPERATURE_PORT				((PORT_t*)&PORTJ)
#define TEMPERATURE_PWR_PIN				PIN7_bp

#define ADDITIONAL_PORT					((PORT_t*)&PORTA)
#define ADDITIONAL_PWR_PIN				PIN1_bp

#define POWER_BOOST_PORT			    ((PORT_t*)&PORTK)
#define POWER_BOOST_PIN				    PIN1_bp

#define LED_PORT						((PORT_t*)&PORTB)
#define LED_PIN							PIN0_bp

#define ONEWIRE_PIN_BITMASK 			PIN0_bm

#define FLASH_PORT					    ((PORT_t*)&PORTF)
#define FLASH_CHIPS_PIN					PIN4_bp


void perinit(void);
void perinitonwake(void);
void persleep(void);
void perset(PORT_t *port, uint8_t pin);
void perclear(PORT_t *port, uint8_t pin);
void pertgl(PORT_t *port, uint8_t pin);
int  perget(PORT_t *port, uint8_t pin);
void perdirset(PORT_t *port, uint8_t pin, uint8_t direction);
void ENABLE_PULLUP(void);
void ENABLE_PULLDOWN(void);
void ALL_OUT(void);
void ALL_IN(void);
void ALL_LOW(void);
void ALL_HIGH(void);
void WM_Sleep(uint16_t seconds);
void WM_Wake(void);

#endif /* PERIPHERALS_H_ */