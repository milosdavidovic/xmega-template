/*
 * usart.c
 *
 * Created: 01-Jul-15 09:37:50 AM
 *  Author: luka
 */ 
//D0 E0

#include "USART.h"

static volatile char usartc0_byte_transmitted;
static volatile char usartc0_message_received;
volatile char usartc0_receive_buffer[USARTC0_RECEIVE_BUFFER_SIZE];
static volatile uint8_t usartc0_receive_buffer_index = 0;

static volatile char usartd0_byte_transmitted;
static volatile char usartd0_message_received;
volatile char usartd0_receive_buffer[USARTD0_RECEIVE_BUFFER_SIZE];
static volatile uint8_t usartd0_receive_buffer_index = 0;

static volatile char usarte0_byte_transmitted;
static volatile char usarte0_message_received;
volatile char usarte0_receive_buffer[USARTE0_RECEIVE_BUFFER_SIZE];
static volatile uint8_t usarte0_receive_buffer_index = 0;

static volatile char usartf0_byte_transmitted;
static volatile char usartf0_message_received;
volatile char usartf0_receive_buffer[USARTF0_RECEIVE_BUFFER_SIZE];
static volatile uint8_t usartf0_receive_buffer_index = 0;

void USARTC0_init(uint16_t baud_rate)
{
	uint16_t value_for_bsel_register = 0;
	
	//From atmel documentation
	/*
	1.     Set the TxD pin value high, and optionally set the XCK pin low.
	2.     Set the TxD and optionally the XCK pin as output.
	3.     Set the baud rate and frame format.
	4.     Set the mode of operation (enables XCK pin output in synchronous mode).
	5.     Enable the transmitter or the receiver, depending on the usage.
	
	For interrupt-driven USART operation, global interrupts should be disabled during the initialization.
	*/
	
	cli(); // global interrupt disable
	
	PORTC_OUTSET = PIN3_bm; //set pin3 (TxD) high
	PORTC_DIRSET = PIN3_bm; //set pin3 (TxD) as out
	PORTC_DIRCLR = PIN2_bm; //set pin2 (RxD) as in
	
	//BSEL formula for desired baud rate
	//BSEL = (clock_speed / (2^BSCALE * 16 * baud_rate)) - 1 //BSCALE is baud rate generator scale factor (should be 0)
	
	//BASEL for Internal 2MHz RC Oscillator
	if(CLK_CTRL == CLK_SCLKSEL_RC2M_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 12;
		}
		else if(baud_rate == 2400)
		{
			value_for_bsel_register = 51;
		}
		else if(baud_rate == 4800)
		{
			value_for_bsel_register = 25;
		}
	}
	//BASEL for Internal 32MHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32M_gc)
	{
		
	}
	//BASEL for Internal 32kHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32K_gc)
	{
		
	}
	//External Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_XOSC_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 47;
		}
		else if(baud_rate == 2400)
		{
			//value_for_bsel_register = ??;
		}
		else if(baud_rate == 4800)
		{
			//value_for_bsel_register = ??;
		}
	}
	
	USARTC0_BAUDCTRLB = 0x00; // BSCALE + BSEL first 3 bits
	USARTC0_BAUDCTRLA = value_for_bsel_register; // BSEL last 8 bits
	
	// Asynchronous Mode, Parity Mode Disabled, Stop Bits 1, Character Size 8-bit
	USARTC0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc; 
	
	USARTC0_CTRLB = USART_RXEN_bm | USART_TXEN_bm; //Enable Receiver, Enable Transmitter   
	USARTC0_CTRLA = USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_LO_gc;//Receive interrupt enable with low-level, Transmit interrupt enable with low-level
	
	PMIC_CTRL |= PMIC_LOLVLEN_bm;//Enable low level interrupts
	
	sei(); // global interrupt enable
}

//Transmit interrupt routine
ISR(USARTC0_TXC_vect)
{
	usartc0_byte_transmitted = 1;
}

//Receive interrupt routine
ISR(USARTC0_RXC_vect)
{
	uint8_t usartc0_received_byte;
	
	usartc0_received_byte = USARTC0_DATA;
	if(usartc0_receive_buffer_index < USARTC0_RECEIVE_BUFFER_SIZE)
	{
		if(usartc0_received_byte == '\r' || usartc0_received_byte == '\n')
		{
			usartc0_receive_buffer[usartc0_receive_buffer_index] = usartc0_received_byte;
			usartc0_message_received = 1;
		}
		usartc0_receive_buffer[usartc0_receive_buffer_index] = usartc0_received_byte;
		usartc0_receive_buffer_index++;
	}
	else
	{
		USARTC0_clear_receiver_buffer();	
	}
	
}

