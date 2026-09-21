#include <ATX2.h>

const int NUM_SENSORS = 4;
const int LOOP_DELAY_MS = 100;
const int DEFAULT_LINE_SKIP = 475;
const float MOTOR_BIAS = 1.2;
const float DEFAULT_SPEED = 20.0;
const int sensorPins[NUM_SENSORS] = { 0, 1, 2, 3 };
int sensorRef[NUM_SENSORS] = { 894, 891, 890, 910 };  // Pre-val 783, 784, 656, 681
int sensorValue[NUM_SENSORS];
void setup() {
  // put your setup code here, to run once:
  XIO();
  OK();
  servo(1, 90);
  beep();
  delay(1000);
  glcdMode(1);
  glcdClear();

  // Experiment();
  Operation();
  // S1_B1_S1();
}

void loop() {
  // put your main code here, to run repeatedly:
}
