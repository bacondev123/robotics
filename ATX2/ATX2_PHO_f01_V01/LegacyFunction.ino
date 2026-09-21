void trackLine(int speed) {
  readSensors();

  int L = sensorValue[1];
  int R = sensorValue[2];

  int Ref_L = sensorRef[1];
  int Ref_R = sensorRef[2];

  if (L > Ref_L && R > Ref_R) {
    fd2(speed, speed + 2);
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
  delay(1000);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void keep_down() {
  servo_drop();
}
void keep_up() {
  delay(3000);
}
void fowardLineStop(int speed) {
  while (1) {
    readSensors();
    bool cross = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);
    if (cross) break;
    else
      fd2(compensateSpeed(speed), compensateSpeed(speed + 2));
  }
  ao();
  delay(LOOP_DELAY_MS);
}

// void driveLineFollowerSwapper(int speed) {
// #if USE_PID
//   driveLineFollowerPID(speed, 3.0, 0.5, 0.02);
// #else
//   trackLine(speed); // Legacy reference assuming macro trackLine remains natively. 
// #endif
// }

// void traverseCrossSwapper(int speed) {
//   int hitConfirm = 0; // Debounced crossing safely integrated here too
//   while (1) {
//     readSensors();
//     bool crossFound = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);
//     if (crossFound) {
//       hitConfirm++;
//       if (hitConfirm >= 2) break;
//     } else {
//       hitConfirm = 0;
//       driveLineFollowerSwapper(speed);
//     }
//   }
//   ao();
// }