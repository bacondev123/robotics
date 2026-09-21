void L90(int turn, int sensor1){
  sl(30);
  delay(turn);
  ao();
  L = analog(sensor1);
  while(L>Ref_L){
    sl(30);
    L = analog(sensor1);
  }
  ao();delay(200);
}
void R90(int turn1, int sensor2){
  sr(30);delay(turn1);ao(); //ปรับเวลา delay ให้หมุนขวา ให้ analog(3) พ้นเส้นดำมาก่อน
  R = analog(sensor2);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  while(R>Ref_R){  // ถ้าค่า R>Ref_R คือ R เจอพื้นขาว จะทำ
    sr(30);
    R = analog(sensor2);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  }
  ao();delay(200); // stop motor
}