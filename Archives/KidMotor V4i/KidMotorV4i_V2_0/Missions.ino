void Operation(){

}

void Mission_1(){
  fd2(20,20);
  delay(1000);
  ao();
}

void runExperiment() {
  missionStoneAge();
}

void missionStoneAge() {
  trackSideLine(3);
  delay(300);
  sl(20); delay(300); ao(); // Replaced spinLeft_Delay 
  delay(300);
  fd2(15, 15);
  delay(300);
  spinRightSkipLines(30, 2, 1);
  delay(300);
  driveForwardTime(450);
  delay(300);
  spinRightSkipLines(30, 2, 1);
  delay(300);
  trackSideLine(0);
  delay(100);
  fd2(15, 15);
  delay(300);
  spinLeftSkipLines(30, 1, 1);
  fd2(15, 15);
  delay(300);
  spinLeftSkipLines(30, 2, 1);
  delay(300);
  driveForwardTime(450);
  delay(300);
  spinRightSkipLines(30, 2, 1);
}