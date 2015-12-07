void setup() {
  Serial.begin(9600);
}

String input;
uint16_t value = 10;
int timeout;
bool timeoutBool;

void loop() {
  if(Serial.available() > 0){
    input = getString();
    
    if(input == "setValue"){
      Serial.print("Send");
      timeout = millis();
      timeoutBool = false;
      while(Serial.available() == 0 && timeoutBool == false){
        if((timeout + 10000) < millis()){
          Serial.print("Timed out");
          timeoutBool = true;
        }
      }
      input = getString();
      if(aNumber(input)) value = input.toInt();
      else Serial.print("Entered value is not an int");
      if(value == 69){
        Serial.print("You wish");
      }
    }

    if(input == "getValue"){
      Serial.print(value);
    }
  }

}

bool aNumber(String str){
  for(char i=0; i<str.length(); i++){
    if( !(isDigit(str.charAt(i)))) return false;
  }
  return true;
}

String getString(){
  delay(10);
  input = "";
  while(Serial.available() > 0){
    input += (char) Serial.read();
  }

  return input;
}

