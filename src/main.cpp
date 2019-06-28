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

#define SERVO_COUNT 4

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

#define SAVE_UNIT_PERIOD 4
// Declarations
void readAndWrite();
void runSavedState();
void checkButton();
void initServoArrays(int);
void checkButton();

Servo servo[SERVO_COUNT];

byte lastSaved[SERVO_COUNT];
bool saving = false;
byte servoSaved[SERVO_COUNT][MAX_SAVED_VALUES];
byte servoTimePassed[SERVO_COUNT][MAX_SAVED_VALUES];
int checkSavingPeriod = 0;

Ticker read_timer(readAndWrite, TIMER_PERIOD, ENDLESS_TIMER, MILLIS);
Ticker button_timer(checkButton, BUTTON_PERIOD, ENDLESS_TIMER, MILLIS);

void setup() {
  // setup servos : default values for attach are (544 , 2400)
  servo[0].attach(SERVO_0_PIN, SERVO_0_MIN_MICROSECOND,
                  SERVO_0_MAX_MICROSECOND); // defaults are 544 - 2400 to
  servo[1].attach(SERVO_1_PIN, SERVO_1_MIN_MICROSECOND,
                  SERVO_1_MAX_MICROSECOND); // defaults are 544 - 2400 to
  servo[2].attach(SERVO_2_PIN, SERVO_2_MIN_MICROSECOND,
                  SERVO_2_MAX_MICROSECOND); // defaults are 544 - 2400 to
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);

  // init servos array
  for (int i = 0; i < SERVO_COUNT; i++)
    lastSaved[i] = 0;
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

bool hasSameValue(int newValue, int index) {
  if(lastSaved[index] == 0)
    return false;
  return servoSaved[index][lastSaved[index] - 1] >= newValue - 2 &&
         servoSaved[index][lastSaved[index] - 1] <= newValue + 2;
}

void updateSaved(int servo_value, int index) {
  if (!hasSameValue(servo_value, index)) {
    servoSaved[index][lastSaved[index]] = servo_value;
    servoTimePassed[index][lastSaved[index]] = 1;
    lastSaved[index]++;
  } else {
    servoTimePassed[index][lastSaved[index] - 1]++;
  }
}

void readAndWrite() {
  int x[4];

  x[0] = map(analogRead(A0), POT_0_MAX, POT_0_MIN, SERVO_MIN, SERVO_MAX);
  servo[0].write(x[0]);
  x[1] = map(analogRead(A1), POT_1_MAX, POT_1_MIN, SERVO_MIN, SERVO_MAX);
  servo[1].write(x[1]);
  x[2] = map(analogRead(A2), POT_2_MAX, POT_2_MIN, SERVO_MIN, SERVO_MAX);
  servo[2].write(x[2]);


  int maxSaved = lastSaved[0];
  for (int i = 0; i < SERVO_COUNT; i++) {
    if (lastSaved[i] > maxSaved)
      maxSaved = lastSaved[i];
  }
  if (maxSaved < MAX_SAVED_VALUES && saving) {
    if (checkSavingPeriod % SAVE_UNIT_PERIOD == 0) {
      updateSaved(x[0], 0);
      updateSaved(x[1], 1);
      updateSaved(x[2], 2);
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
    if (saving) {
      for (int i = 0; i < SERVO_COUNT; i++)
        lastSaved[i] = 0;
    }
  }
}

void initServoArrays(int servoNum) {
  for (int i = 0; i < MAX_SAVED_VALUES; i++) {
    servoSaved[servoNum][i] = 0;
    servoTimePassed[servoNum][i] = 0;
  }
  lastSaved[servoNum] = 0;
}

void runSavedState() {
  int savedActionsTime = 0;
  int servoIndex[4] = {0, 0, 0, 0};
  // Serial.println(servoIndex[0]);
  byte servoSlotRunnedTime[4] = {0, 0, 0, 0};

  for (int i = 0; i < lastSaved[0]; i++)
    savedActionsTime += servoTimePassed[0][i];

  for (int t = 0; t < savedActionsTime - 1; t++) {
    for (byte j = 0; j < SAVE_UNIT_PERIOD; j++) {

      byte servoValue[4];
      for (byte i = 0; i < SERVO_COUNT; i++) {
        servoValue[i] =
            servoTimePassed[i][servoIndex[i]] - servoSlotRunnedTime[i] == 1
                ? servoSaved[i][servoIndex[i]] +
                      (j * ((servoSaved[i][servoIndex[i] + 1] -
                             servoSaved[i][servoIndex[i]]) /
                            SAVE_UNIT_PERIOD))
                : servoSaved[i][servoIndex[i]];
      }
      delay(40);
      servo[0].write(servoValue[0]);
      servo[1].write(servoValue[1]);
      servo[2].write(servoValue[2]);
    }

    for (byte i = 0; i < SERVO_COUNT; i++) {
      servoSlotRunnedTime[i]++;
      if (servoTimePassed[i][servoIndex[i]] == servoSlotRunnedTime[i]) {
        servoIndex[i]++;
        servoSlotRunnedTime[i] = 0;
      }
    }
  }
}