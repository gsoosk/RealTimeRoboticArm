#include <Arduino.h>
#include <Servo.h>
// #include "Ticker.h" // For Timer 

#define SERVO_0_MIN_MICROSECOND 590
#define SERVO_0_MAX_MICROSECOND 2245
#define SERVO_1_MIN_MICROSECOND 800
#define SERVO_1_MAX_MICROSECOND 2400
#define SERVO_2_MIN_MICROSECOND 675
#define SERVO_2_MAX_MICROSECOND 2525

#define SERVO_0_PIN 3
#define SERVO_1_PIN 5
#define SERVO_2_PIN 6
//TODO: SERVO_3_PIN

#define SERVO_MAX 180
#define SERVO_MIN 0

//TODO: Adding servo 3 min and max microsecond 

#define POT_0_MAX 991
#define POT_0_MIN 60
#define POT_1_MAX 962
#define POT_1_MIN 100
#define POT_2_MAX 1012
#define POT_2_MIN 64



#define ENDLESS_TIMER 0
#define TIMER_PERIOD 40 // In Milis Second = 25 Hz

void readAndWrite();

Servo servo0;
Servo servo2;
Servo servo1;

// Ticker timer(readAndWrite, TIMER_PERIOD, ENDLESS_TIMER, MILLIS);

void setup() {
  // default values for attach are (544 , 2400)
  servo0.attach(SERVO_0_PIN , SERVO_0_MIN_MICROSECOND, SERVO_0_MAX_MICROSECOND); // defaults are 544 - 2400 to 
  servo1.attach(SERVO_1_PIN ,SERVO_1_MIN_MICROSECOND, SERVO_1_MAX_MICROSECOND);
  servo2.attach(SERVO_2_PIN , SERVO_2_MIN_MICROSECOND, SERVO_2_MAX_MICROSECOND); // defaults are 544 - 2400 to 
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);

  cli();//stop interrupts

  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 25hz increments
  OCR0A = 9999;// = (16*10^6) / (25*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei();//allow interrupts

  // timer.start();
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  readAndWrite();
}

void loop() {
  // timer.update();
}

void readAndWrite()
{
  int x0 = map(analogRead(A0), POT_0_MAX, POT_0_MIN, SERVO_MIN, SERVO_MAX);
  servo0.write(x0);
  int x1 = map(analogRead(A1), POT_1_MAX, POT_1_MIN, SERVO_MIN, SERVO_MAX);
  servo1.write(x1);
  int x2 = map(analogRead(A2), POT_2_MAX, POT_2_MIN, SERVO_MIN, SERVO_MAX);
  servo2.write(x2);
}