#include <avr/io.h>
#include <avr/interrupt.h>

int PIDcalc(int poss);

void setup() {
  DDRB |= (1 << DDB5);
  DDRD &= (0 << DDD2) & (0 << DDD3);
  DDRD |= (1 << DDD5) | (1 << DDD6);
  
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT1 = 0; //Set counter to 0
  TCCR0A |= (1 << WGM00); //Phase-correct PWM
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1); //None-inverted mode (High at bottom, Low on match)
  TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00); //No prescaler

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0; //Set counter to 0
  TCCR1B |= (1 << WGM12); //Set the interupt in CTC mode so that OCR1A is the top
  OCR1A = 15999; //Create an interrupt that occurs every 1mS
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); //No prescaler
  TIMSK1 |= (1 << OCIE1A); //Enable the interrupt

  EIMSK |= (1 << INT0) | (1 << INT1); //Enable external interrupts on Pins 2 and 3 of port D
  EICRA |= (1 << ISC10) | (1 << ISC00); //Set it so that the interrupt occurs every logic change of pins 2 and 3 of port D

  ADMUX |= (0 << REFS1) | (0 << REFS0); //Reference of AVcc
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | ( 1 << ADPS0); //Enable the ADC and set the prescaler to 128, ADC frequency of 125kHz
  ADCSRA |= (1 << ADATE);
  ADCSRB |= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
  ADMUX |= (0b00000111); //Select ADC channel 7
  ADCSRA |= (1 << ADSC);
 
  Serial.begin(250000);
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
}

void loop() {
}


#define kp 1
#define ki 0.01
#define kd 0.25
#define outMax  20          //The PWM output should have a max of 146
#define outMin -20          //The PWM output should have a min of 106

int PIDcalc(int poss){  
  int lastPoss, change, error;
  double P,I,D;
  double output;

  // Calculate P, I, and D
  error = ADC - poss;

  Serial.println(ADC);
  //P
  P = (kp * error);

  //This should not be needed but to ensure that the Iterm does not get too large it is limited
  I += (ki * error);
  if(I > outMax) I = outMax;
  else if(I < outMin) I = outMin;

  //D
  change = (poss - lastPoss);
  D = (kd * change);
  
  //Add them together
  output = P + I - D;

  //Check once again that the max is not greater than abs(20)
  if(output > outMax) output = outMax;
    else if(output < outMin) output = outMin;
    
  lastPoss = poss; 

  return (int) output;
}


