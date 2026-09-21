float cumulativeIntegral = 0;
float preCalcError = 0;
float currentTractionError = 0;
float cachedRobotLinePos = 1500.0;
static float systemPacingPwm = 15.0;

float computePIDCorrection(float Kp, float Ki, float Kd, float dtScalar) {
  float ProportionalTerm = currentTractionError;

  if (abs(currentTractionError) <= 1) {
    cumulativeIntegral += currentTractionError * dtScalar;
    cumulativeIntegral = constrain(cumulativeIntegral, -100, 100);
  } else {
    cumulativeIntegral = 0;
  }

  float DerivativeRawTerm = (currentTractionError - preCalcError) / dtScalar;
  float DerivativeAbsorber = constrain(DerivativeRawTerm, -150.0, 150.0);

  return (Kp * ProportionalTerm) + (Ki * cumulativeIntegral) + (Kd * DerivativeAbsorber);
}

float estimateLinePosition() {
  long spatialSummation = 0;
  long boundaryTotalWeight = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    long activeDrawIntensity = sensorRef[i] - sensorValue[i];
    if (activeDrawIntensity > 0) {
      spatialSummation += (activeDrawIntensity * (i * 1000L));
      boundaryTotalWeight += activeDrawIntensity;
    }
  }

  float resultingAlignmentCenter = 1500.0;

  if (boundaryTotalWeight <= 0) {
    if (cachedRobotLinePos >= (CENTER_POS - 250) && cachedRobotLinePos <= (CENTER_POS + 250)) {
      resultingAlignmentCenter = CENTER_POS;
    } else if (cachedRobotLinePos < (CENTER_POS - 250)) {
      resultingAlignmentCenter = MIN_POS;
    } else {
      resultingAlignmentCenter = MAX_POS;
    }

    cachedRobotLinePos = resultingAlignmentCenter;
  } else {
    resultingAlignmentCenter = (float)spatialSummation / (float)boundaryTotalWeight;
    cachedRobotLinePos = resultingAlignmentCenter;  // Remove the 0.6 / 0.4 smoothing!
  }

  return cachedRobotLinePos;
}


void driveLineFollowerPID(float targetTopSpeed, float Kp, float Ki, float Kd) {

  const unsigned long TICK_REFRESH_US = 5000;
  static unsigned long priorExecutionMark = micros(); 

  unsigned long runtimeStamp = micros();
  
  if(runtimeStamp - priorExecutionMark > 1000000){ priorExecutionMark = runtimeStamp; return; }

  if (runtimeStamp - priorExecutionMark >= TICK_REFRESH_US) {

    float systemDt = (runtimeStamp - priorExecutionMark) / 1000000.0;
    if (systemDt <= 0) systemDt = 0.001;
    if (systemDt > 0.05) systemDt = 0.005;

    priorExecutionMark = runtimeStamp;

    readSensors();
    float lineDriftFactor = estimateLinePosition();  

    // Max error is between -3.0 and +3.0
    currentTractionError = (lineDriftFactor - CENTER_POS) / 500.0;

    // --- SIMPLIFIED SPEED CONTROL ---
    // Smoothly slow down based on how far off the line we are, without violent braking.
    float requiredSpeedDamping = targetTopSpeed - (abs(currentTractionError) * 5.0);
    
    // Never let the base speed drop below 15 to prevent stuttering/stopping
    if (requiredSpeedDamping < 15.0) requiredSpeedDamping = 15.0; 
    
    // Apply instantly (Removes the laggy accel/decel state machine that causes yo-yoing)
    systemPacingPwm = requiredSpeedDamping; 

    // Calculate steering
    float appliedTurningOffset = computePIDCorrection(Kp, Ki, Kd, systemDt);
    
    float unfilteredLeftPwm = systemPacingPwm + appliedTurningOffset;
    float unfilteredRightPwm = systemPacingPwm - appliedTurningOffset;

    float HW_DRIVE_BIAS = MOTOR_BIAS; 
    
    float cleanLeftEngPwm = constrain(compensateSpeed(unfilteredLeftPwm) + HW_DRIVE_BIAS, -100, 100);
    float cleanRightEngPwm = constrain(compensateSpeed(unfilteredRightPwm), -100, 100);

    fd2(round(cleanLeftEngPwm), round(cleanRightEngPwm));
    preCalcError = currentTractionError;
  }
}


void drivePIDAccelerationProfile(float maximumCeilingPwm, unsigned long totalTimeMs) {
  float tuneKp = 4.0, tuneKi = 0.1, tuneKd = 0.03;
  float trueMaxVelocity = maximumCeilingPwm;
  float trueMinVelocity = 15.0;

  int timeToSpoolUp = 500;
  int timeToBrakeDown = 750;

  unsigned long startClock = millis();

  while (1) {
    unsigned long timeElapsed = millis() - startClock;
    if (timeElapsed >= totalTimeMs) break;

    unsigned long remainingTime = totalTimeMs - timeElapsed;
    float dynamicallyCalculatedVelocity = trueMinVelocity;

    if (timeElapsed <= timeToSpoolUp) {
      dynamicallyCalculatedVelocity = trueMinVelocity + ((float)timeElapsed * (trueMaxVelocity - trueMinVelocity) / timeToSpoolUp);
    } else if (remainingTime <= timeToBrakeDown) {
      dynamicallyCalculatedVelocity = trueMinVelocity + ((float)remainingTime * (trueMaxVelocity - trueMinVelocity) / timeToBrakeDown);
      if (dynamicallyCalculatedVelocity < trueMinVelocity) dynamicallyCalculatedVelocity = trueMinVelocity;
    } else {
      dynamicallyCalculatedVelocity = trueMaxVelocity;
    }

    driveLineFollowerPID(dynamicallyCalculatedVelocity, tuneKp, tuneKi, tuneKd);
  }

  ao();
  resetPIDVariables();
}

void drivePIDConstantProfile(float targetTopSpeed, unsigned long timeBoundaryMs) {
  float tuneKp = 3.0, tuneKi = 0.5, tuneKd = 0.02;
  unsigned long operationCommenceTick = millis();

  while (millis() - operationCommenceTick <= timeBoundaryMs) {
    driveLineFollowerPID(targetTopSpeed, tuneKp, tuneKi, tuneKd);
  }

  ao();
  resetPIDVariables();
}

void resetPIDVariables() {
  cumulativeIntegral = 0;
  currentTractionError = 0;
  preCalcError = 0;
  systemPacingPwm = 15.0;
}

// Development calibration engine
void tunePIDHardwareRoutine(float throttlePwm, float testKp, float testKi, float testKd) {
  const unsigned long CYCLE_RES = 5000;
  static unsigned long priorExecution = 0;
  unsigned long nowMicroT = micros();

  if (nowMicroT - priorExecution >= CYCLE_RES) {
    float evalDt = (nowMicroT - priorExecution) / 1000000.0;
    if (evalDt <= 0) evalDt = 0.001;

    priorExecution = nowMicroT;
    readSensors();
    float lineBiasOutput = estimateLinePosition();

    currentTractionError = (lineBiasOutput - CENTER_POS) / 500.0;

    float resultantCalculatedAdjust = computePIDCorrection(testKp, testKi, testKd, evalDt);

    float debugLeftWheel = constrain(throttlePwm + resultantCalculatedAdjust, -100, 100);
    float debugRightWheel = constrain(throttlePwm - resultantCalculatedAdjust, -100, 100);

    fd2(debugLeftWheel, debugRightWheel);
    preCalcError = currentTractionError;
  }
}