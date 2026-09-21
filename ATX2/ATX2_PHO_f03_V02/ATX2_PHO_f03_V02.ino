#include <ATX2.h>
#include <string.h>
#include "Config.h" // Optional hardware structure inclusion

// ==== SENSORS CONFIG ====
#define NUM_SENSORS 4
#define CENTER_POS 1500  // CENTER
#define MAX_POS 3000     // ABSOLUTE RIGHT
#define MIN_POS 0        // ABSOLUTE LEFT

// ==== CONFIG ====
#define TURN_DELAY 50
#define LOOP_DELAY 50
#define LINE_SKIP 500
#define SMALL_LINE_SKIP 150

// ==== SWAPPER CONFIG ====
#define USE_PID 1     
#define DEBUG_MODE 0  

// ==== OPERATION CONFIG ====
#define OPERATION_PATH 1
#define OPERATION_PATH_VARIATIONS 1

// ===== SENSOR SETUP =====
int sensorPins[NUM_SENSORS] = { 0, 1, 2, 3 };
int sensorRef[NUM_SENSORS] = { 783, 784, 656, 681 }; 
int sensorValue[NUM_SENSORS];

// ===== MAIN SETUP =====
void setup() {
  XIO();
  OK();
  servo(1, 90);
  beep();
  delay(1000);

  glcdMode(1);
  glcdClear();

  // Operation();
  Experiment();
  
}

void loop() {
  // test_func();
  // delay(50);
}

void driveLineFollowerSwapper(int speed) {
#if USE_PID
  driveLineFollowerPID(speed, 3.0, 0.5, 0.02);
#else
  trackLine(speed); // Legacy reference assuming macro trackLine remains natively. 
#endif
}

void traverseCrossSwapper(int speed) {
  int hitConfirm = 0; // Debounced crossing safely integrated here too
  while (1) {
    readSensors();
    bool crossFound = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);
    if (crossFound) {
      hitConfirm++;
      if (hitConfirm >= 2) break;
    } else {
      hitConfirm = 0;
      driveLineFollowerSwapper(speed);
    }
  }
  ao();
}