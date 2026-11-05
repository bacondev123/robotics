void readSensors() {
  for (int i = 0; i < Config::NUM_SENSORS; i++) {
    sensorValue[i] = analogRead(Config::SENSOR_PINS[i]);
  }
}

bool isBlack(int i) { return sensorValue[i] < sensorRef[i]; }
bool isWhite(int i) { return sensorValue[i] >= sensorRef[i]; }
bool isCenterDetected() { return isBlack(1) || isBlack(2); }

bool isCrossDetected() {
  readSensors();
  return sensorValue[0] < sensorRef[0] && sensorValue[3] < sensorRef[3];
}

void calibrateSensors() {
  int minVal[Config::NUM_SENSORS] = { 4095, 4095};
  int maxVal[Config::NUM_SENSORS] = { 0, 0};

  Serial.println("[AUTO-TUNE] Move Robot left & right repeatedly...");
  displayPrint("Calibrating...\nSlide left & right", 0, 10, true);
  
  unsigned long timerStart = millis();

  while (millis() - timerStart < 5000) {
    for (int i = 0; i < Config::NUM_SENSORS; i++) {
      int currentReading = analogRead(Config::SENSOR_PINS[i]);
      if (currentReading > maxVal[i]) maxVal[i] = currentReading;
      if (currentReading < minVal[i]) minVal[i] = currentReading;
    }
    
    if (millis() % 1000 == 0) {
      int timeLeft = 5 - ((millis() - timerStart) / 1000);
      
      // Standard print for Serial Monitor
      if (Config::DEBUG_MODE) {
        Serial.print("Time remaining: ");
        Serial.println(timeLeft);
      }
      
      // OLED Print
      displayPrint("Calibrating...\nTime: " + String(timeLeft) + "s", 0, 10, true);
    }
    delay(5);
  }

  for (int i = 0; i < Config::NUM_SENSORS; i++) {
    int thresholdBias = (maxVal[i] - minVal[i]) * 0.75; 
    sensorRef[i] = minVal[i] + thresholdBias;
  }

  beep();
  Serial.println("---- Calibration Complete ----");
  
  // Standard print for Serial Monitor
  Serial.print("S0 Ref: "); Serial.print(sensorRef[0]);
  Serial.print(" | S1 Ref: "); Serial.println(sensorRef[1]);
  Serial.print("S2 Ref: "); Serial.print(sensorRef[2]);
  Serial.print(" | S3 Ref: "); Serial.println(sensorRef[3]);

  displayPrint("Calibration\nComplete!", 20, 20, true);
  delay(1000);
  
  waitForStart();
}