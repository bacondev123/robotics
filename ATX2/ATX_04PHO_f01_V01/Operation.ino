void Operation() {
  // Start();
  // Mission_1();
  // Mission_2();
  // Mission_3();
  // Mission_4();
  Mission_1_V2();
  Mission_2_V2();
  Mission_3_V2();
  Mission_4_V2();
}
void Mission_1() {
  skip_Line(2);
  delay(1000);
  rotate_Left(0, 300);
  rotate_Left(1, 0);
  delay(500);
  track_Cross(DEFAULT_SPEED);
  servo_drop();

  rotate_Left(1, 400);
  skip_Line(1);
  fowardDelay(50);
  delay(1000);
  rotate_Right(3, 200);
  delay(1000);
  rotate_Right(2, 0);
  skip_Line(2);
  keep_up();
}

void Mission_2() {
  rotate_Right(2, 500);
  delay(500);

  skip_Line(2);
  delay(1000);
  rotate_Right(3, 300);
  rotate_Right(2, 0);
  delay(500);
  track_Cross(DEFAULT_SPEED);
  servo_drop();

  rotate_Right(2, 400);
  skip_Line(1);
  fowardDelay(50);
  delay(1000);
  rotate_Left(0, 200);
  delay(1000);
  rotate_Left(1, 0);
  skip_Line(2);
  keep_up();
}

void Mission_3() {
  rotate_Right(2, 500);
  delay(500);

  skip_Line(3);
  delay(1000);
  rotate_Left(1, 200);
  track_Side(0);
  fowardDelay(100);
  servo_drop();

  rotate_Right(2, 400);
  skip_Side(1, 3);
  rotate_Right(2, 200);
  skip_Line(3);
  keep_up();
}

void Mission_4() {
  rotate_Right(2, 500);
  delay(500);

  skip_Line(3);
  delay(1000);
  rotate_Right(2, 200);
  track_Side(3);
  fowardDelay(100);
  servo_drop();

  rotate_Left(1, 400);
  skip_Side(1, 0);
  rotate_Left(1, 200);
  skip_Line(3);
  keep_up();
}

void Mission_1_V2() {
  skip_Side(1, 3);
  delay(500);
  rotate_Right(2, 300);
  skip_Line(1);
  delay(500);
  rotate_Left(1, 200);
  skip_Line(1);
  delay(1000);
  rotate_Left(0, 300);
  rotate_Left(1, 0);
  track_Cross(DEFAULT_SPEED);
  servo_drop();

  rotate_Left(1, 400);
  skip_Line(1);
  fowardDelay(50);
  delay(1000);
  rotate_Right(3, 200);
  delay(1000);
  rotate_Right(2, 0);
  skip_Line(1);
  rotate_Left(1, 200);
  skip_Side(1, 0);
  rotate_Right(2, 200);
  skip_Line(1);
  keep_up();
}

void Mission_2_V2() {
  rotate_Right(1,400);

  skip_Side(1, 0);
  delay(500);
  rotate_Left(1, 300);
  skip_Line(1);
  delay(500);
  rotate_Right(2, 200);
  skip_Line(1);
  delay(1000);

  rotate_Right(3, 300);
  delay(500);
  rotate_Right(2, 0);
  track_Cross(DEFAULT_SPEED);
  servo_drop();

  rotate_Left(1, 400);
  skip_Line(1);
  fowardDelay(50);
  delay(1000);
  rotate_Left(0, 200);
  delay(1000);
  rotate_Left(1, 0);
  skip_Line(1);
  rotate_Left(1, 200);
  skip_Side(1, 0);
  rotate_Right(2, 200);
  skip_Line(1);
  keep_up();
}

void Mission_3_V2() {
}

void Mission_4_V2() {
}
void returnBlack() {
  rotate_Right(2, 200);
  skip_Side(1, 3);
  fowardDelay(50);
  delay(500);
  rotate_Left(1, 200);
  skip_Line(1);
}
void returnWhite() {
  rotate_Left(1, 200);
  skip_Side(1, 0);
  fowardDelay(50);
  delay(500);
  rotate_Right(2, 200);
  skip_Line(1);
}
void Start() {
  skip_Line(1);
}