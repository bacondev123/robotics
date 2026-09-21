void Experiment() {
  Lab_01();
  Lab_02();
  Lab_03();
  Lab_04();
}
void Lab_01() {
  skip_Side(1, 3);
  delay(1000);
  rotate_Right(2, 200);
  rotate_Right(1, 0);

  skip_Side(1, 3);
  delay(1000);
  rotate_Left(1, 400);
  rotate_Left(2, 0);

  skip_Side(1, 3);
  delay(1000);
  rotate_Left(1, 200);
  rotate_Left(2, 0);

  skip_Side(1, 0);
  delay(1000);
  rotate_Left(1, 200);
  rotate_Left(2, 0);
  // Lap 2
  delay(1500);

  skip_Side(1, 3);
  delay(1000);
  rotate_Left(1, 400);
  rotate_Left(2, 0);

  skip_Line(2);
  delay(1000);

  rotate_Left(1, 200);
  rotate_Left(2, 0);
}
void Lab_02() {
}
void Lab_03() {
}
void Lab_04() {
}
void Motor_tuning(float L, float R){
  fd2(L, R);
  delay(1000);
}