void USARTC0_clear_receiver_buffer()
{
	for(uint8_t i = 0;i<USARTC0_RECEIVE_BUFFER_SIZE;i++)
	{
		usartc0_receive_buffer[i] = 0x00;
	}

	usartc0_receive_buffer_index = 0;
    usartc0_receive_buffer[usartc0_receive_buffer_index] = '\0';
    usartc0_message_received = 0;
}

char USARTC0_get_receive_buffer_index()
{
	return usartc0_receive_buffer_index;
}

void USARTC0_shift_receive_buffer(char lenght)
{
	char *buffer_pointer = (char*)&usartc0_receive_buffer[0];
	char *temp_buffer_pointer;
	
	usartc0_receive_buffer_index -= lenght;
	
	if(lenght < USARTC0_RECEIVE_BUFFER_SIZE)
	{
		temp_buffer_pointer = buffer_pointer + lenght;
	
		for(int i=0;i<USARTC0_RECEIVE_BUFFER_SIZE-lenght;i++)
		{
			*buffer_pointer = *temp_buffer_pointer;
			buffer_pointer++;
			temp_buffer_pointer++;
		}
	}
}

char USARTC0_get_receiver_status()
{
	return usartc0_message_received;
}

char* USARTC0_get_received_message()
{
	return (char*)&(usartc0_receive_buffer[0]);
}

void USARTC0_send_byte(char byte_to_send)
{
	usartc0_byte_transmitted = 0;
	USARTC0_DATA = byte_to_send;
	while(!usartc0_byte_transmitted);
}

void USARTC0_send_string(char* string_pointer)
{
	while(*string_pointer!='\0')
	{
		USARTC0_send_byte(*string_pointer);
		string_pointer++;
	}
}

void USARTC0_send_new_line()
{
	USARTC0_send_string("\n\r");
}

void USARTC0_send_string_P(PGM_P progmem_string_pointer)
{
	char temp_string[100] = {'\0'};
	
	strcpy_P(temp_string, progmem_string_pointer);
	
	USARTC0_send_string(temp_string);
}



//PORT D USART
void USARTD0_init(unsigned int baud_rate)
{
	int value_for_bsel_register = 0;
	
	//From atmel documentation
	/*
	1.     Set the TxD pin value high, and optionally set the XCK pin low.
	2.     Set the TxD and optionally the XCK pin as output.
	3.     Set the baud rate and frame format.
	4.     Set the mode of operation (enables XCK pin output in synchronous mode).
	5.     Enable the transmitter or the receiver, depending on the usage.
	
	For interrupt-driven USART operation, global interrupts should be disabled during the initialization.
	*/
	
	cli(); // global interrupt disable
	
	PORTD_OUTSET = PIN3_bm; //set pin3 (TxD) high
	PORTD_DIRSET = PIN3_bm; //set pin3 (TxD) as out
	PORTD_DIRCLR = PIN2_bm; //set pin2 (RxD) as in
	
	//BSEL formula for desired baud rate
	//BSEL = (clock_speed / (2^BSCALE * 16 * baud_rate)) - 1 //BSCALE is baud rate generator scale factor (should be 0)
	
	//BASEL for Internal 2MHz RC Oscillator
	if(CLK_CTRL == CLK_SCLKSEL_RC2M_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 12;
		}
		else if(baud_rate == 2400)
		{
			value_for_bsel_register = 51;
		}
		else if(baud_rate == 4800)
		{
			value_for_bsel_register = 25;
		}
	}
	//BASEL for Internal 32MHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32M_gc)
	{
		
	}
	//BASEL for Internal 32kHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32K_gc)
	{
		
	}
	//External Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_XOSC_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 47;
		}
		else if(baud_rate == 2400)
		{
			//value_for_bsel_register = ??;
		}
		else if(baud_rate == 4800)
		{
			//value_for_bsel_register = ??;
		}
	}
	
	USARTD0_BAUDCTRLB = 0x00; // BSCALE + BSEL first 3 bits
	USARTD0_BAUDCTRLA = value_for_bsel_register; // BSEL last 8 bits
	
	// Asynchronous Mode, Parity Mode Disabled, Stop Bits 1, Character Size 8-bit
	USARTD0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	USARTD0_CTRLB = USART_RXEN_bm | USART_TXEN_bm; //Enable Receiver, Enable Transmitter   
	USARTD0_CTRLA = USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_LO_gc; //Receive interrupt enable with low-level, Transmit interrupt enable with low-level
	
	PMIC_CTRL |= PMIC_LOLVLEN_bm; //Enable low level interrupts
	
	sei(); // global interrupt enable
}

