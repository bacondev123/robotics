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


  
  // displayPrint("Calibration\nComplete!", 20, 20, true);

  display.print("S0 Ref: "); display.print(sensorRef[0]);
  display.print(" | S1 Ref: "); display.println(sensorRef[1]);
  display.print("S2 Ref: "); display.print(sensorRef[2]);
  display.print(" | S3 Ref: "); display.println(sensorRef[3]);

  delay(1000);
  
  waitForStart();
}
// =================================================================
// LIVE SENSOR MONITOR (OLED & SERIAL)
// =================================================================
void showAnalogLive() {
  Serial.println("\n[MONITOR] Live sensor reading started. Press SW1 to exit...");

  while (1) {
    // 1. Read fresh analog values (GPIO 26 and GPIO 27)
    readSensors();

    // 2. Draw to OLED without flickering
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Title
    display.setCursor(0, 0);
    display.println("=== LIVE SENSORS ===");

    // Left Sensor (D4 / GPIO 26)
    display.setCursor(0, 16);
    display.print("L (P26): ");
    display.print(sensorValue[0]);
    // Shows if the code currently considers this BLACK or WHITE
    display.print(isBlack(0) ? " [BLK]" : " [WHT]");

    // Right Sensor (D5 / GPIO 27)
    display.setCursor(0, 30);
    display.print("R (P27): ");
    display.print(sensorValue[1]);
    display.print(isBlack(1) ? " [BLK]" : " [WHT]");

    // Current Threshold Reference
    display.setCursor(0, 44);
    display.print("Ref L:");
    display.print(sensorRef[0]);
    display.print(" R:");
    display.print(sensorRef[1]);

    // Exit instruction
    display.setCursor(0, 56);
    display.print("Press SW1 to Exit");

    display.display();

    // 3. Also output to Serial Monitor for PC debugging
    Serial.printf("L(26): %4d | R(27): %4d\r", sensorValue[0], sensorValue[1]);

    // 4. Press SW1 (Pin 8) to exit the monitor loop
    if (digitalRead(OK_BUTTON) == LOW) {
      while (digitalRead(OK_BUTTON) == LOW) { delay(10); } // Debounce button release
      beep();
      break;
    }

    delay(50); // Smooth refresh rate
  }

  displayPrint("Monitor Exited!", 0, 20, true);
  delay(500);
}