#include <avr/io.h>
#include <avr/interrupt.h>
#include "MYPID.h"
#include "inits.h"

#define maxOutput  20
#define minOutput -20
#define kp         1
#define kii        0
#define kd         0

int poss=0;
myPID PIDinstance;

void setup() {
  pinMode(8,OUTPUT);
  inits();
  PIDinstance.setValues(maxOutput, minOutput, kp, kii, kd);
  Serial.begin(250000);
  delay(100);
  digitalWrite(8,LOW);
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
  int error = ADC - poss;
  int output = PIDinstance.PIDcalc(error, poss);
  OCR0A = 126 + output;
  OCR0B = 126 + output;

  Serial.print(PIDinstance.P);
  Serial.print("  ");
  Serial.println(output);
}



