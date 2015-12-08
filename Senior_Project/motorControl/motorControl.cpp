#include <avr/io.h>
#include <avr/delay.h>

#include "motorControl.h"
#define F_CPU 16000000UL // 16 MHz


myMotor::myMotor(){
	
}

myMotor::~myMotor(){
	
}

/*  
 *   zeroMotor() is used to set the motor position to zero. This is done by moving
 *  the motor clock-wise for a short time to ensure that it is not currently at the
 *  set zero point. This theoretically could be removed however it looks "cool" and 
 *  is what most modern day speedometers do. The motor then moves counter clock-wise 
 *  and continues to do so until the motor does not move for a set time. Since the 
 *  mechanical structure being used has some flex to it the point the motor stops is 
 *  not actually "zero" but slightly below it. To fix this the motor is then spun
 *  clock-wise for a short set time. From this point the motor is set to zero. 
 */
void myMotor::zeroMotor(){
  // Turn off the timer based interrupt 
  TIMSK1 &= (0 << OCIE1A); 
  bool moving = true;
  int lastPoss=100;

  // Move the motor clock-wise for set time then move counter clock-wise
  setMotorSpeed(10);
  _delay_ms(1000);
  setMotorSpeed(-7);

  // If the motor stops moving it has been "zero'd" out. Since this "zero" is actually
  // slightly below zero the motor is moved clock-wise for a short time to be set to 
  // a true "zero"
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
  
  // Turn the timer based interrupt back on
  TIMSK1 |= (1 << OCIE1A);
}

/*
 * setMotorSpeed(int speed) is used to change the PWM duty cycle of pins PD5 and PD6.
 * Since these outputs are inverses of each other, that being when OCR0A = top = 255
 * PD6 has a duty cycle of 100%. When OCR0B = top = 255 PD5 has a duty cycle of 0%. 
 * For this reason the outputs can be set to the same value and drive the gate driver
 * which requires two inputs that are inverses of each other. 
 */
void myMotor::setMotorSpeed(int speed){
    OCR0A = 126+speed; // Sets PWM duty cycle on PD6
    OCR0B = 126+speed; // Sets PWM duty cycle on PD5
}
