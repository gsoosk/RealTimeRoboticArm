#include <Arduino.h>
#include <Servo.h>
#include "Ticker.h" // For Timer 

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
#define TIMER_PERIOD 40 // In mili seconds Second = 25 Hz

void readAndWrite0();
void readAndWrite1();
void readAndWrite2();

Servo servo0;
Servo servo2;
Servo servo1;

Ticker timer0(readAndWrite0, TIMER_PERIOD, ENDLESS_TIMER, MILLIS);
Ticker timer1(readAndWrite1, TIMER_PERIOD, ENDLESS_TIMER, MILLIS);
Ticker timer2(readAndWrite2, TIMER_PERIOD, ENDLESS_TIMER, MILLIS);

void setup() {
  // default values for attach are (544 , 2400)
  servo0.attach(SERVO_0_PIN , SERVO_0_MIN_MICROSECOND, SERVO_0_MAX_MICROSECOND); // defaults are 544 - 2400 to 
  servo1.attach(SERVO_1_PIN ,SERVO_1_MIN_MICROSECOND, SERVO_1_MAX_MICROSECOND);
  servo2.attach(SERVO_2_PIN , SERVO_2_MIN_MICROSECOND, SERVO_2_MAX_MICROSECOND); // defaults are 544 - 2400 to 
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);

  timer0.start();
  timer1.start();
  timer2.start();
}

void loop() {
  timer0.update();
  timer1.update();
  timer2.update();
}

void readAndWrite0()
{
  int x0 = map(analogRead(A0), POT_0_MAX, POT_0_MIN, SERVO_MIN, SERVO_MAX);
  servo0.write(x0);
}
void readAndWrite1()
{
  int x1 = map(analogRead(A1), POT_1_MAX, POT_1_MIN, SERVO_MIN, SERVO_MAX);
  servo1.write(x1);
}
void readAndWrite2()
{
  int x2 = map(analogRead(A2), POT_2_MAX, POT_2_MIN, SERVO_MIN, SERVO_MAX);
  servo2.write(x2);
}