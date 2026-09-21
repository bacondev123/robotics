void Mission_1() {
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
  L90(200, 1);
  L90(200, 1);
  track_T(20, 4000);
  servo_drop();
  R90(200, 2);
  Skip_Side_Line(1, 0, 20, 0, Ref_LL);
  tr(30);
  delay(400);
  fd_c3(2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  Black();
}
void Mission_2() {
  track_CR(20);
  cross(525);
  delay(1000);
  R90(200, 2);
  R90(200, 2);
  track_T(20, 4000);  // เดินไปจุดปล่อย
  servo_drop();
  R90(200, 2);
  track_CR(20);
  tl(30);
  delay(400);
  fd_c2(1);
  track_CR(20);
  cross(525);
  L90(200, 1);
  track_CL(20);
  cross(600);
  R90(200, 2);
  track_CR(20);  // จดรับของ
  delay(5000);
  R90(500, 2);
  track_CL(20);
  cross(525);
  L90(300, 2);
  L90(1, 1);
  track_CR(20);
  cross(500);
  R90(100, 2);
  ao();
  delay(200);
}
void Mission_3() {
  Skip_Side_Line(2, 500, 20, 0, Ref_L);
  L90(200, 1);
  track_T(20, 3750);
  cross(500);
  servo_drop();  // ปล่อยของ
  R90(500, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
  R90(200, 2);
  Skip_Side_Line(2, 500, 20, 0, Ref_L);
  Black();

  // R90(200,2);
  // Skip_Side_Line(1,600,20, 3,Ref_RR);
  // L90(250,2);
  // Skip_Side_Line(1,1,20,0,Ref_LL);
  // // delay(5000);
  // L90(500,1);
  // Skip_Side_Line(1,500,20,3,Ref_RR);
  // R90(200,1);
  // R90(1,2);
  // Skip_Side_Line(1,500,20,0,Ref_LL);
  // L90(200,1);
}
void Mission_4() {
  Skip_Side_Line(2, 500, 20, 0, Ref_L);
  R90(200, 2);
  track_T(20, 3750);
  cross(500);
  servo_drop();
  R90(600, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  Skip_Side_Line(2, 500, 20, 0, Ref_LL);
  White();
}
void Mission_1_v2test() {
  Skip_Side_Line(2, 500, 20, 0, Ref_L);
  L90(250, 2);
  L90(1, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
  delay(2000);
  R90(500, 2);
  Skip_Side_Line(2, 500, 20, 0, Ref_L);
  L90(200, 2);
  L90(200, 2);
  track_T(20, 4500);
  servo_drop();  // box 1
  R90(500, 2);
  Skip_Cross_PID(1);
  R90(200, 1);
  R90(200, 1);
  Skip_Cross_PID(1);
  R90(200, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  delay(3000);
  L90(600, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(250, 1);
  R90(1, 2);
  Skip_Cross_PID(1);
  L90(200, 1);
  Skip_Cross_PID(1);
  R90(200, 2);
  R90(200, 2);
  Cross_PID(1);
  servo_drop();  // box 2
  R90(500, 2);
  Skip_Cross_PID(1);
  L90(200, 1);  // **
  L90(1, 1);
  Skip_Cross_PID(1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  delay(2000);
  R90(500, 2);
  Skip_Cross_PID(3);
  L90(200, 1);
  Cross_PID(1);  // box 3
  servo_drop();
  R90(500, 2);
  Skip_Cross_PID(1);
  R90(200, 2);
  Skip_Cross_PID(2);
  R90(200, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  delay(2000);
  L90(500, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(200, 1);
  R90(1, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(200, 1);
  Skip_Cross_PID(2);
  R90(200, 2);
  Cross_PID(1);
  servo_drop();  // box 4
  delay(300);
  L90(500, 2);
  Skip_Cross_PID(1);
  L90(200, 1);
  Skip_Cross_PID(2);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
  R90(200, 2);
  Skip_Cross_PID(1);


  ao();
}
