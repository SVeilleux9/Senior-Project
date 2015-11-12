#include <avr/io.h>
#include "MYPID.h"


myPID::myPID(){
	
}

myPID::~myPID(){
	
}

void myPID::setValues(int max, int min, double p, double i, double d){
	maxOutput = max;
	minOutput = min;
	kp = p;
	ki = i;
	kd = d;
	P=0;
	I=0;
	D=0;

}

int myPID::PIDcalc(int error, int poss){  
	int lastPoss=0, change=0, output; 

  // Calculate P, I, and D

  //P, proportional term
  P = kp * error;

  //I, integral term
  I += ki * error;
  if(I >= maxOutput) I = maxOutput;        //This should not be needed but to ensure that the Iterm does not get too large it is limited
  else if(I <= minOutput) I = minOutput;

  //D, derivative term 
  change = (poss - lastPoss);             //Calculate how fast the motor is spinning
  lastPoss = poss;                        //Keep track of the last position for the next time PID is called
  D = (kd * change);
  
  //Add all terms together
  output = (int) (P + I - D);              //Truncate the sum to store a double in an int

  //Check once again that the max is not greater than abs(20)
  if(output > maxOutput) output = maxOutput;
    else if(output < minOutput) output = minOutput;

  return output;
  
}