//Transmit interrupt routine
ISR(USARTD0_TXC_vect)
{
	usartd0_byte_transmitted = 1;
}

//Receive interrupt routine
ISR(USARTD0_RXC_vect)
{
	char usartd0_received_byte;
	
	usartd0_received_byte = USARTD0_DATA;
	if(usartd0_receive_buffer_index < USARTD0_RECEIVE_BUFFER_SIZE)
	{
		if(usartd0_received_byte == '\r' || usartd0_received_byte == '\n')
		{
			usartd0_receive_buffer[usartd0_receive_buffer_index] = usartd0_received_byte;
			usartd0_message_received = 1;
		}
		usartd0_receive_buffer[usartd0_receive_buffer_index] = usartd0_received_byte;
		usartd0_receive_buffer_index++;
	}
	else
	{
		USARTD0_clear_receiver_buffer();	
	}	
}

void USARTD0_clear_receiver_buffer()
{
	for(int i = 0;i<USARTD0_RECEIVE_BUFFER_SIZE;i++)
	{
		usartd0_receive_buffer[i] = 0x00;
	}

	usartd0_receive_buffer_index = 0;
    usartd0_receive_buffer[usartd0_receive_buffer_index] = '\0';
    usartd0_message_received = 0;
}

char USARTD0_get_receive_buffer_index()
{
	return usartd0_receive_buffer_index;
}

void USARTD0_shift_receive_buffer(char lenght)
{
	char *buffer_pointer = (char*)&usartd0_receive_buffer[0];
	char *temp_buffer_pointer;
	
	usartd0_receive_buffer_index -= lenght;
	
	if(lenght < USARTD0_RECEIVE_BUFFER_SIZE)
	{
		temp_buffer_pointer = buffer_pointer + lenght;
		
		for(int i=0;i<USARTD0_RECEIVE_BUFFER_SIZE-lenght;i++)
		{
			*buffer_pointer = *temp_buffer_pointer;
			buffer_pointer++;
			temp_buffer_pointer++;
		}
	}
}

char USARTD0_get_receiver_status()
{
	return usartd0_message_received;
}

char* USARTD0_get_received_message()
{
	return (char*)&(usartd0_receive_buffer[0]);
}

void USARTD0_send_byte(char byte_to_send)
{
	usartd0_byte_transmitted = 0;
	USARTD0_DATA = byte_to_send;
	while(!usartd0_byte_transmitted);
}

void USARTD0_send_string(char* string_pointer)
{
	while(*string_pointer!='\0')
	{
		USARTD0_send_byte(*string_pointer);
		string_pointer++;
	}
}

void USARTD0_send_string_P(PGM_P progmem_string_pointer)
{
	char temp_string[100] = {'\0'};
	
	strcpy_P(temp_string, progmem_string_pointer);
	
	USARTD0_send_string(temp_string);
}

void USARTD0_send_new_line()
{
	USARTD0_send_string("\n\r");
}

void USARTD0_print_int(unsigned int a)
{
	char temp_answer [20];
	char temp_str[20];
	
	temp_answer[0]='\0';
	itoa(a, temp_str,10);
	strcat(temp_answer,temp_str);
	strcat(temp_answer,"\n");

	USARTD0_send_string((char*)temp_answer);
}

void USARTD0_print_long(uint32_t a)
{
	char temp_answer [20];
	char temp_str[20];
	
	temp_answer[0]='\0';
	ltoa(a, temp_str,10);
	strcat(temp_answer,temp_str);
	strcat(temp_answer,"\n");

	USARTD0_send_string((char*)temp_answer);
}

void USARTD0_send_data(char* data_pointer, int lenght)
{
	while(lenght)
	{
		USARTD0_send_byte(*data_pointer);
		data_pointer++;
		lenght--;
	}
}

