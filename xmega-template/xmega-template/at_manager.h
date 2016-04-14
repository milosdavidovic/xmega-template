/*
 * at_menaager.h
 *
 * Created: 21-Sep-15 11:26:30 AM
 *  Author: luka
 */ 


#ifndef AT_H_
#define AT_H_

#include <avr/io.h>
#include "AT_xmega.h"
#include "Usart.h"

void at_manager_init(char port);
void manage_at_commands();
void manage_special_at_commands();

#endif /* AT_H_ */