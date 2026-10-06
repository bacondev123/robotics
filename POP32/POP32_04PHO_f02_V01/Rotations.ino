int mid_L = sensorRef[0];
int mid_l = sensorRef[1];
int mid_r = sensorRef[2];
int mid_R = sensorRef[3];
void ssl(int speed){  // ***หมุนซ้ายด้วยความเร็ว speed 30 จนเจอเส้น
  sl(30);delay(300); //ให้หมุนไป 45 องศาก่อนเพื่อให้เซ็นเซอร์ a1 อยู่บนพื้นขาว
  while(analog(1)>mid_L){sl(speed);} // a1 เจอขาวทำ sl จนเจอดำ
  while(analog(3)>mid_l){sl(speed);} // a3 เจอขาวทำ sl จนเจอดำ
  while(analog(3)<mid_l){sl(speed);}	// a3 เจอดำทำ sl จนเจอขาว
  ao(); //stop
}
void ssr(int speed){  // ***หมุนซ้ายด้วยความเร็ว speed 30 จนเจอเส้น
  sr(30);delay(300); //ให้หมุนไป 45 องศาก่อนเพื่อให้เซ็นเซอร์ a1 อยู่บนพื้นขาว
  while(analog(8)>mid_L){sl(speed);} // a1 เจอขาวทำ sl จนเจอดำ
  while(analog(6)>mid_l){sl(speed);} // a3 เจอขาวทำ sl จนเจอดำ
  while(analog(6)<mid_l){sl(speed);}	// a3 เจอดำทำ sl จนเจอขาว
  ao(); //stop
}


void rotate_Left(int sensorPin, int blind_delay) { // No "= 0" here
  sl(30);
  delay(blind_delay);
  ao();
  readSensors();
  while(!isBlack(sensorPin)){
    readSensors();
    sl(30);
  }
  ao();
  delay(LOOP_DELAY_MS);
}
void rotate_Right(int sensorPin, int blind_delay) { // No "= 0" here
  sr(30);
  delay(blind_delay);
  ao();
  readSensors();
  while(!isBlack(sensorPin)){
    readSensors();
    sr(30);
  }
  ao();
  delay(LOOP_DELAY_MS);
}
void spinLeft(int speed, int sensorIdx) {
  int spinSpeed = speed;
  int creepSpeed = 15;
  
  sl(spinSpeed);
  delay(50); 

  while (1) {
    readSensors();
    if (isWhite(sensorIdx)) break; 
  }
  delay(TURN_DELAY);

  while (1) {
    readSensors();
    if (isBlack(sensorIdx)) break;
  }

  ao();
  delay(20);

  unsigned long alignTime = millis();
  unsigned long timeout = millis();
  
  while (millis() - timeout < 300) {
    readSensors();
    if (isWhite(sensorIdx)) {
      sr(creepSpeed); 
      alignTime = millis();  
    } else {
      ao();
      if (millis() - alignTime >= 50) break;
    }
  }
  
  ao();
  resetPD();
}

void spinRight(int speed, int sensorIdx) {
  int spinSpeed = speed;
  int creepSpeed = 15;
  
  sr(spinSpeed);
  delay(50); 

  while (1) {
    readSensors();
    if (isWhite(sensorIdx)) break; 
  }
  delay(TURN_DELAY);

  while (1) {
    readSensors();
    if (isBlack(sensorIdx)) break;
  }

  ao();
  delay(20);

  unsigned long alignTime = millis();
  unsigned long timeout = millis();
  
  while (millis() - timeout < 300) {
    readSensors();
    if (isWhite(sensorIdx)) {
      sl(creepSpeed);
      alignTime = millis();  
    } else {
      ao();
      if (millis() - alignTime >= 50) break; 
    }
  }

  ao();
  resetPD();
}

void spinLeft_SkipLine(int speed, int sensorIdx, int count) {
  int spinSpeed = speed;
  int creepSpeed = 15;
  
  sl(spinSpeed);
  delay(50); 

  for (int i = 1; i <= count; i++) {
    while (1) {
      readSensors();
      if (isWhite(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);

    while (1) {
      readSensors();
      if (isBlack(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);
  }

  ao();      
  delay(20); 
}

void spinRight_SkipLine(int speed, int sensorIdx, int count) {
  int spinSpeed = speed;
  int creepSpeed = 15;
  
  sr(spinSpeed);
  delay(50); 

  for (int i = 1; i <= count; i++) {
    while (1) {
      readSensors();
      if (isWhite(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);

    while (1) {
      readSensors();
      if (isBlack(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);
  }

  ao();
  delay(20);
}
void spinLeft_SkipLine_Enhanced(int speed, int sensorIdx, int count) {
  int spinSpeed = speed;
  int creepSpeed = 15;
  
  sl(spinSpeed);
  delay(50); 

  for (int i = 1; i <= count; i++) {
    while (1) {
      readSensors();
      if (isWhite(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);

    while (1) {
      readSensors();
      if (isBlack(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);
  }

  ao();
  delay(20);

  unsigned long alignTime = millis();
  unsigned long timeout = millis();

  while (millis() - timeout < 300) {
    readSensors();
    if (isWhite(sensorIdx)) {
      sr(creepSpeed);
      alignTime = millis(); 
    } else {
      ao();
      if (millis() - alignTime >= 50) break;
    }
  }

  ao();
  resetPD();
}
void spinRight_SkipLine_Enhanced(int speed, int sensorIdx, int count) {
  int spinSpeed = speed;
  int creepSpeed = 15;
  
  sr(spinSpeed);
  delay(50); 

  for (int i = 1; i <= count; i++) {
    while (1) {
      readSensors();
      if (isWhite(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);

    while (1) {
      readSensors();
      if (isBlack(sensorIdx)) break;
    }
    if (i < count) delay(TURN_DELAY);
  }

  ao();
  delay(20);

  unsigned long alignTime = millis();
  unsigned long timeout = millis();

  while (millis() - timeout < 300) {
    readSensors();
    if (isWhite(sensorIdx)) {
      sl(creepSpeed);
      alignTime = millis(); 
    } else {
      ao();
      if (millis() - alignTime >= 50) break;
    }
  }

  ao();
  resetPD();
}
void spinLeft_Delay(int duration){
  sl(30);
  delay(duration);
  ao();
}
void spinRight_Delay(int duration){
  sr(30);
  delay(duration);
  ao();
}