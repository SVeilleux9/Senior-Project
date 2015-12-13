/*** Included files ***/

#include <avr/io.h>
#include <avr/eeprom.h>

#include "MYPID.h"

/*** Pre-processor definitions ***/

#define EEPROM_FLOAT_ADDRESS (float *) 0x2
#define EEPROM_BYTE_ADDRESS (uint8_t*) 0x1

/*** Constructor and De-Constructor ***/

myPID::myPID(){
}

myPID::~myPID(){	
}

/*
 *	setValues() is used to set the various variables used by PIDcalc. Most are user set with the one
 *	one exception of adjustmentFactor which is stored in EEPROM. If this value has not been set before
 *	the program will detect that and set it to the default value of 1 and it sets the EEPROM_BYTE_ADDRESS
 *	to 1 so that on next start up the adjustmentFactor value does not get set to default if previously changed.
 */
void myPID::setValues(int max, int min, double p, double i, double d, bool notLinear){
	maxOutput = max;
	minOutput = min;
	kp = p;
	ki = i;
	kd = d;
	
	if(eeprom_read_byte(EEPROM_BYTE_ADDRESS) != 1){
		adjustmentFactor = 1;
		eeprom_write_float(EEPROM_FLOAT_ADDRESS, 1);
		eeprom_write_byte(EEPROM_BYTE_ADDRESS, 1);
	}
	else adjustmentFactor = eeprom_read_float((float*) 0x2);
	
}

/*
 *	PIDcalc() is used to calculate the PID output value under current conditions. This function
 *	should be called over an accurate incremental time. By calculating the error by taking the 
 *	difference between desired and current. The desired value is adjusted as the speedometer 
 *  face is NOT linear, this is the one project specific part in the code. The PID values are
 *	calculated according to the PID definitions. The I term is limited between minOutput and 
 *	maxOutput to ensure that if error is large the I term does not get excessively large and 
 *  cause oscillation. Also the output is limited between the same constraints to limit the 
 *  speed of change.
 */

int myPID::PIDcalc(int current, int desired){  
	int lastCurrent=0, change=0, output, error; 
	double P=0,I=0,D=0;
	
   /* 
	* The adjustment factor is applied to fix linear errors. Set to 1 if not desired.
	* Also if the output is not linear, adjustment can be made. 
	*/
	desired *= adjustmentFactor;
	if(notLinear) (desired < 94) ? (desired /= 2) : (desired -= 47);

	error = desired - current;
	
  // Calculate P, I, and D
 

  //P, proportional term
  P = kp * error;

  //I, integral term
  I += ki * error;
  if(I >= maxOutput) I = maxOutput;				//This should not be needed but to ensure that the I term does not get too large it is limited
  else if(I <= minOutput) I = minOutput;

  //D, derivative term 
  change = (current - lastCurrent);             //Calculate how fast the motor is spinning
  lastCurrent = current;                        //Keep track of the last position for the next time PID is called
  D = (kd * change);
  
  //Add all terms together
  output = (int) (P + I - D);					//Truncate the sum to store a double in an int

  //Check once again that the max is not outside the limits
  if(output > maxOutput) output = maxOutput;
    else if(output < minOutput) output = minOutput;

  return output;
  
}

/*
 * getAdjustmentFactor() returns the adjustmentFactor inside the myPID class. 
 */

float myPID::getAdjustmentFactor(){
	
	return adjustmentFactor;
	
}

/*
 * setAdjustmentFactor() sets the adjustmentFactor inside the myPID class.
 * Also the adjustmentFactor is stored in EEPROM for use after reset.  
 */

void myPID::setAdjustmentFactor(float x){

	adjustmentFactor = x;
	eeprom_write_float(EEPROM_FLOAT_ADDRESS,x);
	
}