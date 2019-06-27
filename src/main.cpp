#include "Ticker.h" // For Timer
#include <Arduino.h>
#include <Servo.h>

#define SERVO_0_MIN_MICROSECOND 590
#define SERVO_0_MAX_MICROSECOND 2245
#define SERVO_1_MIN_MICROSECOND 800
#define SERVO_1_MAX_MICROSECOND 2400
#define SERVO_2_MIN_MICROSECOND 675
#define SERVO_2_MAX_MICROSECOND 2525

#define SERVO_0_PIN 3
#define SERVO_1_PIN 5
#define SERVO_2_PIN 6
// TODO: SERVO_3_PIN

#define SERVO_MAX 180
#define SERVO_MIN 0

// TODO: Adding servo 3 min and max microsecond

#define POT_0_MAX 991
#define POT_0_MIN 60
#define POT_1_MAX 962
#define POT_1_MIN 100
#define POT_2_MAX 1012
#define POT_2_MIN 64

#define ENDLESS_TIMER 0
#define BUTTON_PERIOD 20 // In Miliseconds => 50 Hz
#define TIMER_PERIOD 40  // In Miliseconds => 25 Hz
#define CHECK_BUTTON

#define MAX_SAVED_VALUES 200

#define PLAY_BUTTON_PIN 8
#define SAVE_BUTTON_PIN 9

#define SAVE_UNIT_PERIOD 8
// Declarations
void readAndWrite();
void runSavedState();
void checkButton();
void initServoArrays(int);
void checkButton();

Servo servo0;
Servo servo2;
Servo servo1;

int lastSaved;
bool saving = false;
int checkSavingPeriod = 0;
int servoSaved[4][MAX_SAVED_VALUES];

Ticker read_timer(readAndWrite, TIMER_PERIOD, ENDLESS_TIMER, MILLIS);
Ticker button_timer(checkButton, BUTTON_PERIOD, ENDLESS_TIMER, MILLIS);

void setup() {
  // setup servos : default values for attach are (544 , 2400)
  servo0.attach(SERVO_0_PIN, SERVO_0_MIN_MICROSECOND,
                SERVO_0_MAX_MICROSECOND); // defaults are 544 - 2400 to
  servo1.attach(SERVO_1_PIN, SERVO_1_MIN_MICROSECOND,
                SERVO_1_MAX_MICROSECOND); // defaults are 544 - 2400 to
  servo2.attach(SERVO_2_PIN, SERVO_2_MIN_MICROSECOND,
                SERVO_2_MAX_MICROSECOND); // defaults are 544 - 2400 to
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);

  // init servos array
  lastSaved = 0;
  initServoArrays(0);
  initServoArrays(1);
  initServoArrays(2);

  // saving button
  pinMode(PLAY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SAVE_BUTTON_PIN, INPUT_PULLUP);

  read_timer.start();
  button_timer.start();

  Serial.begin(9600);
}

void loop() {
  read_timer.update();
  button_timer.update();
}

void readAndWrite() {
  int x0 = map(analogRead(A0), POT_0_MAX, POT_0_MIN, SERVO_MIN, SERVO_MAX);
  servo0.write(x0);
  int x1 = map(analogRead(A1), POT_1_MAX, POT_1_MIN, SERVO_MIN, SERVO_MAX);
  servo1.write(x1);
  int x2 = map(analogRead(A2), POT_2_MAX, POT_2_MIN, SERVO_MIN, SERVO_MAX);
  servo2.write(x2);

  if (lastSaved != MAX_SAVED_VALUES - 1 && saving) {
    if (checkSavingPeriod % SAVE_UNIT_PERIOD == 0) {
      servoSaved[0][lastSaved] = x0;
      servoSaved[1][lastSaved] = x1;
      servoSaved[2][lastSaved] = x2;
      lastSaved++;
    }
    checkSavingPeriod++;
  }
}

void checkButton() {
  if (digitalRead(PLAY_BUTTON_PIN) == LOW) {
    while (digitalRead(PLAY_BUTTON_PIN) == LOW) {
    }
    Serial.println("Play Button");
    runSavedState();
  }

  if (digitalRead(SAVE_BUTTON_PIN) == LOW) {
    while (digitalRead(SAVE_BUTTON_PIN) == LOW) {
    }
    Serial.println("Save Button");
    saving = !saving;
    if (saving)
      lastSaved = 0;
  }
}

void initServoArrays(int servoNum) {
  for (int i = 0; i < MAX_SAVED_VALUES; i++) {
    servoSaved[servoNum][i] = 0;
  }
}

void runSavedState() {
  for (int i = 0; i < MAX_SAVED_VALUES && i < lastSaved - 1; i++) {
    for (int j = 0; j < SAVE_UNIT_PERIOD; j++) {
      servo0.write(
          servoSaved[0][i] +
          (j * ((servoSaved[0][i + 1] - servoSaved[0][i]) / SAVE_UNIT_PERIOD)));
      servo1.write(
          servoSaved[1][i] +
          (j * ((servoSaved[1][i + 1] - servoSaved[1][i]) / SAVE_UNIT_PERIOD)));
      servo2.write(
          servoSaved[2][i] +
          (j * ((servoSaved[2][i + 1] - servoSaved[2][i]) / SAVE_UNIT_PERIOD)));
      delay(40);
    }
  }
}