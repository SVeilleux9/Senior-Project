void setup() {
  Serial.begin(9600);
}

String input;
int value = 10;
int timeout;
bool timeoutBool;

void loop() {
  if(Serial.available() > 0){
    input = getString();
    
    if(input == "setValue"){
      Serial.println("Enter Value");
      timeout = millis();
      timeoutBool = false;
      while(Serial.available() == 0 && timeoutBool == false){
        if((timeout + 10000) < millis()){
          Serial.println("Timed out");
          timeoutBool = true;
        }
      }
      input = getString();
      if(aNumber(input)) value = input.toInt();
      else Serial.println("Entered value is not an int");
      if(value == 69){
        Serial.println("You wish");
      }
    }

    if(input == "getValue"){
      Serial.println(value);
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
  delay(100);
  input = "";
  while(Serial.available() > 0){
    input += (char) Serial.read();
  }

  return input;
}

