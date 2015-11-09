#include <avr/io.h>
#include "MYPID.h"


#define kp 1
#define ki 0.01
#define kd 0.01
#define maxOutput  20                        //The PWM output should have a max of 146
#define minOutput -20                        //The PWM output should have a min of 106

int PIDcalc(int poss){  
  int lastPoss, change, error;
  double P,I,D;
  int output;

  // Calculate P, I, and D
  error = ADC - poss;

  //P, proportional term
  P = kp * error;

  //I, integral term
  I += ki * error;
  if(I > maxOutput) I = maxOutput;        //This should not be needed but to ensure that the Iterm does not get too large it is limited
  else if(I < minOutput) I = minOutput;

  //D, derivative term 
  change = (poss - lastPoss);             //Calculate how fast the motor is spinning
  lastPoss = poss;                        //Keep track of the last position for the next time PID is called
  D = (kd * change);
  
  //Add all terms together
  output = (int) P + I - D;              //Truncate the sum to store a double in an int

  //Check once again that the max is not greater than abs(20)
  if(output > minOutput) output = maxOutput;
    else if(output < minOutput) output = minOutput;

  return output;

  //The following was used for debugging. Commented out to improve performance times. 
  /*Serial.print(poss);
  Serial.print("  ");
  Serial.print(ADC);
  Serial.print("  ");
  Serial.println(error); */
  
}