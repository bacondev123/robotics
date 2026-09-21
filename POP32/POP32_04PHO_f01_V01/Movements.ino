void trackLine(int speed) {
  readSensors();

  int L = sensorValue[1];
  int R = sensorValue[2];

  int Ref_L = sensorRef[1];
  int Ref_R = sensorRef[2];

  if (L > Ref_L && R > Ref_R) {
    fd2(speed, speed + MOTOR_BIAS);
  } else if (L < Ref_L && R > Ref_R) {
    sl(speed);
  } else if (L > Ref_L && R < Ref_R) {
    sr(speed);
  }
}
void track_CL(int speed1) {  //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed1
  // จะเดินตามเส้นไปจนกว่า analog(0) จะเจอเส้นดำ

  while (1) {
    readSensors();
    if (analog(0) < sensorRef[0]) {  //  ถ้า analog(0) < Ref_LL คือ เจอดำ
      break;                         // ให้ ออกจาก loop
    }
    trackLine(speed1);  // ฟังก์ชันนี้ จะทำให้หุ่นเดินเกาะเส้น แต่ลูปจะทำให้เดินเกาะเส้นไปเรื่่อยๆ
  }
  ao();
  delay(LOOP_DELAY_MS);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void track_CR(int speed1) {  //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed1
  // จะเดินตามเส้นไปจนกว่า analog(0) จะเจอเส้นดำ

  while (1) {
    readSensors();
    if (analog(3) < sensorRef[3]) {  //  ถ้า analog(0) < Ref_LL คือ เจอดำ
      break;                         // ให้ ออกจาก loop
    }
    trackLine(speed1);  // ฟังก์ชันนี้ จะทำให้หุ่นเดินเกาะเส้น แต่ลูปจะทำให้เดินเกาะเส้นไปเรื่่อยๆ
  }
  ao();
  delay(LOOP_DELAY_MS);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}

void track_Side(int sensorIdx) {
  // Follow the line until the specified side sensor detects black or intersection
  readSensors();
  while (isWhite(sensorIdx)) {
    readSensors();
    trackLine(DEFAULT_SPEED);
  }
  ao();
}

void skip_Side(int count, int sensorIdx) {
  // Skip a specified number of side intersections
  for (int i = 1; i <= count; i++) {
    track_Side(sensorIdx);
    fowardCross();
    ao();
  }
}

void track_Cross(int speed) {
  // Follow the line until a cross intersection is detected
  readSensors();
  while (!crossFound()) {
    readSensors();
    trackLine(speed);
  }
  ao();
  delay(LOOP_DELAY_MS);
}

void skip_Line(int count) {
  // Skip a specified number of cross intersections
  for (int i = 1; i <= count; i++) {
    track_Cross(DEFAULT_SPEED);
    fowardCross();
    ao();
  }
  ao();
}

void fowardCross() {
  // Move forward slightly to push the robot past an intersection
  fd2(DEFAULT_SPEED, DEFAULT_SPEED + MOTOR_BIAS);
  delay(DEFAULT_LINE_SKIP);
  ao();
}
void fowardDelay(int delayMS) {
  // Move forward for a custom amount of time (in milliseconds)
  fd2(DEFAULT_SPEED, DEFAULT_SPEED + MOTOR_BIAS);
  delay(delayMS);
  ao();
}
/*
void keep_down() {
  servo_drop();
}
void keep_up() {
  delay(3000);
}
*/
void fowardLineStop(int speed) {
  while (1) {
    readSensors();
    bool cross = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);
    if (cross) break;
    else
      fd2(speed + MOTOR_BIAS, speed);
  }
  ao();
  delay(LOOP_DELAY_MS);
}