void start() {
  Skip_Side_Line(2, 500, 20, 0, Ref_L);
  L90(250, 2);
  L90(1, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
  delay(2000);
  R90(500, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_L);
}
void Mission_1_v2() {
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(200, 2);
  L90(200, 2);
  Cross_PID(1);
  servo_drop();
  R90(500, 2);
  Skip_Side_Line(1, 650, 20, 0, Ref_L);
  R90(200, 2);
  return_base_right();
}
void Mission_2_v2() {
  R90(200, 2);
  Skip_Side_Line(1, 650, 20, 3, Ref_RR);
  R90(200, 2);
  Cross_PID(1);
  servo_drop();
  R90(500, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  L90(1, 2);
  return_base_middle();
}
void Mission_3_v2() {
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(200, 2);
  skip_close_cross(1, 100);
  servo_drop();
  L90(500, 1);
  Skip_Side_Line(1, 650, 20, 3, Ref_RR);
  R90(100, 2);
  return_base_right();
}
void Mission_4_v2() {
  R90(200, 2);
  skip_close_cross(1, 500);
  skip_close_cross(1, 50);
  servo_drop();
  R90(500, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(200, 1);
  return_start();
}
void return_start() {
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(125, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
}
void return_base_middle() {
  delay(1000);
  Skip_Side_Line(2, 500, 20, 3, Ref_RR);
  R90(500, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
}
void return_base_right() {
  delay(1000);
  R90(200, 2);
  Skip_Side_Line(1, 600, 20, 3, Ref_RR);
  L90(75, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  delay(2000);
  L90(500, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
}
void Mission_5_v2() {
  R90(200, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  skip_close_cross(1, 500);
  L90(200, 1);
  Cross_PID(1);
  servo_drop();
  R90(500, 2);
  skip_close_cross(1, 600);
  R90(500, 2);
  delay(1000);
  return_base_right_v2();
}
void Mission_6_v2() {
  R90(200,2);
  skip_close_cross(1,500);
  R90(200,2);
  Cross_PID(1);
  servo_drop();
  R90(500,2);
  skip_close_cross(1,500);
  L90(500,1);
  delay(1000);
  return_base_middle_v2();
}
void Mission_7_v2() {
  R90(200, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  Skip_Side_Line(1, 650, 20, 3, Ref_RR);
  skip_close_cross(1,100);
  servo_drop();
  L90(500, 1);
  Skip_Side_Line(1, 750, 20, 3, Ref_RR);
  R90(500,2);
  delay(1000);
  return_base_right_v2();
}
void Mission_8_v2() {
  R90(200,2);
  skip_close_cross(1,500);
  skip_close_cross(1,100);
  servo_drop();
  R90(500,2);
  skip_close_cross(1,500);
  L90(500,1);
  return_start();
  // return_base_middle_v2();
}
void return_base_right_v2() {
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  delay(3000);
  L90(500, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
}
void return_base_middle_v2() {
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(500, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(200,1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  delay(3000);
  R90(500, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
}