/*** Included files ***/

#include <avr/io.h>
#include <avr/delay.h>
#include <arduino.h>

#include "comm.h"

/*** Pre-processor definitions ***/

#define F_CPU 16000000UL // 16 MHz

/*** Constructor and De-constructor ***/

comm::comm(){
}

comm::~comm(){
}

/*
 *	setValue() is called when the user sends the string "setValue". The user will be prompted
 *	to send the value. If the user does not send a value within ~10 seconds the program will
 *	exit and the user will see the message "Timed out". If the user does send a value but the 
 *	value is NOT a number the program will send back "Entered value is not a number". If the 
 *	user does send a number but it is not in the range .2 <= input <= 2 then the user will be 
 *	told the "Value out of range. Range is .2 - 2". If the user does follow all the rules set
 *	above the sent value will be stored in the class variable "value".
 */

void comm::setValue(){
	float temp; 

	Serial.println("Send Value");
	
	if(!timeout()){
		input = getString();
		if(aNumber(input)) temp = input.toFloat();
		else Serial.println("Entered value is not an number");
		
		if(temp > 2 || temp < .2) Serial.println("Value out of range. Range is .2 - 2");
		else value = temp;
	}
	
}


/*
 * aNumber() is used to detect if an input string can be represented as a float. Each character
 * in the string is gone through and checked to see if it is a digit or a '.'. If neither of these
 * are true then the function is exited and returns false. Only if all characters in the string 
 * pass the test then the function is exited and true is returned. 
 */

bool comm::aNumber(String str){
	for(char i=0; i<str.length(); i++){
		if( !(isDigit(str.charAt(i))) && !(str.charAt(i) == '.')) return false;
	}
	return true;
}

/*
 * getString() is used to get the input string when it is detected that there is avaibilble data
 * in the serial receiver. The program first delays 10mS to allow for all the information to be
 * received. The information in the serial receiver is then stored in "input" and returned for
 * later use. 
 */

String comm::getString(){
	_delay_ms(10);
	input = "";
	while(Serial.available() > 0){
		input += (char) Serial.read();
	}

	return input;
}

bool timeout(){
	long time;
	bool timedout;
	
	time = millis();
	timedout = false;
	
	while(Serial.available() == 0 && timedout == false){
		if((time + 320000) < millis()){
			Serial.println("Timed out");
			timedout = true;
		}
	}
	
	return timedout;
}