void USARTD0_print_float(float a)
{
	char temp_answer [20];
	char temp_str[20];
	
	temp_answer[0]='\0';
	dtostrf(a, 8, 4, temp_str);
	strcat(temp_answer,temp_str);
	strcat(temp_answer,"\n");

	USARTD0_send_string((char*)temp_answer);
}



//PORT E USART
void USARTE0_init(unsigned int baud_rate)
{
	int value_for_bsel_register = 0;
	
	//From atmel documentation
	/*
	1.     Set the TxD pin value high, and optionally set the XCK pin low.
	2.     Set the TxD and optionally the XCK pin as output.
	3.     Set the baud rate and frame format.
	4.     Set the mode of operation (enables XCK pin output in synchronous mode).
	5.     Enable the transmitter or the receiver, depending on the usage.
	
	For interrupt-driven USART operation, global interrupts should be disabled during the initialization.
	*/
	
	cli(); // global interrupt disable
	
	PORTE_OUTSET = PIN3_bm; //set pin3 (TxD) high
	PORTE_DIRSET = PIN3_bm; //set pin3 (TxD) as out
	PORTE_DIRCLR = PIN2_bm; //set pin2 (RxD) as in
	
	//BSEL formula for desired baud rate
	//BSEL = (clock_speed / (2^BSCALE * 16 * baud_rate)) - 1 //BSCALE is baud rate generator scale factor (should be 0)
	
	//BASEL for Internal 2MHz RC Oscillator
	if(CLK_CTRL == CLK_SCLKSEL_RC2M_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 12;
		}
		else if(baud_rate == 2400)
		{
			value_for_bsel_register = 51;
		}
		else if(baud_rate == 4800)
		{
			value_for_bsel_register = 25;
		}
	}
	//BASEL for Internal 32MHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32M_gc)
	{
		
	}
	//BASEL for Internal 32kHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32K_gc)
	{
		
	}
	//External Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_XOSC_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 47;
		}
		else if(baud_rate == 2400)
		{
			//value_for_bsel_register = ??;
		}
		else if(baud_rate == 4800)
		{
			//value_for_bsel_register = ??;
		}
	}
	
	USARTE0_BAUDCTRLB = 0x00; // BSCALE + BSEL first 3 bits
	USARTE0_BAUDCTRLA = value_for_bsel_register; // BSEL last 8 bits
	
	// Asynchronous Mode, Parity Mode Disabled, Stop Bits 1, Character Size 8-bit
	USARTE0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc; 
	USARTE0_CTRLB = USART_RXEN_bm | USART_TXEN_bm; //Enable Receiver, Enable Transmitter   
	USARTE0_CTRLA = USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_LO_gc; //Receive interrupt enable with low-level, Transmit interrupt enable with low-level
	
	PMIC_CTRL |= PMIC_LOLVLEN_bm; //Enable low level interrupts
	
	sei(); // global interrupt enable
}

//Transmit interrupt routine
ISR(USARTE0_TXC_vect)
{
	usarte0_byte_transmitted = 1;
}

//Receive interrupt routine
ISR(USARTE0_RXC_vect)
{
	char usarte0_received_byte;
	
	usarte0_received_byte = USARTE0_DATA;
	if(usarte0_receive_buffer_index < USARTE0_RECEIVE_BUFFER_SIZE)
	{
		if(usarte0_received_byte == '\r' || usarte0_received_byte == '\n')
		{
			usarte0_receive_buffer[usarte0_receive_buffer_index] = usarte0_received_byte;
			usarte0_message_received = 1;
		}
		usarte0_receive_buffer[usarte0_receive_buffer_index] = usarte0_received_byte;
		usarte0_receive_buffer_index++;
	}
	else
	{
		USARTE0_clear_receiver_buffer();	
	}
	
}

void USARTE0_clear_receiver_buffer()
{
	for(int i = 0;i<USARTE0_RECEIVE_BUFFER_SIZE;i++)
	{
		usarte0_receive_buffer[i] = 0x00;
	}

	usarte0_receive_buffer_index = 0;
    usarte0_receive_buffer[usarte0_receive_buffer_index] = '\0';
    usarte0_message_received = 0;
}

char USARTE0_get_receive_buffer_index()
{
	return usarte0_receive_buffer_index;
}

