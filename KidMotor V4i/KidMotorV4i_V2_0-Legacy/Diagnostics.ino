// =================================================================
// SAFE DIAGNOSTIC TOOLS (I2C Protected)
// =================================================================

void scanButtonPin() {
  // Safe candidate pins on RP2040:
  // (Skips I2C pins 4 & 5, flash pins 23-25, and sensor ADC pins 26-27)
  const int safeCandidatePins[] = { 
    0, 1, 2, 3, 6, 7, 8, 9, 10, 
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 28 
  };
  const int totalPins = sizeof(safeCandidatePins) / sizeof(safeCandidatePins[0]);

  // 1. Update OLED FIRST before configuring button pins
  displayPrint("SCANNING PINS...\n\nPress SW1 Button", 0, 0, true);

  Serial.println("\n========================================");
  Serial.println("  SAFE BUTTON SCANNER ACTIVE (I2C Safe) ");
  Serial.println("  Press SW1 on your board now...        ");
  Serial.println("========================================");

  // 2. Configure ONLY safe candidate pins as INPUT_PULLUP
  for (int i = 0; i < totalPins; i++) {
    pinMode(safeCandidatePins[i], INPUT_PULLUP);
  }

  int detectedPin = -1;

  // 3. Scan loop
  while (detectedPin == -1) {
    for (int i = 0; i < totalPins; i++) {
      int pin = safeCandidatePins[i];

      // Active LOW: Pressing the button pulls the pin to 0 (GND)
      if (digitalRead(pin) == LOW) {
        detectedPin = pin;
        break;
      }
    }
    delay(10); // Prevents CPU lockup
  }

  // 4. Output Results
  beep();

  Serial.println("\n----------------------------------------");
  Serial.print(">>> DETECTED! SW1 is on GPIO Pin: ");
  Serial.println(detectedPin);
  Serial.println("----------------------------------------");

  // Show the discovered pin clearly on the screen
  displayPrint("BUTTON FOUND!\n\nSW1 Pin = " + String(detectedPin), 0, 0, true);
  
  // Hold the message for 5 seconds so you can see it
  delay(5000);
}
// =================================================================
// INTERACTIVE MOTOR & BUZZER PIN TESTER
// =================================================================
void testMotorAndBuzzerPins() {
  // Safe candidate pins on RP2040 (excludes I2C 4/5, SW1/SW2 8/9, Sensors 26/27)
  const int testPins[] = { 2, 3, 6, 7, 10, 11, 12, 13, 14, 15, 18, 19, 20, 21, 22 };
  const int count = sizeof(testPins) / sizeof(testPins[0]);

  Serial.println("\n[MOTOR TESTER] Press SW1 to test the next pin...");
  displayPrint("MOTOR TESTER\nPress SW1 to step", 0, 0, true);

  for (int i = 0; i < count; i++) {
    int pin = testPins[i];

    // 1. Wait for you to press SW1 (Pin 8) to test the next pin
    while (digitalRead(8) == HIGH) { delay(10); }
    while (digitalRead(8) == LOW)  { delay(10); }

    // 2. Show current tested pin on screen
    displayPrint("Testing GPIO: " + String(pin), 0, 15, true);
    Serial.printf("\n--> Testing GPIO %d ...\n", pin);

    // 3. Pulse the pin at 50% power for 1 second
    pinMode(pin, OUTPUT);
    analogWrite(pin, 128); // 50% power
    delay(1000);
    analogWrite(pin, 0);   // Stop

    delay(200);
  }

  displayPrint("TEST FINISHED!", 0, 20, true);
  Serial.println("\n[TEST FINISHED] Update HardwareEngine.h with your pins.");
}