void readSensors() {
  for (int idx = 0; idx < NUM_SENSORS; idx++) {
    sensorValue[idx] = analog(sensorPins[idx]);
  }
}

bool isBlack(int idx){
  return sensorValue[idx] < sensorRef[idx];
}
bool isWhite(int idx){
  return sensorValue[idx] >= sensorRef[idx];
}
bool centerDetected() {
  return isBlack(1) || isBlack(2);
}
bool crossFound() {
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

void sensors_Calibration() {
  glcdMode(0);
  glcdClear();
  setTextSize(1);

  int calibMinVals[NUM_SENSORS] = { 4095, 4095, 4095, 4095 };
  int calibMaxVals[NUM_SENSORS] = { 0, 0, 0, 0 };

  glcd(0, 0, "Slide Robot...");

  unsigned long calibClock = millis();

  while (millis() - calibClock < 5000) {

    for (int s_idx = 0; s_idx < NUM_SENSORS; s_idx++) {
      int liveSenseData = analog(sensorPins[s_idx]);

      if (liveSenseData > calibMaxVals[s_idx]) calibMaxVals[s_idx] = liveSenseData;
      if (liveSenseData < calibMinVals[s_idx]) calibMinVals[s_idx] = liveSenseData;
    }
    
    glcd(1, 0, "Wait: %d Sec  ", 5 - ((millis() - calibClock) / 1000));
    delay(5);
  }

  // Establishes a highly sensitive logic edge using 75% offset weighting toward absolute Floor bounds
  for (int s_idx = 0; s_idx < NUM_SENSORS; s_idx++) {
    int referenceTargetBias = (calibMaxVals[s_idx] - calibMinVals[s_idx]) * 0.75;  
    sensorRef[s_idx] = calibMinVals[s_idx] + referenceTargetBias;
  }
  
  beep();
  glcdClear();
  glcd(0, 0, "Calibration Done!");
  char valOutputBuffer[20];
  sprintf(valOutputBuffer, "S0:%d S1:%d", sensorRef[0], sensorRef[1]);
  glcd(2, 0, valOutputBuffer);
  sprintf(valOutputBuffer, "S2:%d S3:%d", sensorRef[2], sensorRef[3]);
  glcd(3, 0, valOutputBuffer);
  glcd(5, 0, "Press OK...");
  sw_ok_press();
  glcdClear();
}