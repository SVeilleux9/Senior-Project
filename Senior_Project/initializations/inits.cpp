#include <avr/io.h>
#include "inits.h"
#include <arduino.h>

void inits(){

	// Set up the IO pins
	DDRD &= (0 << DDD2) & (0 << DDD3);							// PD2 and PD3 = inputs
	DDRD |= (1 << DDD5) | (1 << DDD6);							// PD5 and PD6 = outputs
	DDRB |= (1 << DDB0);										// PB0		   = output
	
	
	// Set up timer and settings for PWM outputs on PD5 and PD6
	TCCR0A = 0;
	TCCR0B = 0;
	TCNT1 = 0;													//Set counter to 0
	TCCR0A |= (1 << WGM00);										//Phase-correct PWM (desired for motor control)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);					//OCR0A: non-inverted mode (High at bottom, Low on match)
	TCCR0A |= (1 << COM0B0);									//OCR0B: inverted mode (Low at bottom, High on match);
	TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);			//No pre-scaler

	// Set up the interrupt pins on PD2 and PD3
	EIMSK |= (1 << INT0) | (1 << INT1);							//Enable external interrupts on PD2 and PD3
	EICRA |= (1 << ISC10) | (1 << ISC00);						//Set it so that the interrupt occurs every logic change of PD2 and PD3

	// Set up timer settings for the 1mS interrupt used for PID control
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;													//Set counter to 0
	TCCR1B |= (1 << WGM12);										//Set the interrupt in CTC mode so that OCR1A is the top (TCNT1 resets when it equals OCR1A)
	OCR1A = 15999;												//Create an interrupt that occurs every 1mS
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);			//No pre-scaler
	TIMSK1 |= (1 << OCIE1A);									//Enable the interrupt

	// Set up the ADC 
	ADMUX |= (0 << REFS1) | (0 << REFS0);						//Reference of AVcc
	ADCSRA |= (1 << ADEN);										//Enable the ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | ( 1 << ADPS0);		//Set the pre-scaler to 128, ADC frequency of 125kHz
	ADCSRA |= (1 << ADATE);
	ADCSRB |= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);		//ADC set to free-running mode
	ADMUX |= (0b00000111);										//Select ADC channel 7
	ADCSRA |= (1 << ADSC);										//Start ADC

	PORTB &= (0 << DDB0);										//Set PB0 low to enable the gate driver
	
	
}
