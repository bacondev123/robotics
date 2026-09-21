#include <ATX2.h>
int Ref_LL = 593;
int Ref_L = 573;
int Ref_R = 541;
int Ref_RR = 528;
int LL = analog(0);
int L = analog(1);
int R = analog(2);
int RR = analog(3);
void setup() {
  XIO();
  servo(1,90); // หมุนไปที่ 90 องศา

  OK();
  beep();
  delay(1000);
  glcdMode(2);
  glcdClear();
  start();
  Mission_5_v2();
  Mission_6_v2();
  Mission_7_v2();
  Mission_8_v2();
  // Mission_1_v2();
  // Mission_2_v2();
  // Mission_3_v2();
  // Mission_4_v2();
  // test_servo();
  // cross(250);
  // track_CR(20);
  // cross(800);
  // Mission_1();
  // Mission_2();
  // Mission_3();
  // Mission_4();
  // Mission_1_v2test();
  
}


void loop() {

}

void trackline(int speed) { //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed
  L = analog(1);  // เก็บค่าที่อ่านได้จาก analog(2) * ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  R = analog(2);  // เก็บค่าที่อ่านได้จาก analog(3) * ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  if (L > Ref_L && R > Ref_R) {         // L และ R เจอขาว
    fd2(speed, speed+1);                        // ให้ตรงไป
  } else if (L < Ref_L && R > Ref_R) {  // L เจอดำ และ R เจอขาว
    sl(speed);                          // ให้หมุนซ้าย
  } else if (L > Ref_L && R < Ref_R) {  // L เจอขาว และ R เจอดำ
    sr(speed);                          // ให้หมุนขวา
  }
}

