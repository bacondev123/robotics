void trackLine(int speed) {
  readSensors();

  int L = sensorValue[1];
  int R = sensorValue[2];

  int Ref_L = sensorRef[1];
  int Ref_R = sensorRef[2];

  if (L > Ref_L && R > Ref_R) {
    fd2(speed, speed + 2);
  } 
  else if (L < Ref_L && R > Ref_R) {
    sl(speed);
  } 
  else if (L > Ref_L && R < Ref_R) {
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
void keep_down(){
  servo_drop();
}
void keep_up(){
  delay(3000);  
}