#include <ATX2.h>
#include <string.h>

// ==== SENSORS CONFIG ====
#define NUM_SENSORS 4
#define CENTER_POS 1500  // CENTER
#define MAX_POS 3000     // ABSOLUTE RIGHT
#define MIN_POS 0        // ABSOLUTE LEFT

// ==== CONFIG ====
#define TURN_DELAY_MS 50
#define LOOP_DELAY_MS 50
#define LINE_SKIP_MS 475
#define SMALL_LINE_SKIP_MS 50
#define SEQUENCE_DELAY_MS 500
#define MOTOR_BIAS 1.2

// ==== SWAPPER CONFIG ====
// #define USE_PID 1     
#define DEBUG_MODE 0  

// ==== OPERATION CONFIG ====
#define OPERATION_PATH 1
#define OPERATION_PATH_VARIATIONS 1

// ===== SENSOR SETUP =====
int sensorPins[NUM_SENSORS] = { 0, 1, 2, 3 };
int sensorRef[NUM_SENSORS] = {894, 891, 890, 910}; // Pre-val 783, 784, 656, 681
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

  // fd2(20 + MOTOR_BIAS ,20);
  // delay(600);
  // ao();
  // sensors_Calibration();
  Operation();
  // servo_drop();
  // Experiment();
  
}

void loop() {
  // test_func();
  // delay(50);
}

