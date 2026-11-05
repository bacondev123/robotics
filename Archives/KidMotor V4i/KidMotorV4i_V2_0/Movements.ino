void driveForward(int maxSpeed, unsigned long totalTimer) {
  int trueMinSpeed = compensateSpeed(15);         
  int trueMaxSpeed = compensateSpeed(maxSpeed);  
  unsigned long startTime = millis();

  while (1) {
    unsigned long elapsedTime = millis() - startTime;
    if (elapsedTime >= totalTimer) break;

    unsigned long remainingTime = totalTimer - elapsedTime;
    int currentSpeed = trueMinSpeed;  

    if (elapsedTime <= 150) {
      currentSpeed = trueMinSpeed + (elapsedTime * (trueMaxSpeed - trueMinSpeed) / 150);
    } else if (remainingTime <= 150) {
      currentSpeed = trueMinSpeed + (remainingTime * (trueMaxSpeed - trueMinSpeed) / 150);
      if (currentSpeed < trueMinSpeed) currentSpeed = trueMinSpeed;  
    } else {
      currentSpeed = trueMaxSpeed;
    }

    fd2(currentSpeed, currentSpeed + 2); 
  }
  ao();
  resetPID();
  delay(Config::LOOP_DELAY);
}

void driveForwardTime(unsigned long timer) {
  driveForward(20, timer);
}

void trackCross() {
  while (1) {
    if (isCrossDetected()) break;
    trackLinePID(30, 4.0, 0.1, 0.03);
  }
  ao();
  resetPID();
  delay(Config::LOOP_DELAY);
}

void skipCross(int count) {
  for (int i = 1; i <= count; i++) {
    driveForward(20, Config::SMALL_LINE_SKIP);
    trackCross();
    driveForwardTime(Config::LINE_SKIP);
  }
  ao();
  delay(Config::LOOP_DELAY);
}

void trackSideLine(int sensorIndex) {
  while (1) {
    readSensors();
    if (isBlack(sensorIndex)) break;
    trackLinePID(20, 3.0, 0.5, 0.02);
  }
  ao();
  delay(Config::LOOP_DELAY);
}

void skipSideLine(int count, int sensorIndex) {
  for (int i = 1; i <= count; i++) {
    driveForward(20, Config::SMALL_LINE_SKIP);
    trackSideLine(sensorIndex);
    driveForwardTime(Config::LINE_SKIP);
  }
  ao();
  delay(Config::LOOP_DELAY);
}

void spinLeftToLine(int turningSpeed, int sensorIndex) {
  sl(turningSpeed);
  while (1) { readSensors(); if (isWhite(1) && isWhite(2)) break; }
  while (1) { readSensors(); if (isBlack(sensorIndex)) break; }

  sl(15);
  delay(Config::TURN_DELAY);

  unsigned long settleTimer = millis();
  while (1) {
    readSensors();
    if (isWhite(sensorIndex)) {
      sr(15); 
      settleTimer = millis();  
    } else {
      ao();
      if (millis() - settleTimer >= 50) break;  
    }
  }
  resetPID();
}

void spinRightToLine(int turningSpeed, int sensorIndex) {
  sr(turningSpeed);
  while (1) { readSensors(); if (isWhite(1) && isWhite(2)) break; }
  while (1) { readSensors(); if (isBlack(sensorIndex)) break; }

  sr(15);
  delay(Config::TURN_DELAY);

  unsigned long settleTimer = millis();
  while (1) {
    readSensors();
    if (isWhite(sensorIndex)) {
      sl(15);
      settleTimer = millis();  
    } else {
      ao();
      if (millis() - settleTimer >= 50) break;  
    }
  }
  resetPID();
}

void spinLeftSkipLines(int baseSpeed, int sensorIndex, int skipCount) {
  sl(compensateSpeed(baseSpeed));
  delay(50); 

  for (int i = 1; i <= skipCount; i++) {
    while (1) { readSensors(); if (isWhite(sensorIndex)) break; }
    if (i < skipCount) delay(Config::TURN_DELAY);
    
    while (1) { readSensors(); if (isBlack(sensorIndex)) break; }
    if (i < skipCount) delay(Config::TURN_DELAY);
  }

  ao();
  delay(20);

  unsigned long settleTimer = millis();
  unsigned long maxTimeout = millis();
  while (millis() - maxTimeout < 300) {
    readSensors();
    if (isWhite(sensorIndex)) {
      sr(compensateSpeed(15));
      settleTimer = millis(); 
    } else {
      ao();
      if (millis() - settleTimer >= 50) break;
    }
  }
  ao();
  resetPID();
}

void spinRightSkipLines(int baseSpeed, int sensorIndex, int skipCount) {
  sr(compensateSpeed(baseSpeed));
  delay(50); 

  for (int i = 1; i <= skipCount; i++) {
    while (1) { readSensors(); if (isWhite(sensorIndex)) break; }
    if (i < skipCount) delay(Config::TURN_DELAY);

    while (1) { readSensors(); if (isBlack(sensorIndex)) break; }
    if (i < skipCount) delay(Config::TURN_DELAY);
  }

  ao();     
  delay(20);

  unsigned long settleTimer = millis();
  unsigned long maxTimeout = millis();

  while (millis() - maxTimeout < 300) {
    readSensors();
    if (isWhite(sensorIndex)) {
      sl(compensateSpeed(15));
      settleTimer = millis(); 
    } else {
      ao();
      if (millis() - settleTimer >= 50) break;
    }
  }
  ao();
  resetPID();
}