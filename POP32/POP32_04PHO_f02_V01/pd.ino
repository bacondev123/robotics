// =========================================================
// SIMPLE & CRISP PD LINE FOLLOWER ENGINE (POP32)
// =========================================================
//Stable PD tuning list:
//    maxspeed        Kp           Ki          Kd
//      20       ||   8      ||   0  ||   2


#define CENTER_POS 1500.0
#define MIN_POS    0.0
#define MAX_POS    3000.0

float lastPosition = CENTER_POS;
float lastError    = 0.0;
float integral     = 0.0;

// Reset PID memory
void resetPD() {
  lastError    = 0.0;
  integral     = 0.0;
  lastPosition = CENTER_POS;
}

// ---------------------------------------------------------
// 1. CALCULATE WEIGHTED LINE POSITION (0 to 3000, Center = 1500)
// ---------------------------------------------------------
float getLinePosition() {
  readSensors();

  long sum = 0;
  long totalWeight = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    long weight = sensorRef[i] - sensorValue[i]; // Higher weight = more black
    if (weight > 0) {
      sum += (weight * (i * 1000L));
      totalWeight += weight;
    }
  }

  // Line Lost Memory: if no black detected, remember the last exit direction
  if (totalWeight <= 0) {
    if (lastPosition < (CENTER_POS - 250.0)) {
      lastPosition = MIN_POS;       // Was on the far left
    } else if (lastPosition > (CENTER_POS + 250.0)) {
      lastPosition = MAX_POS;       // Was on the far right
    } else {
      lastPosition = CENTER_POS;
    }
  } else {
    lastPosition = (float)sum / (float)totalWeight;
  }

  return lastPosition;
}

// ---------------------------------------------------------
// 2. CORE PID CONTROLLER STEP
// ---------------------------------------------------------
void trackPD(float speed, float Kp, float Kd, float Ki = 0.0) {
  float position = getLinePosition();
  
  // Normalize error: scale between -3.0 (Far Left) to +3.0 (Far Right)
  float error = (position - CENTER_POS) / 500.0;

  // P Term (Proportional)
  float P = Kp * error;

  // I Term (Integral) with Anti-Windup (only accumulate near center)
  if (abs(error) <= 1.0) {
    integral += error;
    integral = constrain(integral, -15.0, 15.0);
  } else {
    integral = 0.0; 
  }
  float I = Ki * integral;

  // D Term (Derivative)
  float D = Kd * (error - lastError);

  // Compute total motor differential power
  float power = P + I + D;

  float leftSpeed  = speed + power;
  float rightSpeed = speed - power;

  leftSpeed  = constrain(leftSpeed, -100.0, 100.0);
  rightSpeed = constrain(rightSpeed, -100.0, 100.0);

  fd2(round(leftSpeed + MOTOR_BIAS), round(rightSpeed));

  lastError = error;
}
void trackPD_Left(float speed, float Kp, float Kd) {
  float position = getLinePosition();
  
  // Normalize error: scale between -3.0 (Far Left) to +3.0 (Far Right)
  float error = (position - CENTER_POS) / 500.0;

  // P Term (Proportional)
  float P = Kp * error;

  // D Term (Derivative)
  float D = Kd * (error - lastError);

  // Compute total motor differential power
  float power = P + D;

  float leftSpeed  = speed + power;
  float rightSpeed = speed - power;

  leftSpeed  = constrain(leftSpeed, -100.0, 100.0);
  rightSpeed = constrain(rightSpeed, -100.0, 100.0);

  fd2(round(leftSpeed + MOTOR_BIAS), round(rightSpeed + 10));

  lastError = error;
}

// ---------------------------------------------------------
// 3. RUN PID FOR A FIXED TIME DURATION (in Milliseconds)
// ---------------------------------------------------------
void trackPD_Time(float speed, float Kp, float Kd, unsigned long durationMs, float Ki = 0.0) {
  resetPD();
  unsigned long startClock = millis();

  while (millis() - startClock < durationMs) {
    trackPD(speed, Kp, Kd, Ki);
  }

  ao();
  resetPD();
}
void trackPDLeft_Time(float speed, float Kp, float Kd, unsigned long durationMs) {
  resetPD();
  unsigned long startClock = millis();

  while (millis() - startClock < durationMs) {
    trackPD_Left(speed, Kp, Kd);
  }

  ao();
  resetPD();
}

// ---------------------------------------------------------
// 4. RUN PID UNTIL AN INTERSECTION (CROSS LINE) IS FOUND
// ---------------------------------------------------------
void trackPD_Cross(float speed, float Kp, float Kd, float Ki = 0.0) {
  resetPD();
  
  while (!crossFound()) {
    trackPD(speed, Kp, Kd, Ki);
  }

  ao();
  resetPD();
}
void trackPD_Side(int sensorIdx, float speed, float Kp, float Kd, float Ki = 0.0) {
  resetPD();
  
  while (isBlack(sensorIdx)) {
    trackPD(speed, Kp, Kd, Ki);
  }

  ao();
  resetPD();
}