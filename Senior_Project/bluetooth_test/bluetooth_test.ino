#include "comm.h"
#include "inits.h"

void setup() {
  inits();
  Serial.begin(9600);
}


comm commInstance;

void loop() {
  if(Serial.available() > 0){
    commInstance.input = commInstance.getString();
    
    if(commInstance.input == "setValue") commInstance.setValue();
    if(commInstance.input == "getValue") Serial.println(commInstance.value);
  }

}

ISR(INT0_vect){ 
 
}

ISR(INT1_vect){

}

/*
 *  ISR(TIMER1_COMPA_vect) is a timer based interrupt that occurs every 1mS. This is used 
 *  to accurately calculate the PID constants every 1mS.
 */
ISR(TIMER1_COMPA_vect){

}
