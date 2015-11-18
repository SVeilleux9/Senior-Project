#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "MYPID.h"
#include "inits.h"

#define F_CPU 16000000UL // 16 MHz
#define maxOutput  20
#define minOutput -20
#define kp         1
#define kii        0
#define kd         0

int poss=0;
int myDelay = 0;
myPID PIDinstance;

void setup() {
  PIDinstance.setValues(maxOutput, minOutput, kp, kii, kd);
  inits();  
  zeroMotor();
  Serial.begin(250000);
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
  Serial.println(ADC);
  int output = PIDinstance.PIDcalc(poss) * 2;
  setMotorSpeed(output);
}


void zeroMotor(){
  TIMSK1 &= (0 << OCIE1A);
  bool moving = true;
  int lastPoss;

  setMotorSpeed(10);
  _delay_ms(1000);
  setMotorSpeed(-7);
  
  while(moving == true){
    if(poss == lastPoss){
      moving = false;
      setMotorSpeed(10);
      _delay_ms(70);
      setMotorSpeed(0);
    }
    else lastPoss = poss;
    _delay_ms(20);
  }
  poss = 0;
  TIMSK1 |= (1 << OCIE1A);
}


void setMotorSpeed(int speed){
    OCR0A = 126+speed;
    OCR0B = 126+speed;
}

