#include <avr/io.h>
#include "inits.h"

void inits(){

  DDRD &= (0 << DDD2) & (0 << DDD3);
  DDRD |= (1 << DDD5) | (1 << DDD6);
  
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT1 = 0; //Set counter to 0
  TCCR0A |= (1 << WGM00); //Phase-correct PWM
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << COM0B0); //None-inverted mode (High at bottom, Low on match)
  TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00); //No prescaler

  EIMSK |= (1 << INT0) | (1 << INT1); //Enable external interrupts on Pins 2 and 3 of port D
  EICRA |= (1 << ISC10) | (1 << ISC00); //Set it so that the interrupt occurs every logic change of pins 2 and 3 of port D

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; //Set counter to 0
  TCCR1B |= (1 << WGM12); //Set the interupt in CTC mode so that OCR1A is the top
  OCR1A = 15999; //Create an interrupt that occurs every 1mS
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); //No prescaler
  TIMSK1 |= (1 << OCIE1A); //Enable the interrupt

  ADMUX |= (0 << REFS1) | (0 << REFS0); //Reference of AVcc
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | ( 1 << ADPS0); //Enable the ADC and set the prescaler to 128, ADC frequency of 125kHz
  ADCSRA |= (1 << ADATE);
  ADCSRB |= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
  ADMUX |= (0b00000111); //Select ADC channel 7
  ADCSRA |= (1 << ADSC);

}

