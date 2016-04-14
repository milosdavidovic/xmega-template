/*
 * AT_XMEGA.h
 *
 * Created: 7/14/2015 11:40:43
 *  Author: marko.tikvic
 */ 


#ifndef AT_XMEGA_H_
#define AT_XMEGA_H_

#include <avr/io.h>
#include "Usart.h"
#include "system.h"

#define ERR_VAL ((int)-1)

void at_init(char uart_port);
int at_compare();
void at_handle(int id);

int special_at_compare();
void special_at_handle(int id);

void (*AT_UART_send_str)(char *str);
void (*AT_UART_clear_buffer)(void);
void (*AT_UART_send_new_line)(void);

#endif /* AT_XMEGA_H_ */