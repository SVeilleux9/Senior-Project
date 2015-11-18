#include <avr/io.h>
#include <avr/interrupt.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(8,OUTPUT);
  pinMode(2,INPUT);

  EIMSK |= (1 << INT0); //Enable external interrupts on Pins 2 and 3 of port D
  EICRA |= (1 << ISC01) | (1 << ISC00); //Set it so that the interrupt occurs every logic change of pins 2 and 3 of port D
}

int i=0;
ISR(INT0_vect){ 
  if(i >= 2){
    PORTB ^= (1 << DDB0);
    i=0;
  }
  i++;
}


void loop() {
  // put your main code here, to run repeatedly:

}
