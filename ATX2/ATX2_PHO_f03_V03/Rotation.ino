void pivotLeftToLine(int turnCommandSpeed, int targetSensorPin) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  
  sl(activeSpinPwm);
  delay(50); // Blind thrust to guarantee we escape the current line

  while (1) {
    readSensors();
    if (isWhite(targetSensorPin)) break; 
  }
  delay(TURN_DELAY_MS);

  while (1) {
    readSensors();
    if (isBlack(targetSensorPin)) break;
  }

  // --- ACTIVE BRAKING PHASE ---
  sr(activeSpinPwm);  // Brief counter-spin to kill rotational kinetic energy
  delay(15);          // 15ms pulse (adjust to 10-20ms depending on robot weight)
  ao();               // Electronic motor lock
  delay(40);          // Let the physical chassis vibrations settle
  
  resetPIDVariables();
}

void pivotRightToLine(int turnCommandSpeed, int targetSensorPin) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  
  sr(activeSpinPwm);
  delay(50); 

  while (1) {
    readSensors();
    if (isWhite(targetSensorPin)) break; 
  }
  delay(TURN_DELAY_MS);

  while (1) {
    readSensors();
    if (isBlack(targetSensorPin)) break;
  }

  // --- ACTIVE BRAKING PHASE ---
  sl(activeSpinPwm);  
  delay(15);          
  ao();               
  delay(40);          
  
  resetPIDVariables();
}

void pivotLeftSkipLines(int turnCommandSpeed, int targetSensorPin, int lineSkipAmount) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  
  sl(activeSpinPwm);
  delay(50); 

  for (int skipIteration = 1; skipIteration <= lineSkipAmount; skipIteration++) {
    while (1) {
      readSensors();
      if (isWhite(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY_MS);

    while (1) {
      readSensors();
      if (isBlack(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY_MS);
  }

  // --- ACTIVE BRAKING PHASE ---
  sr(activeSpinPwm);  
  delay(15);          
  ao();               
  delay(40);          
  
  resetPIDVariables();
}

void pivotRightSkipLines(int turnCommandSpeed, int targetSensorPin, int lineSkipAmount) {
  int activeSpinPwm = compensateSpeed(turnCommandSpeed);
  
  sr(activeSpinPwm);
  delay(50); 

  for (int skipIteration = 1; skipIteration <= lineSkipAmount; skipIteration++) {
    while (1) {
      readSensors();
      if (isWhite(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY_MS);

    while (1) {
      readSensors();
      if (isBlack(targetSensorPin)) break;
    }
    if (skipIteration < lineSkipAmount) delay(TURN_DELAY_MS);
  }

  // --- ACTIVE BRAKING PHASE ---
  sl(activeSpinPwm);  
  delay(15);          
  ao();               
  delay(40);          
  
  resetPIDVariables();
}