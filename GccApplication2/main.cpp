/*
 * GccApplication2.cpp
 *
 * Created: 12/5/2017 8:37:14 PM
 * Author : Pritesh
 */ 


#include<avr/io.h>
#define F_CPU 16000000
#include<util/delay.h>      //used for delay function
#include<avr/interrupt.h>
#include<string.h>
#include<stdlib.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void flick(){
	PORTC |= (1<<PINC0);
	_delay_ms(1000);
	PORTC &= ~(1<<PINC0);
	_delay_ms(1000);
	
}



char signal[5],b[2];
void handle_string(char *demo){
	if(strcmp(demo,"aa#")==0){
			PORTD |=(1<<PIND3);
			flick();
		}else if(strcmp(demo,"ab#")==0){
			flick();
			PORTD &=~(1<<PIND3);
		}else if(strcmp(demo,"ba#")==0){
			PORTD |=(1<<PIND4);
		}else if(strcmp(demo,"bb#")==0){
			PORTD &=~(1<<PIND4);
		}else if(strcmp(demo,"ca#")==0){
			PORTD |=(1<<PIND5);
		}else if(strcmp(demo,"cb#")==0){
			PORTD &=~(1<<PIND5);
		}else if(strcmp(demo,"da#")==0){
			PORTD |=(1<<PIND6);
		}else if(strcmp(demo,"db#")==0){
			PORTD &=~(1<<PIND6);
		}else if(strcmp(demo,"ea#")==0){
			PORTD |=(1<<PIND7);
		}else if(strcmp(demo,"eb#")==0){
			PORTD &=~(1<<PIND7);
		}else if(strcmp(demo,"fa#")==0){
			PORTB |=(1<<PINB0);
		}else if(strcmp(demo,"fb#")==0){
			PORTB &=~(1<<PINB0);
		}else if(strcmp(demo,"zz#")==0){
			PORTC &=~(1<<PINC5);
			_delay_ms(10);
			PORTC |=(1<<PINC5);
			
		}
}


class Switch{

	private:
	bool state =false;

	public:
	bool getState(){
		return state;
	}

	void setState(bool s){
		state = s;
	}

};

Switch s1,s2,s3,s4;

void port_init(){
	DDRD |=(1<<PIND3)|(1<<PIND4)|(1<<PIND5)|(1<<PIND6)|(1<<PIND7);
	PORTD &=~((1<<PIND3)|(1<<PIND4)|(1<<PIND5)|(1<<PIND6)|(1<<PIND7));
	PORTB &=~(1<<PINB0);
	DDRB |=(1<<PINB0);
	DDRC |=(1<<PINC5);
	DDRC &=~((1<<PINC4)|(1<<PINC3)|(1<<PINC2)|(1<<PINC1));
	PORTC |=(1<<PINC5)|(1<<PINC4)|(1<<PINC3)|(1<<PINC2)|(1<<PINC1);
}


void usart_init()
{
	UCSRB |=  (1 << RXEN) | (1 << TXEN);   	// Turn on the transmission reception ..
	// circuitry and receiver interrupt
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1) |(1<<USBS); // Use 8-bit character sizes

	UBRRL = BAUD_PRESCALE; 	// Load lower 8-bits of the baud rate value..
	// into the low byte of the UBRR register
	UBRRH = (BAUD_PRESCALE >> 8); 	// Load upper 8-bits of the baud rate value..
	// into the high byte of the UBRR register
}

void usart_init_async()
{
	UCSRB |=  (1<<RXCIE)|(1 << RXEN) | (1 << TXEN);   	// Turn on the transmission reception ..
	// circuitry and receiver interrupt
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1) |(1<<USBS); // Use 8-bit character sizes

	UBRRL = BAUD_PRESCALE; 	// Load lower 8-bits of the baud rate value..
	// into the low byte of the UBRR register
	UBRRH = (BAUD_PRESCALE >> 8); 	// Load upper 8-bits of the baud rate value..
	// into the high byte of the UBRR register
}

void usart_putchar( unsigned char data )
{
	// Do nothing while UDRE0 indicates data still in transit
	while( !(UCSRA & (1<<UDRE)) );
	
	// Put data into buffer
	UDR = data;
}

void WriteStringData(char *strData)
{
	while(*strData!='\0')
	{
		usart_putchar(*strData);
		strData++;
	}
	usart_putchar('#');
	return;

}

void dimmer_init(){
	GICR = (1<<INT0);
	MCUCR =(1<<ISC01)|(1<<ISC00);
	
}
void dimmer_one_init(){
	dimmer_init();
	TIMSK =(1<<OCIE1A);
	TCCR1B =(1<<CS12)|(1<<CS10);
	OCR1A=0;
	
}
void dimmer_two_init(){
	dimmer_init();
	TIMSK =(1<<OCIE1B);
	TCCR1B =(1<<CS12)|(1<<CS10);
	OCR1B=0;
}

void check_switch(){
	
	if (bit_is_clear(PINC, PC4)) {
		if(s1.getState()==false){
			PORTD |=(1<<PIND3);
			s1.setState(true);
		}
	}else if(s1.getState()==true){
			PORTD &=~(1<<PIND3);
			s1.setState(false);
	}
	
	if (bit_is_clear(PINC, PC3)) {
		if(s2.getState()==false){
			PORTD |=(1<<PIND4);
			s2.setState(true);
		}
		}else if(s2.getState()==true){
		PORTD &=~(1<<PIND4);
		s2.setState(false);
	}
	
	
	if (bit_is_clear(PINC, PC2)) {
		if(s3.getState()==false){
			PORTD |=(1<<PIND5);
			s3.setState(true);
		}
		}else if(s3.getState()==true){
		PORTD &=~(1<<PIND5);
		s3.setState(false);
	}
	
	
}
int main()
{
	DDRD |=(1<<PIND3);
	DDRC=0xFF;
	flick();
	sei();
	port_init();
	usart_init_async();
	while(1)             
	{
		check_switch();
			
	}
	return 0;
}




ISR (USART_RXC_vect)
{
	char value;
	value = UDR; 		// Fetch the received byte value into the variable "value"
	b[0]=value;
	b[1]='\0';
	UDR='p';
	strcat(signal,b);
	
	if(value=='#' && strlen(signal)==3){
		handle_string(signal);
		strcpy(signal,"");
	}else if(strlen(signal)>3){
		strcpy(signal,"");
	}
}

ISR(TIMER1_COMPA_vect){
	PORTD &=~(1<<PIND3);
}

ISR(TIMER1_COMPB_vect){
	
}

ISR(INT0_vect){
	TCNT1 = 0;
	PORTD |=(1<<PIND3);
}

