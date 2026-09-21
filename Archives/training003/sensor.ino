// ===== READ ALL SENSORS =====
void readSensors() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValue[i] = analog(sensorPins[i]);
  }
}

bool isBlack(int i){
  return sensorValue[i] < sensorRef[i];
}
bool isWhite(int i){
  return sensorValue[i] >= sensorRef[i];
}
bool centerDetected() {
  return isBlack(1) || isBlack(2);
}
bool cross(){
  readSensors();
  return sensorValue[0] < sensorRef[0] && sensorValue[3] < sensorRef[3];
}
bool allBlack() {
  for (int i = 0; i < NUM_SENSORS; i++){
    if(!isBlack(i)) return false;
  }
  return true;
}

bool allWhite(){
  for(int i = 0; i < NUM_SENSORS; i++){
    if(isBlack(i)) return false;
  }
  return true;
}



// ===== DEBUG DISPLAY =====
// void debugSensors() {
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     char buffer[20];
//     sprintf(buffer, "S%d: %4d", i, sensorValue[i]); // vertical display = clean
//     glcd(i, 0, buffer);
//   }
// }
void sensors_Calibration() {
  // 1. Initialize Display
  glcdMode(0);
  glcdClear();
  setTextSize(1);

  // 2. Initialize Min/Max arrays
  // Min starts high (4095), Max starts low (0)
  int minVal[NUM_SENSORS] = { 4095, 4095, 4095, 4095 };
  int maxVal[NUM_SENSORS] = { 0, 0, 0, 0 };

  // 3. User Instruction
  glcd(0, 0, "Slide Robot...");

  // 4. Scanning Loop (5 Seconds)
  unsigned long timerStart = millis();

  while (millis() - timerStart < 5000) {

    // Read all sensors and update their Min and Max values automatically
    for (int i = 0; i < NUM_SENSORS; i++) {
      int currentReading = analog(sensorPins[i]);

      if (currentReading > maxVal[i]) maxVal[i] = currentReading;
      if (currentReading < minVal[i]) minVal[i] = currentReading;
    }

    // Show countdown (It seems your library supports %d but not %4d!)
    glcd(1, 0, "Wait: %d Sec  ", 5 - ((millis() - timerStart) / 1000));
    delay(5);
  }

  // 5. Calculate "Hyper-Sensitive" References directly into your PID array
  for (int i = 0; i < NUM_SENSORS; i++) {

    // Instead of exactly in the middle (50%), we use a 75% bias toward the Max (Floor).
    // This widens the "hitbox" of the sensor, catching thin lines way earlier!
    // Example: Min is 200, Max is 1000. Difference is 800. 800 * 0.75 = 600.
    // 200 + 600 = Target is now 800 instead of 600!

    int threshold_bias = (maxVal[i] - minVal[i]) * 0.75;  // Try 0.75, 0.80, or 0.85
    sensorRef[i] = minVal[i] + threshold_bias;
  }
  // 6. Display Data
  beep();
  glcdClear();

  // Show Calculated References using sprintf (safest way for your screen)
  glcd(0, 0, "Calibration Done!");

  char buf[20];
  sprintf(buf, "S0:%d S1:%d", sensorRef[0], sensorRef[1]);
  glcd(2, 0, buf);

  sprintf(buf, "S2:%d S3:%d", sensorRef[2], sensorRef[3]);
  glcd(3, 0, buf);

  glcd(5, 0, "Press OK...");
  sw_ok_press();
  glcdClear();
}

// int get_sensor_ref(int sensor){
//   switch(sensor){
//     case 0:return Ref_LL;
//     case 1:return Ref_L;
//     case 2:return Ref_R;
//     case 3:return Ref_RR;
//     default:return 0;
//   }
// }