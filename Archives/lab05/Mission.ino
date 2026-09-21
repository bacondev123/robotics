void Mission_1() {
  skip_close_cross(1, 500);
  delay(1000);
  L90(200, 1);
  Skip_Square_Left(1);
  L90(200, 1);
  track_T(20, 3750);
  R90(200, 2);
  skip_close_cross(1, 400);
  R90(200, 2);
  Skip_Square_Right(1);
  R90(200, 2);
  skip_close_cross(1, 500);
  R90(500, 2);
}
void Mission_2() {
  skip_close_cross(1, 500);
  delay(1000);
  R90(200, 2);
  Skip_Square_Right(1);
  R90(200, 2);
  track_T(20, 3250);
  L90(200, 1);
  skip_close_cross(1, 400);
  L90(200, 1);
  Skip_Square_Left(1);
  L90(200, 1);
  skip_close_cross(1, 500);
  L90(500, 1);
}
void Mission_3() {
  skip_close_cross(1, 500);
  L90(200, 1);
  Skip_Square_Left(1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(200, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  skip_close_cross(1, 500);
  L90(200, 1);
  Skip_Side_Line(1, 0, 20, 3, Ref_RR);
  R90(550, 2);
  skip_close_cross(1, 500);
  R90(200, 2);
  skip_close_cross(1, 500);
  return_base();
}
void Mission_4() {
  skip_close_cross(1, 500);
  R90(200, 2);
  Skip_Square_Right(1);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(200, 2);
  skip_close_cross(1, 500);
  R90(200, 2);
  Skip_Side_Line(1, 0, 20, 0, Ref_LL);
  L90(550, 1);
  skip_close_cross(1, 500);
  L90(200, 1);
  skip_close_cross(1, 500);
  return_base();
}
void Mission_5() {
  skip_start_right();
  delay(500);
  Skip_Cross_PID(2);
  L90(200, 1);
  L90(200, 2);
  track_T(20, 3500);
  L90(200, 1);
  skip_close_cross(1, 500);
  R90(200, 2);
  R90(150, 1);
  Skip_Cross_PID(2);
  return_base();
}
void Mission_6() {
  skip_start_right();
  delay(500);
  Skip_Cross_PID(2);
  R90(200, 2);
  R90(200, 1);
  track_T(20, 3500);
  R90(200, 2);
  skip_close_cross(1, 500);
  L90(200, 1);
  L90(200, 2);
  Skip_Cross_PID(2);
  return_base();
}
void Mission_7() {
  skip_start_right();
  Skip_Cross_PID(2);
  L90(200, 1);
  Skip_Side_Line(1, 0, 20, 3, Ref_RR);
  L90(200, 1);
  skip_close_cross(1, 650);
  delay(500);
  R90(100, 2);
  skip_close_cross(2, 500);
  return_base();
}
void Mission_8() {
  skip_start_right();
  Skip_Cross_PID(2);
  R90(200, 2);
  Skip_Side_Line(1, 0, 20, 0, Ref_LL);
  R90(200, 2);
  skip_close_cross(1, 650);
  delay(500);
  L90(100, 1);
  skip_close_cross(2, 500);
  return_base();
}
void Mission_9() {
  skip_start_right();
  Skip_Cross_PID(3);
  L90(200, 1);
  Skip_Center_Third_Left();
  cross(250);
  L90(100, 1);
  delay(500);
  Skip_Side_Line(1, 0, 20, 3, Ref_RR);
  L90(200, 1);
  skip_close_cross(1, 400);
  R90(200, 2);
  Skip_Center_Third_Return_Left();
  cross(250);
  R90(200, 2);
  delay(300);
  skip_close_cross(3, 500);
  delay(500);
  return_base();
}
void Mission_10() {
  skip_start_right();
  Skip_Cross_PID(3);
  R90(200, 2);
  Skip_Center_Third();
  cross(250);
  R90(200, 2);
  delay(500);
  Skip_Side_Line(1, 0, 20, 0, Ref_LL);
  R90(200, 2);
  skip_close_cross(1, 500);
  L90(200, 1);
  Skip_Center_Third_Return();
  cross(150);
  L90(200, 1);
  delay(300);
  skip_close_cross(3, 500);
  delay(500);
  return_base();
}
void Mission_11() {
  skip_start_right();
  Skip_Cross_PID(3);
  R90(200, 2);
  Skip_Center_Full();
  cross(150);
  delay(300);
  R90(200, 2);
  R90(1, 1);
  skip_close_cross(1, 500);
  L90(200, 1);
  delay(200);
  L90(200, 1);
  track_T(20, 3500);
  delay(3000);
  R90(200, 2);
  skip_close_cross(1, 550);
  R90(200, 2);
  R90(1, 1);
  skip_close_cross(1, 500);
  L90(200, 1);
  Skip_Center_Full_Return();
  cross(150);
  L90(200, 1);
  L90(1, 2);
  skip_close_cross(3, 500);
  return_base();
}
void Mission_12() {
  skip_start_right();
  Skip_Cross_PID(2);
  skip_close_cross(1, 575);
  R90(200, 2);
  Skip_Center_Full();
  cross(150);
  delay(300);
  R90(200, 2);
  R90(1, 1);
  skip_close_cross(1, 500);
  R90(200, 2);
  delay(200);
  R90(200, 2);
  track_T(20, 3500);
  delay(3000);
  L90(200, 1);
  skip_close_cross(1, 550);
  L90(200, 1);
  L90(1,2);
  skip_close_cross(1, 500);
  L90(200, 1);
  Skip_Center_Full_Return();
  cross(150);
  L90(200, 1);
  L90(1, 2);
  skip_close_cross(3, 500);
  return_base();
}
void Mission_13() {
  skip_start_right();
  Skip_Cross_PID(2);
  skip_close_cross(1, 575);
  R90(200, 2);
  Skip_Center_Full();
  cross(150);
  delay(300);
  R90(200, 2);
  R90(1, 1);
  skip_close_cross(1, 500);
  L90(200, 1);
  delay(200);
  track_T(20, 4500);
  delay(3000);
  L90(200, 1);
  skip_close_cross(1, 650);
  R90(200, 2);
  R90(1,1);
  skip_close_cross(1, 500);
  L90(200, 1);
  Skip_Center_Full_Return();
  cross(150);
  L90(200, 1);
  L90(1, 2);
  skip_close_cross(3, 500);
  return_base();
}
void Mission_14() {
  skip_start_right();
  Skip_Cross_PID(2);
  skip_close_cross(1, 575);
  R90(200, 2);
  Skip_Center_Full();
  cross(150);
  delay(300);
  R90(200, 2);
  R90(1, 1);
  skip_close_cross(1, 500);
  R90(200, 2);
  delay(200);
  track_T(20, 4500);
  delay(3000);
  R90(200, 2);
  skip_close_cross(1, 650);
  L90(200, 1);
  L90(1,2);
  skip_close_cross(1, 500);
  L90(200, 1);
  Skip_Center_Full_Return();
  cross(150);
  L90(200, 1);
  L90(1, 2);
  skip_close_cross(3, 500);
  return_base();
}
void Mission_15() {
  skip_start_right();
  Skip_Cross_PID(2);
  skip_close_cross(1, 575);
  R90(200, 2);
  Skip_Center_Full();
  cross(150);
  delay(300);
  R90(200, 2);
  R90(1, 1);
  skip_close_cross(1, 500);
  delay(200);
  track_T(20, 2000);
  delay(3000);
  R90(400, 2);
  skip_close_cross(2, 500);
  L90(200, 1);
  Skip_Center_Full_Return();
  cross(150);
  L90(200, 1);
  L90(1, 2);
  skip_close_cross(3, 500);
  return_base();
}
void Skip_Center_Third_Return_Left() {
  skip_Circle_Right_PID(1);
  cross(300);
  R90(200, 2);
  cross(500);
  skip_Circle_Right_PID(2);
}
void Skip_Center_Third_Return() {
  skip_Circle_Left_PID(1);
  cross(300);
  L90(200, 1);
  cross(350);
  skip_Circle_Left_PID(2);
}
void Skip_Center_Third() {
  skip_Circle_Right_PID(2);
  cross(300);
  R90(200, 2);
  cross(500);
  skip_Circle_Right_PID(1);
}
void Skip_Center_Third_Left() {
  skip_Circle_Left_PID(2);
  cross(300);
  L90(200, 1);
  cross(500);
  skip_Circle_Right_PID(1);
}
void Skip_Center_Full() {
  skip_Circle_Right_PID(2);
  cross(300);
  R90(200, 2);
  cross(500);
  skip_Circle_Right_PID(2);
}
void Skip_Center_Full_Return() {
  skip_Circle_Left_PID(2);
  cross(300);
  L90(200, 1);
  cross(500);
  skip_Circle_Left_PID(2);
}

void skip_start_right() {
  skip_close_cross(1, 500);
  R90(200, 2);
  Skip_Square_Right(1);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(200, 2);
}
void return_base() {
  R90(200, 2);
  Skip_Side_Line(1, 550, 20, 0, Ref_LL);
  L90(200, 1);
  Skip_Side_Line(1, 400, 20, 3, Ref_RR);
  Skip_Square_Right(1);
  R90(200, 2);
  skip_close_cross(1, 500);
  delay(500);
  R90(500, 2);
}