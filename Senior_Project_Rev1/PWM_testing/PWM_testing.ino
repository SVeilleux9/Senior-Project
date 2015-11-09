#include <avr/io.h>
#include <avr/interrupt.h>

int PIDcalc(int poss);

void setup() {
  DDRD &= (0 << DDD2) & (0 << DDD3);
  DDRD |= (1 << DDD5) | (1 << DDD6);
  
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT1 = 0; //Set counter to 0
  TCCR0A |= (1 << WGM00); //Phase-correct PWM
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << COM0B0); //None-inverted mode (High at bottom, Low on match)
  TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00); //No prescaler

  EIMSK |= (1 << INT0) | (1 << INT1); //Enable external interrupts on Pins 2 and 3 of port D
  EICRA |= (1 << ISC10) | (1 << ISC00); //Set it so that the interrupt occurs every logic change of pins 2 and 3 of port D

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; //Set counter to 0
  TCCR1B |= (1 << WGM12); //Set the interupt in CTC mode so that OCR1A is the top
  OCR1A = 15999; //Create an interrupt that occurs every 1mS
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); //No prescaler
  TIMSK1 |= (1 << OCIE1A); //Enable the interrupt

  ADMUX |= (0 << REFS1) | (0 << REFS0); //Reference of AVcc
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | ( 1 << ADPS0); //Enable the ADC and set the prescaler to 128, ADC frequency of 125kHz
  ADCSRA |= (1 << ADATE);
  ADCSRB |= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
  ADMUX |= (0b00000111); //Select ADC channel 7
  ADCSRA |= (1 << ADSC);
 
  //Serial.begin(250000);
}


int poss=0;
ISR(INT0_vect){ 
  (PIND & (1 << DDD2))/4 == (PIND & (1 << DDD3))/8 ?  poss++ : poss--;
}

ISR(INT1_vect){
  (PIND & (1 << DDD2))/4 == (PIND & (1 << DDD3))/8 ?  poss-- : poss++;
}

ISR(TIMER1_COMPA_vect){
  int output = PIDcalc(poss);
  OCR0A = 126 + output;
  OCR0B = 126 + output;
}

void loop() {
}


#define kp 1
#define ki 0.01
#define kd 0.01
#define outMax  20                        //The PWM output should have a max of 146
#define outMin -20                        //The PWM output should have a min of 106

int PIDcalc(int poss){  
  int lastPoss, change, error;
  double P,I,D;
  int output;

  // Calculate P, I, and D
  error = ADC - poss;

  //P, proportional term
  P = kp * error;

  //I, integral term
  I += ki * error);
  if(I > maxOutput) I = maxOutput;        //This should not be needed but to ensure that the Iterm does not get too large it is limited
  else if(I < minOutput) I = minOutput;

  //D, derivative term 
  change = (poss - lastPoss);             //Calculate how fast the motor is spinning
  lastPoss = poss;                        //Keep track of the last position for the next time PID is called
  D = (kd * change);
  
  //Add all terms together
  output = floor(P + I - D);              //Truncate the sum to store a double in an int

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


