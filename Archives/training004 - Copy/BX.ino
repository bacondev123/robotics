void b2() {
  // RunPath("T3");
  trackCross_PID_Offset(18, 0.5, 0.02, 0);
  // trackSideLine_PID(3, 10, 0.5, 0.02);
  // spinLeft_SensorLineSkip(30, 3, 1);
  // walkCross_StaticSpeed(300);
  // trackSideLine_PID(0, 10, 0.5, 0.02);
  // skipCross_PID_Static(1);
}
void b1_stone_age_hybrid() {

  trackSideLine_PID_Static(3);
  delay(300);
  spinLeft_Delay(300);  // Base 20 speed spin to guarantee no slipping!
  delay(300);
  fowardLineStop(15);
  // trackSideLine_PID_Static(0);
  delay(300);
  spinRight_SensorLineSkip(30, 2, 1);
  delay(300);
  walkCross_StaticSpeed(450);
  delay(300);
  spinRight_SensorLineSkip(30, 2, 1);
  delay(300);
  // skipSideLine_PID(1, 0);
  trackSideLine_PID_Static(0);
  delay(100);
  fowardLineStop(15);
  delay(300);
  spinLeft_SensorLineSkip(30, 1, 1);
  fowardLineStop(15);
  delay(300);
  spinLeft_SensorLineSkip(30, 2, 1);
  delay(300);
  walkCross_StaticSpeed(450);
  delay(300);
  spinRight_SensorLineSkip(30, 2, 1);

  // spinLeft_Delay(100);
  // spinLeft_Sensor(20, 0);
}
void b3(){
  RunPath("T");
}
void b4(const char* variations){
  if(variations == "R") RunPath("w 1R2 T w 1R1 W");
  else if(variations == "2R") RunPath("w 1R2 T w 1L2 T w 1R1 W");
  else if(variations == "C2R") RunPath("w 1R2 T w 1R2 T3 w 1R1 W");
  else if(variations == "2C2R") RunPath("w 1R2 T w 1R2 T3 w 1L1 T3 w 1R2 T W");
  else if(variations == "3C2R") RunPath("w 1R2 T3 w 1L2 T3 w 1L1 T3 w 1R2 T W");
}
void b5(const char* variations){
  if(variations == "R") RunPath("w 1R2 T w 1R1 W");
  else if(variations == "2R") RunPath("w 1R2 T w 1L2 T3 W 1R1");
  if(variations == "L") RunPath("w 1L1 T w 1L2 W");
  else if(variations == "2L") RunPath("w 1L1 T w 1R1 T0 W 1L2");
}
void b6(const char* variations){
  if(variations == "L") RunPath("1L2 T0 w 1L1 T W");
  else if (variations == "2L") RunPath("1L2 T0 w 1R2 T0 w 1L1 T W");
  else if (variations == "3L") RunPath("1L2 T0 w 1R2 T0 200W T0 w 1L1 T W");
  else if (variations == "4L") RunPath("1L2 T0 w 1R2 T0 200W T0 200W T0 1L1 T W");
}
void b7(const char* variations){
  if(variations == "L") RunPath("T W 1L1 T W");
  else if(variations == "2L") RunPath("T W 1L1 W 1R2 T3 W 1L2 T3 W");
  else if(variations == "3L") RunPath("T W 1L1 W 1R2 T3 W 1R1 T3 W 1L2 T W");
  else if(variations == "4L") RunPath("T W 1L1 W 1R2 T3 W 1R1 T3 W 1R1 T3 W 1L2 T W");
  if(variations == "R") RunPath("T W 1R2 T W");
  else if(variations == "2R") RunPath("T W 1R2 W 1L1 T0 W 1R1 T W");
  else if(variations == "3R") RunPath("T W 1R2 W 1L1 T0 W 1L2 T0 W 1R1 T W");
  else if(variations == "4R") RunPath("T W 1R2 W 1L1 T0 W 1L2 T0 W 1R1 T0 W 1R1 T W");
}