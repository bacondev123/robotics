void readSensors() {
  // Read analog values from all connected sensors and store them in an array
  for (int idx = 0; idx < NUM_SENSORS; idx++) {
    sensorValue[idx] = analog(sensorPins[idx]);
  }
}

bool isBlack(int idx){
  // Check if a specific sensor is over a black line
  return sensorValue[idx] < sensorRef[idx];
}

bool isWhite(int idx){
  // Check if a specific sensor is over a white surface
  return sensorValue[idx] >= sensorRef[idx];
}

bool centerDetected() {
  // Check if the middle sensors (1 or 2) detect black line 
  return isBlack(1) || isBlack(2);
}

bool crossFound() {
  // Check if the outer sensors (0 and 3) detect a cross intersection
  readSensors();
  return sensorValue[0] < sensorRef[0] && sensorValue[3] < sensorRef[3];
}

bool allBlack() {
  // Return true if all sensors are reading black
  for (int i = 0; i < NUM_SENSORS; i++){
    if(!isBlack(i)) return false;
  }
  return true;
}

bool allWhite(){
  // Return true if all sensors are reading white
  for(int i = 0; i < NUM_SENSORS; i++){
    if(isBlack(i)) return false;
  }
  return true;
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
void sensors_Calibration() {
  oled.clear();
  // glcdMode(0) and setTextSize(1) are not needed for POP32 basic oled.text

  int calibMinVals[NUM_SENSORS] = { 4095, 4095, 4095, 4095 };
  int calibMaxVals[NUM_SENSORS] = { 0, 0, 0, 0 };

  oled.text(0, 0, "Slide Robot...");
  oled.show(); // POP32 requires oled.show() to push text to the screen

  unsigned long calibClock = millis();

  while (millis() - calibClock < 5000) {

    for (int s_idx = 0; s_idx < NUM_SENSORS; s_idx++) {
      int liveSenseData = analog(sensorPins[s_idx]);

      if (liveSenseData > calibMaxVals[s_idx]) calibMaxVals[s_idx] = liveSenseData;
      if (liveSenseData < calibMinVals[s_idx]) calibMinVals[s_idx] = liveSenseData;
    }
    
    // POP32's oled.text natively supports formatting (like %d)
    oled.text(1, 0, "Wait: %d Sec  ", 5 - ((millis() - calibClock) / 1000));
    oled.show(); 
    delay(5);
  }

  // Establishes a highly sensitive logic edge using 60% offset weighting toward absolute Floor bounds
  for (int s_idx = 0; s_idx < NUM_SENSORS; s_idx++) {
    int referenceTargetBias = (calibMaxVals[s_idx] - calibMinVals[s_idx]) * 0.60;  
    sensorRef[s_idx] = calibMinVals[s_idx] + referenceTargetBias;
  }
  
  beep();
  oled.clear();
  oled.text(0, 0, "Calibration Done!");
  
  // No sprintf/buffer needed anymore, oled.text prints formatted strings directly
  oled.text(2, 0, "S0:%d S1:%d", sensorRef[0], sensorRef[1]);
  oled.text(3, 0, "S2:%d S3:%d", sensorRef[2], sensorRef[3]);
  oled.text(5, 0, "Press OK...");
  oled.show();
  
  waitSW_OK(); // Replaced ATX2 sw_ok_press()
  oled.clear();
  oled.show();
}