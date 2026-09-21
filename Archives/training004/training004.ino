#include <ATX2.h>
#include <string.h>


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
#define USE_PID 1     // 1 = use pid. 0 = use regular trackline
#define DEBUG_MODE 0  // 1 = enable display debugging. 0 = disable display debugging
// ==== OPERATION CONFIG ====
#define OPERATION_PATH 1
#define OPERATION_PATH_VARIATIONS 1

// constexpr uint8_t NUM_SENSORS = 4;
// ===== SENSOR SETUP =====
int sensorPins[NUM_SENSORS] = { 0, 1, 2, 3 };
int sensorRef[NUM_SENSORS] = { 783, 784, 656, 681 };  // old val 593, 573, 541, 528
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

  Operation();
  // Experiment();

  // mission_test(); 
  // test_func();
}


// ===== - =====
void loop() {

  // trackLine_PID(30, 4, 0.1, 0.03);
  // trackLine_PID_TUNE(30, 4.5, 0, 0.08);
  // checkBatteryRealTime();

  // test_func();

  // readSensors();

  // ===== DEBUG VIEW =====
  // sensors_Calibration();
  // glcdClear();
  // debugSensors();

  // delay(50);
}

void trackLine_Swapper(int speed) {
#if USE_PID
  trackLine_PID(speed, 3, 0.5, 0.02);
#else
  trackLine(speed);
#endif
}
void trackCross_Swapper(int speed) {
  while (1) {
    readSensors();
    bool cross = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);
    if (cross) {
      break;
    } else {
      trackLine_Swapper(speed);
    }
  }
  ao();
}
