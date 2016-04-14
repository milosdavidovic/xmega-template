/*
 * at_menager.c
 *
 * Created: 21-Sep-15 11:26:19 AM
 *  Author: luka
 */ 

#include "at_manager.h"

char (*USART_get_receiver_status)();


void at_manager_init(char port)
{
	if(port == 'C' || port == 'c')
	{
		USART_get_receiver_status = &USARTC0_get_receiver_status;
	}
	else if(port == 'D' || port == 'd')
	{
		USART_get_receiver_status = &USARTD0_get_receiver_status;
	}
	else if(port == 'E' || port == 'e')
	{
		USART_get_receiver_status = &USARTE0_get_receiver_status;
	}
	else if(port == 'F' || port == 'f')
	{
		USART_get_receiver_status = &USARTF0_get_receiver_status;
	}
}

void manage_at_commands()
{
	int command_id = -1;
	
	if((*USART_get_receiver_status)())
	{
		command_id = at_compare();
		at_handle(command_id);
	}
}

//ova special je samo da bi mogli da izadjemo iz telit moda
void manage_special_at_commands()
{
	int command_id = -1;
	
	if((*USART_get_receiver_status)())
	{
		command_id = special_at_compare();
		special_at_handle(command_id);
	}
}