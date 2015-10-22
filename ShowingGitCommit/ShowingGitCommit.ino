#include <avr/io.h>
#include <avr/interrupt.h>

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

  ADMUX |= (0 << REFS1) | (1 << REFS0); //Reference of AVcc
  ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | ( 1 << ADPS0); //Enable the ADC and set the prescaler to 128, ADC frequency of 125kHz
  ADCSRA |= (1 << ADATE);
  ADCSRB |= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0);
  ADMUX |= (0b00000111); //Select ADC channel 7
  ADCSRA |= (1 << ADSC);
 
  Serial.begin(250000);
}



int poss=0;
int last;
  
ISR(INT0_vect){
  int current[6] = {8,0,4,12,8};
  int stage;  
  int i=0;

  PORTB ^= (1 << DDB5);
  stage = (PIND & (1 << DDD2)) + (PIND & (1 << DDD3));

  for(i=1; i<5; i++){
    if(stage == current[i]){
      if(last == current[i-1]) poss++;
      else poss--;
    }
  }
 last = stage;
 //Serial.println(poss);
}

ISR(INT1_vect, ISR_ALIASOF(INT0_vect));


double ITerm=0;
int lastPoss=0;

ISR(TIMER1_COMPA_vect){
  double kp = 1, ki = 0.01, kd = .25;
  double outMax = 200;
  double outMin = -200;
  double output;

  
/*Compute all the working error variables*/
  int error = ADC - poss;
  if(abs(error) >2){
    ITerm+= (ki * error);
    if(ITerm > outMax) ITerm= outMax;
    else if(ITerm < outMin) ITerm= outMin;
    double dInput = (poss - lastPoss);
    Serial.println(error);
  
    /*Compute PID Output*/
    output = kp * error + ITerm- kd * dInput;
    
    if(output > outMax) output = outMax;
      else if(output < outMin) output = outMin;
  
    /*Remember some variables for next time*/
    lastPoss = poss;
  }
  
  if(output > 50){
    OCR0A = output;
    OCR0B = 0;
  }
  else if(output < -50){
    OCR0A = 0;
    OCR0B = output;
  }
  else{
    OCR0A = 0;
    OCR0B = 0;
  }
}

void loop() {

  Serial.println("This is a test to show that GIT is easy on windows. Showing Matt Chambers.");


}
