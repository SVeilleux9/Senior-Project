#include <avr/io.h>
#include <avr/interrupt.h>
#include "MYPID.h"
#include "inits.h"

#define maxOutput -20
#define minOutput  20
#define kp         1
#define ki        .01
#define kd        .01

int poss=0;
myPID PIDinstance;

void setup() {
  inits();
  PIDinstance.setValues(maxOutput, minOutput, kp, ki, kd);
}

void loop() {
}

ISR(INT0_vect){ 
  (PIND & (1 << DDD2)) == (PIND & (1 << DDD3))/2 ?  poss++ : poss--;
}

ISR(INT1_vect){
  (PIND & (1 << DDD2)) == (PIND & (1 << DDD3))/2 ?  poss-- : poss++;
}

ISR(TIMER1_COMPA_vect){
  int output = PIDinstance.PIDcalc(poss);
  OCR0A = 126 + output;
  OCR0B = 126 + output;
}