void USARTE0_shift_receive_buffer(char lenght)
{
	char *buffer_pointer = (char*)&usarte0_receive_buffer[0];
	char *temp_buffer_pointer;
	
	usarte0_receive_buffer_index -= lenght;
	
	if(lenght < USARTE0_RECEIVE_BUFFER_SIZE)
	{
		temp_buffer_pointer = buffer_pointer + lenght;
		
		for(int i=0;i<USARTE0_RECEIVE_BUFFER_SIZE-lenght;i++)
		{
			*buffer_pointer = *temp_buffer_pointer;
			buffer_pointer++;
			temp_buffer_pointer++;
		}
	}
}

char USARTE0_get_receiver_status()
{
	return usarte0_message_received;
}

char* USARTE0_get_received_message()
{
	return (char*)&(usarte0_receive_buffer[0]);
}

void USARTE0_send_byte(char byte_to_send)
{
	usarte0_byte_transmitted = 0;
	USARTE0_DATA = byte_to_send;
	while(!usarte0_byte_transmitted);
}

void USARTE0_send_string(char* string_pointer)
{
	while(*string_pointer!='\0')
	{
		USARTE0_send_byte(*string_pointer);
		string_pointer++;
	}
}

void USARTE0_send_data(char* data_pointer, int lenght)
{
	while(lenght)
	{
		USARTE0_send_byte(*data_pointer);
		data_pointer++;
		lenght--;
	}
}

void USARTE0_send_new_line()
{
	USARTE0_send_string("\n\r");
}

void USARTE0_send_string_P(PGM_P progmem_string_pointer)
{
	char temp_string[100] = {'\0'};
	
	strcpy_P(temp_string, progmem_string_pointer);
	
	USARTE0_send_string(temp_string);
}



void USARTF0_init(uint16_t baud_rate)
{
	volatile uint16_t value_for_bsel_register = 0;
	
	//From atmel documentation
	/*
	1.     Set the TxD pin value high, and optionally set the XCK pin low.
	2.     Set the TxD and optionally the XCK pin as output.
	3.     Set the baud rate and frame format.
	4.     Set the mode of operation (enables XCK pin output in synchronous mode).
	5.     Enable the transmitter or the receiver, depending on the usage.
	
	For interrupt-driven USART operation, global interrupts should be disabled during the initialization.
	*/
	
	cli(); // global interrupt disable
	
	PORTF_OUTSET = PIN3_bm; //set pin3 (TxD) high
	PORTF_DIRSET = PIN3_bm; //set pin3 (TxD) as out
	PORTF_DIRCLR = PIN2_bm; //set pin2 (RxD) as in
	
	//BSEL formula for desired baud rate
	//BSEL = (clock_speed / (2^BSCALE * 16 * baud_rate)) - 1 //BSCALE is baud rate generator scale factor (should be 0)
	
	//BASEL for Internal 2MHz RC Oscillator
	if(CLK_CTRL == CLK_SCLKSEL_RC2M_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 12;
		}
		else if(baud_rate == 2400)
		{
			value_for_bsel_register = 51;
		}
		else if(baud_rate == 4800)
		{
			value_for_bsel_register = 25;
		}
	}
	//BASEL for Internal 32MHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32M_gc)
	{
		
	}
	//BASEL for Internal 32kHz RC Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_RC32K_gc)
	{
		
	}
	//External Oscillator
	else if (CLK_CTRL == CLK_SCLKSEL_XOSC_gc)
	{
		if(baud_rate == 9600)
		{
			value_for_bsel_register = 47;
		}
		else if(baud_rate == 2400)
		{
			//value_for_bsel_register = ??;
		}
		else if(baud_rate == 4800)
		{
			//value_for_bsel_register = ??;
		}
	}
	
	USARTF0_BAUDCTRLB = 0x00; // BSCALE + BSEL first 3 bits
	USARTF0_BAUDCTRLA = value_for_bsel_register; // BSEL last 8 bits
	
	// Asynchronous Mode, Parity Mode Disabled, Stop Bits 1, Character Size 8-bit
	USARTF0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc; 
	
	USARTF0_CTRLB = USART_RXEN_bm | USART_TXEN_bm; //Enable Receiver, Enable Transmitter   
	USARTF0_CTRLA = USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_LO_gc;//Receive interrupt enable with low-level, Transmit interrupt enable with low-level
	
	PMIC_CTRL |= PMIC_LOLVLEN_bm;//Enable low level interrupts
	
	sei(); // global interrupt enable
}

