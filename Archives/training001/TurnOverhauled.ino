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
  unsigned long settle_timer = 0;

  sl(true_turning_speed);

  for (int i = 1; i <= skipCount; i++) {

    // STAGE 1: DYNAMIC ESCAPE
    while (1) {
      readSensors();
      if (isWhite(sensorIndex)) break;
    }

    // SPATIAL DEBOUNCE: Push blindly through blurry edges
    // (Only if we have not reached our final destination yet)
    if (i < skipCount) delay(TURN_DELAY);

    // STAGE 2: TARGET ACQUISITION
    while (1) {
      readSensors();
      if (isBlack(sensorIndex)) break;
    }

    // SPATIAL DEBOUNCE: Blind lock-in on intermediate lines
    if (i < skipCount) delay(TURN_DELAY);
  }

  sl(true_min_speed);
  delay(LOOP_DELAY);

  // STAGE 3: THE PRO SETTLE PHASE
  settle_timer = millis();
  while (1) {
    readSensors();
    if (isWhite(sensorIndex)) {
      // Sliding! Reverse slightly to stay hooked!
      sr(true_min_speed);
      settle_timer = millis();  // Reset clock
    } else {
      // Locked solid. Brake.
      ao();
      if (millis() - settle_timer >= 50) break;
    }
  }

  PID_Reset();
}


void spinRight_SensorLineSkip(int baseTurningSpeed, int sensorIndex, int skipCount) {
  int true_turning_speed = compensateSpeed(baseTurningSpeed);
  int true_min_speed = compensateSpeed(15);
  unsigned long settle_timer = 0;

  sr(true_turning_speed);

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

  sr(true_min_speed);
  delay(LOOP_DELAY);

  // SETTLE PHASE
  settle_timer = millis();
  while (1) {
    readSensors();
    if (isWhite(sensorIndex)) {
      // Counter steer Left for right spins
      sl(true_min_speed);
      settle_timer = millis();
    } else {
      ao();
      if (millis() - settle_timer >= 50) break;
    }
  }

  PID_Reset();
}
// void spinLeft_Sensor(int turningSpeed, int sensorIndex) {
//   // 1. Send the command ONCE
//   sl(turningSpeed);

//   // 2. STAGE 1: DYNAMIC ESCAPE
//   // Spin until ALL inner sensors completely leave the old line
//   while (1) {
//     readSensors();
//     // If S1 and S2 both see the white floor, we have cleared the line!
//     if (isWhite(1) && isWhite(2)) break;
//   }

//   // 3. STAGE 2: TARGET ACQUISITION
//   // Keep spinning until the sensor we want hits the new line
//   while (1) {
//     readSensors();
//     if (isBlack(sensorIndex)) break;
//   }

//   // 4. Clean Brake
//   ao();

//   // Extra Upgrade: Instead of a harsh delay, let's instantly trigger PID!
//   // resetting PID here stops the robot from twitching violently
//   // right after a corner.
//   PID_Reset();
//   delay(LOOP_DELAY);
// }


// void spinRight_Sensor(int turningSpeed, int sensorIndex) {
//   sr(turningSpeed);

//   // Dynamic Escape
//   while (1) {
//     readSensors();
//     if (isWhite(1) && isWhite(2)) break;
//   }

//   // Target Acquisition
//   while (1) {
//     readSensors();
//     if (isBlack(sensorIndex)) break;
//   }

//   ao();
//   PID_Reset();
//   delay(LOOP_DELAY);
// }
// void spinLeft_SensorLineSkip(int turningSpeed, int sensorIndex, int skipCount) {
//   // 1. Send the spin command ONCE
//   sl(turningSpeed);

//   // 2. Loop through the number of lines we want to hit!
//   // If skipCount is 0, this loop only runs exactly 1 time (Normal Turn).
//   // If skipCount is 2, it runs 3 times (Skips 2 lines, grabs the 3rd).
//   for (int i = 1; i <= skipCount; i++) {

//     // STAGE 1: DYNAMIC ESCAPE (Wait for the white floor)
//     while (1) {
//       readSensors();
//       // By checking the EXACT sensor we are turning with,
//       // it perfectly acts as a counter for every "spoke" of the star intersection!
//       if (isWhite(sensorIndex)) break;
//     }

//     delay(TURN_DELAY);

//     // STAGE 2: TARGET ACQUISITION (Wait for the new black line)
//     while (1) {
//       readSensors();
//       if (isBlack(sensorIndex)) break;
//     }
//     delay(TURN_DELAY);
//   }

//   // 3. Clean Brake & Reset
//   ao();
//   PID_Reset();
//   delay(LOOP_DELAY);
// }


// void spinRight_SensorLineSkip(int turningSpeed, int sensorIndex, int skipCount) {
//   sr(turningSpeed);

//   for (int i = 1; i <= skipCount; i++) {
//     // Dynamic Escape
//     while (1) {
//       readSensors();
//       if (isWhite(sensorIndex)) break;
//     }

//     delay(TURN_DELAY);

//     // Target Acquisition
//     while (1) {
//       readSensors();
//       if (isBlack(sensorIndex)) break;
//     }
//     delay(TURN_DELAY);
//   }

//   ao();
//   PID_Reset();
//   delay(LOOP_DELAY);
// }