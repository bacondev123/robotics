void pivotLeftToLine(int turnCommandSpeed, int targetSensorPin) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  int antiStallPwm = compensateSpeed(15);
  
  sl(activeSpinPwm);
  delay(50); 

  while (1) {
    readSensors();
    if (isWhite(targetSensorPin)) break; 
  }
  delay(TURN_DELAY);

  while (1) {
    readSensors();
    if (isBlack(targetSensorPin)) break;
  }

  ao();
  delay(20);

  unsigned long alignStartTime = millis();
  unsigned long safeTimeoutClock = millis();
  
  while (millis() - safeTimeoutClock < 300) {
    readSensors();
    if (isWhite(targetSensorPin)) {
      sr(antiStallPwm); 
      alignStartTime = millis();  
    } else {
      ao();
      if (millis() - alignStartTime >= 50) break;
    }
  }
  
  ao();
  resetPIDVariables();
}

void pivotRightToLine(int turnCommandSpeed, int targetSensorPin) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  int antiStallPwm = compensateSpeed(15);
  
  sr(activeSpinPwm);
  delay(50); 

  while (1) {
    readSensors();
    if (isWhite(targetSensorPin)) break; 
  }
  delay(TURN_DELAY);

  while (1) {
    readSensors();
    if (isBlack(targetSensorPin)) break;
  }

  ao();
  delay(20);

  unsigned long alignStartTime = millis();
  unsigned long safeTimeoutClock = millis();
  
  while (millis() - safeTimeoutClock < 300) {
    readSensors();
    if (isWhite(targetSensorPin)) {
      sl(antiStallPwm);
      alignStartTime = millis();  
    } else {
      ao();
      if (millis() - alignStartTime >= 50) break; 
    }
  }

  ao();
  resetPIDVariables();
}

void pivotLeftSkipLines(int turnCommandSpeed, int targetSensorPin, int lineSkipAmount) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  int antiStallPwm = compensateSpeed(15);
  
  sl(activeSpinPwm);
  delay(50); 

  for (int skipIteration = 1; skipIteration <= lineSkipAmount; skipIteration++) {
    while (1) {
      readSensors();
      if (isWhite(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY);

    while (1) {
      readSensors();
      if (isBlack(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY);
  }

  ao();      
  delay(20); 

  unsigned long alignStartTime = millis();
  unsigned long safeTimeoutClock = millis();

  while (millis() - safeTimeoutClock < 300) {
    readSensors();
    if (isWhite(targetSensorPin)) {
      sr(antiStallPwm);
      alignStartTime = millis();  
    } else {
      ao();
      if (millis() - alignStartTime >= 50) break;
    }
  }

  ao();
  resetPIDVariables();
}

void pivotRightSkipLines(int turnCommandSpeed, int targetSensorPin, int lineSkipAmount) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  int antiStallPwm = compensateSpeed(15);
  
  sr(activeSpinPwm);
  delay(50); 

  for (int skipIteration = 1; skipIteration <= lineSkipAmount; skipIteration++) {
    while (1) {
      readSensors();
      if (isWhite(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY);

    while (1) {
      readSensors();
      if (isBlack(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY);
  }

  ao();
  delay(20);

  unsigned long alignStartTime = millis();
  unsigned long safeTimeoutClock = millis();

  while (millis() - safeTimeoutClock < 300) {
    readSensors();
    if (isWhite(targetSensorPin)) {
      sl(antiStallPwm);
      alignStartTime = millis(); 
    } else {
      ao();
      if (millis() - alignStartTime >= 50) break;
    }
  }

  ao();
  resetPIDVariables();
}