//Transmit interrupt routine
ISR(USARTF0_TXC_vect)
{
	usartf0_byte_transmitted = 1;
}

//Receive interrupt routine
ISR(USARTF0_RXC_vect)
{
	char usartf0_received_byte;
	
	usartf0_received_byte = USARTF0_DATA;
	if(usartf0_receive_buffer_index < USARTF0_RECEIVE_BUFFER_SIZE)
	{
		if(usartf0_received_byte == '\r' || usartf0_received_byte == '\n')
		{
			usartf0_receive_buffer[usartf0_receive_buffer_index] = usartf0_received_byte;
			usartf0_message_received = 1;
		}
		usartf0_receive_buffer[usartf0_receive_buffer_index] = usartf0_received_byte;
		usartf0_receive_buffer_index++;
	}
	else
	{
		USARTF0_clear_receiver_buffer();	
	}
	
}

void USARTF0_clear_receiver_buffer()
{
	for(uint8_t i = 0;i<USARTF0_RECEIVE_BUFFER_SIZE;i++)
	{
    	usartf0_receive_buffer[i] = 0x00;
	}
    
    usartf0_receive_buffer_index = 0;
    usartf0_receive_buffer[usartf0_receive_buffer_index] = '\0';
    usartf0_message_received = 0;
}

char USARTF0_get_receive_buffer_index()
{
	return usartf0_receive_buffer_index;
}

void USARTF0_shift_receive_buffer(char lenght)
{
	char *buffer_pointer = (char*)&usartf0_receive_buffer[0];
	char *temp_buffer_pointer;
	
	usartf0_receive_buffer_index -= lenght;
	
	if(lenght < USARTF0_RECEIVE_BUFFER_SIZE)
	{
		temp_buffer_pointer = buffer_pointer + lenght;
	
		for(int i=0;i<USARTF0_RECEIVE_BUFFER_SIZE-lenght;i++)
		{
			*buffer_pointer = *temp_buffer_pointer;
			buffer_pointer++;
			temp_buffer_pointer++;
		}
	}
}

char USARTF0_get_receiver_status()
{
	return usartf0_message_received;
}

char* USARTF0_get_received_message()
{
	return (char*)&(usartf0_receive_buffer[0]);
}

void USARTF0_send_byte(char byte_to_send)
{
	usartf0_byte_transmitted = 0;
	USARTF0_DATA = byte_to_send;
	while(!usartf0_byte_transmitted);
}

void USARTF0_send_string(char* string_pointer)
{
	while(*string_pointer!='\0')
	{
		USARTF0_send_byte(*string_pointer);
		string_pointer++;
	}
}

void USARTF0_send_string_P(PGM_P progmem_string_pointer)
{
	char temp_string[100] = {'\0'};
	
	strcpy_P(temp_string, progmem_string_pointer);
	
	USARTF0_send_string(temp_string);
}

void USARTF0_send_data(char* data_pointer, int lenght)
{
	while(lenght)
	{
		USARTF0_send_byte(*data_pointer);
		data_pointer++;
		lenght--;
	}
}

void USARTF0_send_one_character(char a)
{
	char temp_answer [4];
	char temp_str[4];
	
	temp_answer[0]='\0';
	itoa(a, temp_str,10);
	strcat(temp_answer,temp_str);
	strcat(temp_answer,"\r\n");

	USARTF0_send_string((char*)temp_answer);
}

void USARTF0_print_int(unsigned int a)
{
	char temp_answer [20];
	char temp_str[20];
	
	temp_answer[0]='\0';
	itoa(a, temp_str,10);
	strcat(temp_answer,temp_str);
	strcat(temp_answer,"\n");

	USARTF0_send_string((char*)temp_answer);
}

void USARTF0_print_float(float a)
{
	char temp_answer [20];
	char temp_str[20];
	
	temp_answer[0]='\0';
	dtostrf(a, 8, 4, temp_str);
	strcat(temp_answer,temp_str);
	strcat(temp_answer,"\n");

	USARTF0_send_string((char*)temp_answer);
}

void USARTF0_send_new_line()
{
	USARTF0_send_string("\n\r");
}