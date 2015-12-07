/*** included files ***/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "MYPID.h"
#include "inits.h"

/*** pre-processor constants ***/
 
#define F_CPU 16000000UL // 16 MHz
#define maxOutput  20
#define minOutput -20
#define kp         1
#define kii        0
#define kd         0

/*** global variables ***/

int poss=0;

/*** class initializations ***/

myPID PIDinstance;

void setup() {
  PIDinstance.setValues(maxOutput, minOutput, kp, kii, kd);
  inits();  
  zeroMotor();
}


void loop() {
}

/*
 *  ISR(INT0_vect) and ISR(INT1_vect) are interupts that occur when PD2 and PD3 change
 *  logic states respectively. These interupts are used to track the motor possition  
 *  through the motor encoder. The encoder can be found on this website 
 *  
 *  https://www.pololu.com/product/2823
 *  
 *  It is shown that the encoder has two outputs that are are offset by 90 degrees. By
 *  measuring the logic states of each corresponding encoder outputs pin on the Atmega328p 
 *  it can be detected which channel is leading and which is lagging. From here the
 *  direction the motor is spinning is known and the position variable poss can be adjusted
 *  accordingly. 
 */
ISR(INT0_vect){ 
  // PD3 must be divided by 2 as it reads to be 8 not 1
  (PIND & (1 << DDD2)) == (PIND & (1 << DDD3))/2 ?  poss++ : poss--;   
}

ISR(INT1_vect){
  // PD3 must be divided by 2 as it reads to be 8 not 1
  (PIND & (1 << DDD2)) == (PIND & (1 << DDD3))/2 ?  poss-- : poss++;
}

/*
 *  ISR(TIMER1_COMPA_vect) is a timer based interrupt that occurs every 1mS. This is used 
 *  to accurately calculate the PID constants every 1mS.
 */
ISR(TIMER1_COMPA_vect){
  int output = PIDinstance.PIDcalc(poss);
  setMotorSpeed(output);
}

/*  
 *   zeroMotor() is used to set the motor position to zero. This is done by moving
 *  the motor clock-wise for a short time to ensure that it is not currently at the
 *  set zero point. This theoretically could be removed however it looks "cool" and 
 *  is what most modern day speedometers do. The motor then moves counter clock-wise 
 *  and contiues to do so until the motor does not move for a set time. Since the 
 *  mechanical structure being used has some flex to it the point the motor stops is 
 *  not actually "zero" but slightly below it. To fix this the motor is then spun
 *  clock-wise for a short set time. From this point the motor is set to zero. 
 */
void zeroMotor(){
  // Turn off the timer based interrupt 
  TIMSK1 &= (0 << OCIE1A); 
  bool moving = true;
  int lastPoss;

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

  // Turn the timer based interupt back on
  TIMSK1 |= (1 << OCIE1A);
}

/*
 * setMotorSpeed(int speed) is used to change the PWM duty cycle of pins PD5 and PD6.
 * Since these outputs are inverses of each other, that being when OCR0A = top = 255
 * PD6 has a duty cycle of 100%. When OCR0B = top = 255 PD5 has a duty cycle of 0%. 
 * For this reason the outputs can be set to the same value and drive the gate driver
 * which requires two inputs that are inverses of each other. 
 */
void setMotorSpeed(int speed){
    OCR0A = 126+speed; // Sets PWM duty cycle on PD6
    OCR0B = 126+speed; // Sets PWM duty cycle on PD5
}

