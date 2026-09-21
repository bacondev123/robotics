#include <ATX2.h>
int Ref_LL = 593;
int Ref_L = 573;
int Ref_C = 600;
int Ref_R = 541;
int Ref_RR = 528;
int LL = analog(0);
int L = analog(1);
int C = analog(2);
int R = analog(3);
int RR = analog(4);
void setup() {
  XIO();
  servo(1, 90);  // หมุนไปที่ 90 องศา
  OK();
  beep();
  delay(1000);
  glcdMode(2);
  glcdClear();

  test_1();
}
void loop(){
  // trackline_PID(20,8,0.02,3);
}