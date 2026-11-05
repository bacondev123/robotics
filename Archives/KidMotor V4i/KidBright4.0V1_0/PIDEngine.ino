void resetPID() {
  I = 0;
  lineError = 0;
  previousError = 0;
  currentBaseSpeed = 15.0;
}

float calculatePID(float Kp, float Ki, float Kd, float dt) {
  float P = lineError;

  if (abs(lineError) <= 1) { 
    I += lineError * dt;
    I = constrain(I, -100, 100);
  } else {
    I = 0;
  }

  float D_raw = (lineError - previousError) / dt;
  float D = constrain(D_raw, -150.0, 150.0);
  return (Kp * P) + (Ki * I) + (Kd * D);
}

float readLinePosition() { 
  long sumPosition = 0;
  long totalWeight = 0;

  for (int i = 0; i < Config::NUM_SENSORS; i++) {
    long lineStrength = sensorRef[i] - sensorValue[i];
    if (lineStrength > 0) {
      sumPosition += (lineStrength * (i * 1000L));
      totalWeight += lineStrength;
    }
  }

  float calculatedPosition = Config::CENTER_POS;  

  if (totalWeight <= 0) {
    if (lastPosition >= (Config::CENTER_POS - 250) && lastPosition <= (Config::CENTER_POS + 250)) {
      calculatedPosition = Config::CENTER_POS;
    } else if (lastPosition < (Config::CENTER_POS - 250)) {
      calculatedPosition = Config::MIN_POS;
    } else {
      calculatedPosition = Config::MAX_POS;
    }
    lastPosition = calculatedPosition; 
  } else {
    calculatedPosition = (float)sumPosition / (float)totalWeight;
    lastPosition = (lastPosition * 0.6) + (calculatedPosition * 0.4);
  }
  return lastPosition;
}

void trackLinePID(float maxSpeed, float Kp, float Ki, float Kd) {
  static unsigned long lastLoopTime = micros(); 
  unsigned long currentTime = micros();

  if(currentTime - lastLoopTime > 1000000){ lastLoopTime = currentTime; return; }

  if (currentTime - lastLoopTime >= 5000) {
    float dt = (currentTime - lastLoopTime) / 1000000.0;
    if (dt <= 0) dt = 0.001;
    if (dt > 0.05) dt = 0.005;

    lastLoopTime = currentTime;

    readSensors();
    float position = readLinePosition();  

    lineError = (position - Config::CENTER_POS) / 500.0;
    float deltaError = abs(lineError - previousError);

    float curveSeverity = (abs(lineError) * 7.0) + (deltaError * 10.0);
    float targetSpeed = maxSpeed - curveSeverity;
    float currentMinFloor = Config::MIN_CORNER_SPEED;

    if (abs(lineError) >= 2) currentMinFloor = -25.0f;
    else if (abs(lineError) >= 1) currentMinFloor = -12.5f;

    targetSpeed = constrain(targetSpeed, currentMinFloor, maxSpeed);
    
    if (currentBaseSpeed < targetSpeed) {
      currentBaseSpeed += 0.4;
      if (currentBaseSpeed > targetSpeed) currentBaseSpeed = targetSpeed;
    } else if (currentBaseSpeed > targetSpeed) {
      currentBaseSpeed -= 2.0;  
      if (currentBaseSpeed < targetSpeed) currentBaseSpeed = targetSpeed;
    }

    float pidValue = calculatePID(Kp, Ki, Kd, dt);

    float finalLeft = compensateSpeed(currentBaseSpeed + pidValue);
    float finalRight = compensateSpeed(currentBaseSpeed - pidValue);

    fd2(round(finalLeft), round(finalRight));
    previousError = lineError;
  }
}