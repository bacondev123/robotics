void servo_drop(){
  servo(1,170);
  delay(1000);
  servo(1,90);
  ao();
  delay(250);
}
void test_servo(){
  while(1){
  glcd(1,0, "Knob= %d ", knob(0, 180));  // แสดงค่า knob ในช่วง 0 - 180
  servo(1,knob(0, 180)); // servo หมุนไปที่ knob องศา
  }

}