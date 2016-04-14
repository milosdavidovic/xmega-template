/*
 * usart.h
 *
 * Created: 01-Jul-15 09:38:02 AM
 *  Author: luka
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#define USARTC0_RECEIVE_BUFFER_SIZE 100
#define USARTD0_RECEIVE_BUFFER_SIZE 100
#define USARTE0_RECEIVE_BUFFER_SIZE 100
#define USARTF0_RECEIVE_BUFFER_SIZE 100

extern volatile char usartc0_receive_buffer[USARTC0_RECEIVE_BUFFER_SIZE];
extern volatile char usartd0_receive_buffer[USARTD0_RECEIVE_BUFFER_SIZE];
extern volatile char usarte0_receive_buffer[USARTE0_RECEIVE_BUFFER_SIZE];
extern volatile char usartf0_receive_buffer[USARTF0_RECEIVE_BUFFER_SIZE];

/*
Initialize USART on portC0 with desired baud rate
Note: Currently supported baud rates 2400, 4800 and 9600 @2Mhz internal clock
@baud_rate-> desired baud rate
*/
void USARTC0_init(uint16_t baud_rate);

/*
Sends byte over usartc0
@byte_to_send-> byte to send
*/
void USARTC0_send_byte(char byte_to_send);

/*
Sends string over usartc0
@string_pointer-> string to send
*/
void USARTC0_send_string(char* string_pointer);

/*
Clear receiver buffer
Note: Besides clearing the buffer it sets 'usartc0_message_received' flag to 0.
*/
void USARTC0_clear_receiver_buffer();

/*
Gets status of the receiver
Note: Message is received when end characters ('\r' and/or '\n') are received
RETURN-> char value 0(message not yet received) OR 1(message receievd)
*/
char USARTC0_get_receiver_status();

/*
Gets last received message
RETURN-> char pointer to received string
*/
char* USARTC0_get_received_message();

/*
Shifts out given number of characters from receiver buffer
@lenght-> number of characters to be shifted out
*/
void USARTC0_shift_receive_buffer(char lenght);

/*
Gets current receiver buffer size
RETURN-> char value of buffer size
*/
char USARTC0_get_receive_buffer_index();

/*
Sends new line
*/
void USARTC0_send_new_line();

/*
Sends string from program memory
@progmem_string_pointer-> string to send
*/
void USARTC0_send_string_P(PGM_P progmem_string_pointer);


/*
Initialize USART on portD0 with desired baud rate
Note: Currently supported baud rates 2400, 4800 and 9600 @2Mhz internal clock
@baud_rate-> desired baud rate
*/
void USARTD0_init(unsigned int baud_rate);

/*
Sends byte over usartd0
@byte_to_send-> byte to send
*/
void USARTD0_send_byte(char byte_to_send);

/*
Sends string over usartd0
@string_pointer-> string to send
*/
void USARTD0_send_string(char* string_pointer);

/*
Sends string from program memory
@progmem_string_pointer-> string to send
*/
void USARTD0_send_string_P(PGM_P progmem_string_pointer);

/*
Clear receiver buffer
Note: Besides clearing the buffer it sets 'usartd0_message_received' flag to 0.
*/
void USARTD0_clear_receiver_buffer();

/*
Gets status of the receiver
Note: Message is received when end characters ('\r' and/or '\n') are received
RETURN-> char value 0(message not yet received) OR 1(message receievd)
*/
char USARTD0_get_receiver_status();

/*
Gets last received message
RETURN-> char pointer to received string
*/
char* USARTD0_get_received_message();

/*
Shifts out given number of characters from receiver buffer
@lenght-> number of characters to be shifted out
*/
void USARTD0_shift_receive_buffer(char lenght);

/*
Gets current receiver buffer size
RETURN-> char value of buffer size
*/
char USARTD0_get_receive_buffer_index();

/*
Sends new line
*/
void USARTD0_send_new_line();

/*
*/
void USARTD0_print_int(unsigned int a);

/*
*/
void USARTD0_send_data(char* data_pointer, int lenght);

/*
*/
void USARTD0_print_float(float a);

/*
*/
void USARTD0_print_long(uint32_t a);

/*
Initialize USART on portE0 with desired baud rate
Note: Currently supported baud rates 2400, 4800 and 9600 @2Mhz internal clock
@baud_rate-> desired baud rate
*/
void USARTE0_init(unsigned int baud_rate);

/*
Sends byte over usarte0
@byte_to_send-> byte to send
*/
void USARTE0_send_byte(char byte_to_send);

/*
Sends string over usarte0
@string_pointer-> string to send
*/
void USARTE0_send_string(char* string_pointer);

void USARTE0_send_data(char* data_pointer, int lenght);

/*
Clear receiver buffer
Note: Besides clearing the buffer it sets 'usarte0_message_received' flag to 0.
*/
void USARTE0_clear_receiver_buffer();

/*
Gets status of the receiver
Note: Message is received when end characters ('\r' and/or '\n') are received
RETURN-> char value 0(message not yet received) OR 1(message receievd)
*/
char USARTE0_get_receiver_status();

/*
Gets last received message
RETURN-> char pointer to received string
*/
char* USARTE0_get_received_message();

/*
Shifts out given number of characters from receiver buffer
@lenght-> number of characters to be shifted out
*/
void USARTE0_shift_receive_buffer(char lenght);

/*
Gets current receiver buffer size
RETURN-> char value of buffer size
*/
char USARTE0_get_receive_buffer_index();

/*
Sends new line
*/
void USARTE0_send_new_line();

/*
Sends string from program memory
@progmem_string_pointer-> string to send
*/
void USARTE0_send_string_P(PGM_P progmem_string_pointer);



/*
Initialize USART on portF0 with desired baud rate
Note: Currently supported baud rates 2400, 4800 and 9600 @2Mhz internal clock
@baud_rate-> desired baud rate
*/
void USARTF0_init(uint16_t baud_rate);

/*
Sends byte over usartf0
@byte_to_send-> byte to send
*/
void USARTF0_send_byte(char byte_to_send);

/*
Sends string over usartf0
@string_pointer-> string to send
*/
void USARTF0_send_string(char* string_pointer);

/*
Clear receiver buffer
Note: Besides clearing the buffer it sets 'usartc0_message_received' flag to 0.
*/
void USARTF0_clear_receiver_buffer();

/*
Gets status of the receiver
Note: Message is received when end characters ('\r' and/or '\n') are received
RETURN-> char value 0(message not yet received) OR 1(message receievd)
*/
char USARTF0_get_receiver_status();

/*
Gets last received message
RETURN-> char pointer to received string
*/
char* USARTF0_get_received_message();

/*
Shifts out given number of characters from receiver buffer
@lenght-> number of characters to be shifted out
*/
void USARTF0_shift_receive_buffer(char lenght);

/*
Gets current receiver buffer size
RETURN-> char value of buffer size
*/
char USARTF0_get_receive_buffer_index();

/*
Sends string from program memory
@progmem_string_pointer-> string to send
*/
void USARTF0_send_string_P(PGM_P progmem_string_pointer);

/*

*/
void USARTF0_send_data(char* data_pointer, int lenght);

/*
Sends one character over usartf0
@char a-> character to send
*/
void USARTF0_send_one_character(char a);

/*
Sends one int over usartf0
@unsigned int a-> int to send
*/
void USARTF0_print_int(unsigned int a);

/*
Sends new line
*/
void USARTF0_send_new_line();

/*
*/
void USARTF0_print_float(float a);


#endif /* USART_H_ */