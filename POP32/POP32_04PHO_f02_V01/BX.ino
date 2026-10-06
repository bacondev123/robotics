void b1_legacy() {
  fowardDelay(925);
  delay(300);
  spinLeft_SkipLine(30, 1, 1);
  // waitSW_OK();
  spinLeft_SkipLine(30, 2, 1);
  delay(300);
  fowardDelay(1000);
  delay(300);
  spinRight_SkipLine(30, 2, 1);
  delay(300);
  fowardDelay(800);
  delay(300);
  spinRight_SkipLine(30, 1, 1);
  track_jc(20);


  fowardDelay(1000);
  delay(300);
  spinLeft_Delay(800);
  waitSW_OK();

  delay(300);
  fowardDelay(950);
  waitSW_OK();
  spinLeft_Delay(200);
  spinLeft_SkipLine(30, 1, 0);
  delay(300);
  fowardDelay(900);
  delay(300);
  waitSW_OK();
  spinRight_SkipLine(30, 1, 1);

  delay(300);
  skip_Line(1);
}
void b1() {
  trackPD_Cross(16, 12, 2.75);
}
void b2() {
  trackPD_Cross(18, 13, 2.5);
}
void b3() {
  trackPD_Cross(20, 8, 3);
}
void b4() {
}
void b512() {
  trackPD_Time(16, 6, 2, 1500);
  waitSW_OK();
  fowardCross();
  delay(1000);
  spinLeft_SkipLine(30, 2, 1);
  // trackPDLeft_Time(16,8,2,100);
}
void b532() {
  trackPD_Time(16, 6, 2, 1500);
  waitSW_OK();
  fowardCross();
  delay(1000);
  spinRight_SkipLine(30, 2, 1);
}