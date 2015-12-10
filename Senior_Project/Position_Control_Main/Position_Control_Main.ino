/*** included files ***/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "MYPID.h"
#include "inits.h"
#include "motorControl.h"

/*** pre-processor constants ***/
 
#define F_CPU 16000000UL // 16 MHz
#define maxOutput  20
#define minOutput -20
#define kp         1
#define ki         0
#define kd         0

/*** class initializations ***/

myPID PIDinstance;
myMotor motor;

void setup() {
  PIDinstance.setValues(maxOutput, minOutput, kp, ki, kd);
  motor.poss = 0;
  inits();  
  motor.zeroMotor();
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
  // PD3 must be divided by 2 as it reads to be 8 and PD2 reads to be 4
  (PIND & (1 << DDD2)) == (PIND & (1 << DDD3))/2 ?  motor.poss++ : motor.poss--;   
}

ISR(INT1_vect){
  // PD3 must be divided by 2 as it reads to be 8 and PD2 reads to be 4
  (PIND & (1 << DDD2)) == (PIND & (1 << DDD3))/2 ?  motor.poss-- : motor.poss++;
}

/*
 *  ISR(TIMER1_COMPA_vect) is a timer based interrupt that occurs every 1mS. This is used 
 *  to accurately calculate the PID constants every 1mS.
 */
ISR(TIMER1_COMPA_vect){
  int output = PIDinstance.PIDcalc(motor.poss);
  motor.setMotorSpeed(output);
}


