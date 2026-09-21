#include <stdlib.h> 
#include <ctype.h>  

void executePathSequence(const char* pathStr) {

  char pathBuffer[64];
  strlcpy(pathBuffer, pathStr, sizeof(pathBuffer));
  char* command = strtok(pathBuffer, " ");

  while (command != NULL) {

    int cmdLen = strlen(command);
    char firstChar = command[0];
    char lastChar = command[cmdLen - 1];

    if (cmdLen >= 2 && isdigit(firstChar) && lastChar == 'W') {
      int driveTime = atoi(command);
      if (driveTime <= 9) driveTime *= 100;
      driveForwardConstant(driveTime);
    }
    else if (cmdLen == 1) {
      if (firstChar == 'S') skipCrossIntersections(1);
      else if (firstChar == 'T') followUntilIntersection();
      else if (firstChar == 'w') driveForwardConstant(300);
      else if (firstChar == 'W') driveForwardConstant(400);
    }
    else if (cmdLen == 2) {
      char actionType = command[0];
      int sensorIndex = command[1] - '0';
      if (sensorIndex < 0 || sensorIndex > 9) sensorIndex = 0; 

      if (actionType == 'T') followUntilSensorDetected(sensorIndex);
      else if (actionType == 'S') skipSideLineJunctions(1, sensorIndex);
      else if (actionType == 'L') pivotLeftToLine(30, sensorIndex);
      else if (actionType == 'R') pivotRightToLine(30, sensorIndex);
    }
    else if (cmdLen == 3) {
      int skipCount = command[0] - '0';
      char actionType = command[1];
      int sensorIndex = command[2] - '0';

      if (skipCount < 1 || skipCount > 9) skipCount = 1;
      if (sensorIndex < 0 || sensorIndex > 9) sensorIndex = 0;

      if (actionType == 'L') pivotLeftSkipLines(30, sensorIndex, skipCount);
      else if (actionType == 'R') pivotRightSkipLines(30, sensorIndex, skipCount);
    }
    command = strtok(NULL, " ");
  }
}

void followUntilSensorDetected(int sensorIndex) {
  float baseSpeed = 20.0;
  float kP = 3.0, kI = 0.5, kD = 0.02;
  int hitCount = 0; 

  while (1) {
    readSensors();
    if (isBlack(sensorIndex)) {
      hitCount++;
      if (hitCount >= 2) break; 
    } else {
      hitCount = 0; 
      driveLineFollowerPID(baseSpeed, kP, kI, kD);
    }
  }
  ao();
  delay(LOOP_DELAY_MS);
}

void skipSideLineJunctions(int passCount, int sensorIndex) {
  for (int i = 1; i <= passCount; i++) {
    driveForwardVelocityProfile(20, SMALL_LINE_SKIP_MS);
    followUntilSensorDetected(sensorIndex);
    driveForwardConstant(LINE_SKIP_MS);
  }
  ao();
  delay(LOOP_DELAY_MS);
}

void skipCrossIntersections(int crossJumpCount) {
  for (int i = 1; i <= crossJumpCount; i++) {
    driveForwardVelocityProfile(20, SMALL_LINE_SKIP_MS);
    followUntilIntersection();
    driveForwardConstant(LINE_SKIP_MS);
  }
  ao();
  delay(LOOP_DELAY_MS);
}

void followUntilIntersection() {
  float baseSpeed = 30.0;
  float kP = 4.0, kI = 0.1, kD = 0.03;
  int crossHitCount = 0;

  while (1) {
    readSensors();
    bool crossFound = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);

    if (crossFound) {
      crossHitCount++;
      if (crossHitCount >= 2) break;
    } else {
      crossHitCount = 0;
      driveLineFollowerPID(baseSpeed, kP, kI, kD);
    }
  }
  ao();
  resetPIDVariables();
  delay(LOOP_DELAY_MS);
}

void traverseIntersectionBlind(float kP, float kI, float kD, unsigned long escapeTimeMs) {
  float safeSpeed = 35.0;
  unsigned long searchStartTime = millis();
  int crossHitCount = 0;

  // Phase 1: Wait to escape initial junction logic constraint, then hunt for next crossing.
  while (1) {
    readSensors();
    bool crossFound = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);

    if (crossFound && (millis() - searchStartTime > 250)) {
      crossHitCount++;
      if (crossHitCount >= 2) break;  
    } else {
      crossHitCount = 0;
      driveLineFollowerPID(safeSpeed, kP, kI, kD);
    }
  }

  // Phase 2: Forward push
  unsigned long blindDriveTimer = millis();
  while (millis() - blindDriveTimer < escapeTimeMs) {
    fd2(safeSpeed, safeSpeed + 2.0); // Maintain structural right bias mapping assumption. 
  }
  
  ao();
  resetPIDVariables();
  delay(LOOP_DELAY_MS);
}

void driveForwardVelocityProfile(int maxTargetSpeed, unsigned long travelDurationMs) {
  int safeMinVoltage = compensateSpeed(15);
  int targetMaxVoltage = compensateSpeed(maxTargetSpeed);
  int spoolTime = 150;                           
  int brakeDownTime = 150;                         

  unsigned long startTime = millis();

  while (1) {
    unsigned long timeActive = millis() - startTime;
    if (timeActive >= travelDurationMs) break;

    unsigned long timeRemaining = travelDurationMs - timeActive;
    int calculatedPwmOut = safeMinVoltage;  

    if (timeActive <= spoolTime) {
      calculatedPwmOut = safeMinVoltage + (timeActive * (targetMaxVoltage - safeMinVoltage) / spoolTime);
    }
    else if (timeRemaining <= brakeDownTime) {
      calculatedPwmOut = safeMinVoltage + (timeRemaining * (targetMaxVoltage - safeMinVoltage) / brakeDownTime);
      if (calculatedPwmOut < safeMinVoltage) calculatedPwmOut = safeMinVoltage;
    }
    else {
      calculatedPwmOut = targetMaxVoltage;
    }

    fd2(calculatedPwmOut, calculatedPwmOut + 2.0); 
  }

  ao();
  resetPIDVariables();
  delay(LOOP_DELAY_MS);
}

void driveForwardConstant(unsigned long travelTimeMs) {
  driveForwardVelocityProfile(20, travelTimeMs); 
}