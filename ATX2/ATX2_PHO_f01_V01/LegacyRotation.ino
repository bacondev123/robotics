void L90(int turn, int sensor1) {
  sl(30);
  delay(turn);
  ao();
  int L = sensorValue[1];
  int Ref_L = sensorRef[1];
  L = analog(sensor1);
  while (L > Ref_L) {
    sl(30);
    L = analog(sensor1);
  }
  ao();
  delay(300);
}
void R90(int turn1, int sensor2) {
  sr(30);
  delay(turn1);
  ao();                 //ปรับเวลา delay ให้หมุนขวา ให้ analog(3) พ้นเส้นดำมาก่อน
  int R = sensorValue[2];
  int Ref_R = sensorRef[2];
  R = analog(sensor2);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  while (R > Ref_R) {   // ถ้าค่า R>Ref_R คือ R เจอพื้นขาว จะทำ
    sr(30);
    R = analog(sensor2);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  }
  ao();
  delay(300);  // stop motor
}

void spinLeft_Delay(int duration){
  sl(20);
  delay(duration);
  ao();
}
void spinRight_Delay(int duration){
  sr(20);
  delay(duration);
  ao();
}
void Skip_Junction_Right(int count) {
  for (int i = 1; i <= count; i++) {
    R90(0, 2);
    delay(50);
    R90(0, 1);
    delay(150);
  }
}
void Skip_Junction_Left(int count) {
  for (int i = 1; i <= count; i++) {
    L90(0, 1);
    delay(50);
    L90(0, 2);
    delay(150);
  }
}