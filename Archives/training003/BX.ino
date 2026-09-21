void b1() {
  // RunPath("T3");
  trackCross_PID_Offset(18, 0.5, 0.02, 0);
  // trackSideLine_PID(3, 10, 0.5, 0.02);
  // spinLeft_SensorLineSkip(30, 3, 1);
  // walkCross_StaticSpeed(300);
  // trackSideLine_PID(0, 10, 0.5, 0.02);
  // skipCross_PID_Static(1);
}
void b1_stone_age_hybrid() {
  
  // 1. PID forward until the far RIGHT sensor (S3) hits the black corner wall
  trackSideLine_PID_Static(3); 
  
  // 2. We hit the wall! Stop trying to PID. 
  // Let's "stone-age" hard-spin RIGHT until the center sensors lock on the line again
  spinRight_Sensor(20, 3); // Base 20 speed spin to guarantee no slipping!
  
  // 3. PID forward on that short vertical line until the far LEFT sensor (S0) hits
  trackSideLine_PID_Static(0); 

  // 4. We hit the wall! "Stone-age" hard-spin LEFT to lock on again
  spinLeft_Sensor(20, 0);
  
  // 5. You are now free! Drive smoothly away!
  // trackLine_PIDTime(20, 1000) or whatever is next...
}