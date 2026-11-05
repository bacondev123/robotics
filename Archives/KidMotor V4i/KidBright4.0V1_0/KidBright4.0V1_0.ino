#include "HardwareEngine.h"
#include "Config.h"


int sensorRef[Config::NUM_SENSORS] = { 2048, 2048, 2048, 2048 }; 
int sensorValue[Config::NUM_SENSORS];

float I = 0;
float previousError = 0;
float lineError = 0; // Renamed to avoid system collision!
float lastPosition = Config::CENTER_POS;
float currentBaseSpeed = 15.0;  

void setup() {
  initializeHardware();
  initializeDisplay(); 
  
  analogReadResolution(12); // Keep this for Raspberry Pi!
  
  // calibrateSensors(); 
  displayPrint("Hello Arduino");
  delay(5000);
  calibrateSensors();
  beep();
  waitForStart();
  // displayPrint("Hello Arduino");
  

  // runExperiment();
}

void loop() {
  // trackLinePID(30, 4.0, 0.1, 0.03);
}