#include <POP32.h>

constexpr uint8_t NUM_SENSORS = 4;
constexpr uint8_t sensorPins[NUM_SENSORS] = { 1, 3, 6, 8 };
int sensorRef[NUM_SENSORS] = { 3063, 3114, 3213, 2954 };  // Old val   2950, 2740, 3088, 2788
int sensorValue[NUM_SENSORS];
const float MOTOR_BIAS = 1.5;
const int LOOP_DELAY_MS = 100;
const int DEFAULT_LINE_SKIP = 905;
const float DEFAULT_SPEED = 20.0;

void setup() {
  oled.clear();
  oled.text(0, 0, "SW_OK");
  oled.show();
  waitSW_OK();
  oled.clear();
  oled.text(0, 0, "RUN..");
  oled.show();

  // Motor_tuning(20,20 + MOTOR_BIAS);
  Experiment();

  // sensors_Calibration();
}

void loop() {
  // show_analog();
  // sensors_Calibration();
  // trackLine(30);
}
void show_analog() {
  oled.text(0, 0, "a1=%d    ", analog(1));  // %d แสดงค่า a1
  oled.text(2, 0, "a3=%d    ", analog(3));  // %d แสดงค่า a3
  oled.text(4, 0, "a6=%d    ", analog(6));
  oled.text(6, 0, "a8=%d    ", analog(8));
  oled.show();
}
void debugSensorPin() {
  oled.clear();
  oled.text(0, 0, "TESTING SENSORS");
  oled.show();
  while (1) {
    readSensors();
    // Displays TRUE (1) or FALSE (0) for Far Left (0) and Far Right (3)
    oled.text(2, 0, "Left (Idx 0):  %d ", isBlack(0));
    oled.text(4, 0, "Right (Idx 3): %d ", isBlack(3));
    oled.show();
    delay(50);
  }
}