void spinLeft_Sensor(int turningSpeed, int sensorIndex) {
  int min_speed = 15;
  unsigned long settle_timer = 0;

  sl(turningSpeed);

  // 1. DYNAMIC ESCAPE
  while (1) {
    readSensors();
    if (isWhite(1) && isWhite(2)) break;
  }

  // 2. TARGET ACQUISITION
  while (1) {
    readSensors();
    if (isBlack(sensorIndex)) break;
  }

  sl(min_speed);
  delay(TURN_DELAY);

  // 3. SETTLE PHASE / AUTO-ALIGN
  settle_timer = millis();
  while (1) {
    readSensors();
    if (isWhite(sensorIndex)) {
      // Skidded past the line! Counter-steer to pull back.
      sr(min_speed);
      settle_timer = millis();  // Reset stability clock
    } else {
      // Resting on the black line safely
      ao();
      if (millis() - settle_timer >= 50) break;  // 50ms of proven stability!
    }
  }

  PID_Reset();
}


void spinRight_Sensor(int turningSpeed, int sensorIndex) {
  int min_speed = 15;
  unsigned long settle_timer = 0;

  sr(turningSpeed);

  while (1) {
    readSensors();
    if (isWhite(1) && isWhite(2)) break;
  }

  while (1) {
    readSensors();
    if (isBlack(sensorIndex)) break;
  }

  sr(min_speed);
  delay(LOOP_DELAY);

  settle_timer = millis();
  while (1) {
    readSensors();
    if (isWhite(sensorIndex)) {
      sl(min_speed);
      settle_timer = millis();
    } else {
      ao();
      if (millis() - settle_timer >= 50) break;
    }
  }

  PID_Reset();
}


void spinLeft_SensorLineSkip(int baseTurningSpeed, int sensorIndex, int skipCount) {
  int true_turning_speed = compensateSpeed(baseTurningSpeed);
  int true_min_speed = compensateSpeed(15);
  
  // 1. THE GLARE & GEOMETRY ESCAPE
  sl(true_turning_speed);
  delay(50); // Blind push off the starting cross-intersection (ignores light glare)

  // 2. THE SKIP ALGORITHM
  for (int i = 1; i <= skipCount; i++) {

    // STAGE 1: DYNAMIC ESCAPE
    while (1) {
      readSensors();
      if (isWhite(sensorIndex)) break;
    }

    // SPATIAL DEBOUNCE (Noise filter)
    if (i < skipCount) delay(TURN_DELAY);

    // STAGE 2: TARGET ACQUISITION
    while (1) {
      readSensors();
      if (isBlack(sensorIndex)) break;
    }

    if (i < skipCount) delay(TURN_DELAY);
  }

  // 3. KINETIC DAMPENING (Fixed the bug!)
  ao();       // BRAKE IMMEDIATELY! Do NOT blindly keep driving with sl/delay.
  delay(20);  // Allow the chassis 20ms to naturally skid to a stop using floor friction.

  // 4. THE PRO SETTLE PHASE - [FAIL-SAFE EDITION]
  unsigned long settle_timer = millis();
  unsigned long settle_max_timeout = millis();

  // MAX TIMEOUT: If settle takes longer than 300ms, it is hallucinating! 
  // Break the loop and let the main PID forward-drive take over. 
  while (millis() - settle_max_timeout < 300) {
    readSensors();
    if (isWhite(sensorIndex)) {
      // Over-steered! We slipped too far LEFT. Counter steer Right slowly.
      sr(true_min_speed);
      settle_timer = millis();  // Reset the success clock
    } else {
      // Target Acquired: Brakes on.
      ao();
      // Must hold stable on the line for 50 uninterrupted ms!
      if (millis() - settle_timer >= 50) break;
    }
  }

  // Double check brakes are locked before returning to the main logic
  ao();
  PID_Reset();
}


void spinRight_SensorLineSkip(int baseTurningSpeed, int sensorIndex, int skipCount) {
  int true_turning_speed = compensateSpeed(baseTurningSpeed);
  int true_min_speed = compensateSpeed(15);
  
  // 1. THE GLARE & GEOMETRY ESCAPE
  sr(true_turning_speed);
  delay(50); 

  for (int i = 1; i <= skipCount; i++) {

    while (1) {
      readSensors();
      if (isWhite(sensorIndex)) break;
    }

    if (i < skipCount) delay(TURN_DELAY);

    while (1) {
      readSensors();
      if (isBlack(sensorIndex)) break;
    }

    if (i < skipCount) delay(TURN_DELAY);
  }

  // 3. KINETIC DAMPENING
  ao();       // Immediate Brake upon touching target!
  delay(20);

  // 4. THE PRO SETTLE PHASE - [FAIL-SAFE EDITION]
  unsigned long settle_timer = millis();
  unsigned long settle_max_timeout = millis();

  while (millis() - settle_max_timeout < 300) {
    readSensors();
    if (isWhite(sensorIndex)) {
      // Over-steered! We slipped too far RIGHT. Counter steer Left slowly.
      sl(true_min_speed);
      settle_timer = millis(); 
    } else {
      ao();
      if (millis() - settle_timer >= 50) break;
    }
  }

  ao();
  PID_Reset();
}
// void spinRight_SensorLineSkip(int baseTurningSpeed, int sensorIndex, int skipCount) {
//   int true_turning_speed = compensateSpeed(baseTurningSpeed);
//   int true_min_speed = compensateSpeed(15);
//   unsigned long settle_timer = 0;

//   sr(true_turning_speed);

//   for (int i = 1; i <= skipCount; i++) {

//     while (1) {
//       readSensors();
//       if (isWhite(sensorIndex)) break;
//     }

//     if (i < skipCount) delay(TURN_DELAY);

//     while (1) {
//       readSensors();
//       if (isBlack(sensorIndex)) break;
//     }

//     if (i < skipCount) delay(TURN_DELAY);
//   }

//   sr(true_min_speed);
//   delay(LOOP_DELAY);

//   // SETTLE PHASE
//   settle_timer = millis();
//   while (1) {
//     readSensors();
//     if (isWhite(sensorIndex)) {
//       // Counter steer Left for right spins
//       sl(true_min_speed);
//       settle_timer = millis();
//     } else {
//       ao();
//       if (millis() - settle_timer >= 50) break;
//     }
//   }

//   PID_Reset();
// }
