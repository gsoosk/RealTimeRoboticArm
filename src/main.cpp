#include <Arduino.h>
#include <Servo.h>
#define SERVO_0_MIN_MICROSECOND 590
#define SERVO_0_MAX_MICROSECOND 2245
#define SERVO_1_MIN_MICROSECOND 800
#define SERVO_1_MAX_MICROSECOND 2400
#define SERVO_2_MIN_MICROSECOND 675
#define SERVO_2_MAX_MICROSECOND 2525
//TODO: Adding servo 3 min and max microsecond 



Servo servo;
void setup() {
  // default values for attach are (544 , 2400)
  servo.attach(9, SERVO_1_MIN_MICROSECOND, SERVO_1_MAX_MICROSECOND); // defaults are 544 - 2400 to 
  Serial.begin(9600);
  servo.write(90);
}

void loop() {
  while ( Serial.available() )
  {
    String x = Serial.readString();
    servo.write(x.toInt());
  }
  
}