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
  servo(1, 90);  // หมุนไปที่ 90 องศา
  OK();
  beep();
  delay(1000);
  glcdMode(2);
  glcdClear();

  // skip_close_cross(1, 500); //start
  // delay(500);
  // Mission_1();
  // Mission_2();
  // Mission_3();
  // Mission_4();
  // Mission_5();
  // Mission_6();
  // Mission_7();
  // Mission_8();
  // Mission_9();
  // Mission_10();
  // Mission_11();
  // Mission_12();
  // Mission_13();
  // Mission_14();
  Mission_15();
  // skip_close_cross(1,500); // PID skip center test
  // R90(75,2);
  // Skip_Center_Full();
}

void loop() {
}
