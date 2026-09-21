void track_CR(int speed3) {  //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed1
  // จะเดินตามเส้นไปจนกว่า analog(0) จะเจอเส้นดำ
  while (1) {
    if (analog(3) < Ref_RR) {  //  ถ้า analog(0) < Ref_LL คือ เจอดำ
      break;                   // ให้ ออกจาก loop
    }
    trackline(speed3);  // ฟังก์ชันนี้ จะทำให้หุ่นเดินเกาะเส้น แต่ลูปจะทำให้เดินเกาะเส้นไปเรื่่อยๆ
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void track_CL(int speed2) {  //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed1
  // จะเดินตามเส้นไปจนกว่า analog(0) จะเจอเส้นดำ
  while (1) {
    if (analog(0) < Ref_LL) {  //  ถ้า analog(0) < Ref_LL คือ เจอดำ
      break;                   // ให้ ออกจาก loop
    }
    trackline(speed2);  // ฟังก์ชันนี้ จะทำให้หุ่นเดินเกาะเส้น แต่ลูปจะทำให้เดินเกาะเส้นไปเรื่่อยๆ
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}


void R8(int sensor) {
  R = analog(sensor);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  while (R > Ref_R) {  // ถ้าค่า R>Ref_R คือ R เจอพื้นขาว จะทำ
    sr(30);
    R = analog(sensor);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  }
  ao();
  delay(200);  // stop motor
}
void fd_c(int sensor) {             // เดินหน้าจนกว่าจะเจอเส้น แล้ว หยุด
  while (1) {                       // ให้ทำคำสั่ง วนไปตลอดไม่หยุด
    if (analog(sensor) < Ref_LL) {  //  ถ้า analog(0) < 600 คือ เจอดำ
      break;                        // ให้ ออกจาก loop
    }
    fd2(20, 21);  // ถ้ายังไม่ออกจาก loop ก็จำทำงาน วนซ้ำ ไปตลอด
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void fd_c2(int sensor) {            // เดินหน้าจนกว่าจะเจอเส้น แล้ว หยุด
  while (1) {                       // ให้ทำคำสั่ง วนไปตลอดไม่หยุด
    if (analog(sensor) < Ref_LL) {  //  ถ้า analog(0) < 600 คือ เจอดำ
      break;                        // ให้ ออกจาก loop
    }
    fd2(20, 35);  // ถ้ายังไม่ออกจาก loop ก็จำทำงาน วนซ้ำ ไปตลอด
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void fd_c3(int sensor) {            // เดินหน้าจนกว่าจะเจอเส้น แล้ว หยุด
  while (1) {                       // ให้ทำคำสั่ง วนไปตลอดไม่หยุด
    if (analog(sensor) < Ref_LL) {  //  ถ้า analog(0) < 600 คือ เจอดำ
      break;                        // ให้ ออกจาก loop
    }
    fd2(35, 25);  // ถ้ายังไม่ออกจาก loop ก็จำทำงาน วนซ้ำ ไปตลอด
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void trackline(int speed) {             //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed
  L = analog(1);                        // เก็บค่าที่อ่านได้จาก analog(2) * ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  R = analog(2);                        // เก็บค่าที่อ่านได้จาก analog(3) * ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  if (L > Ref_L && R > Ref_R) {         // L และ R เจอขาว
    fd2(speed, speed + 1);              // ให้ตรงไป
  } else if (L < Ref_L && R > Ref_R) {  // L เจอดำ และ R เจอขาว
    sl(speed);                          // ให้หมุนซ้าย
  } else if (L > Ref_L && R < Ref_R) {  // L เจอขาว และ R เจอดำ
    sr(speed);                          // ให้หมุนขวา
  }
}
void track_side_line(int speed4, int sensor, int ref) {
  while (1) {
    if (analog(sensor) < ref) {
      break;
    } else {
      trackline(speed4);
    }
  }
}
void Skip_Side_Line(int count, int cross_duration, int line_speed, int sensor, int ref) {
  for (int i = 1; i <= count; i++) {
    track_side_line(line_speed, sensor, ref);
    cross(cross_duration);
    delay(100);
  }
}
void track_cross_PID1() {
  while (true) {
    if (analog(0) < Ref_LL && analog(3) < Ref_RR) {
      break;
    } else {
      trackline_PID(20, 2.5, 0.01, 0.75);
    }
  }
  ao();
}
void Skip_Cross_PID(int count) {
  for (int i = 1; i <= count; i++) {
    track_cross_PID1();
    cross(500);
  }
}
void Cross_PID(int count) {
  for (int i = 1; i <= count; i++) {
    cross(500);
    track_cross_PID1();
  }
}
void skip_close_cross(int count, int duration) {
  for (int i = 1; i <= count; i++) {
    close_cross_detect();
    cross(duration);
  }
}
void skip_Circle_Right_PID(int count) {
  for (int i = 1; i <= count; i++) {
    Circle_Right_PID();
    cross(100);
  }
}
void skip_Circle_Left_PID(int count) {
  for (int i = 1; i <= count; i++) {
    Circle_Left_PID();
    cross(100);
  }
}
void Circle_Right_PID() {
  while (true) {
    if (analog(3) < Ref_RR) {
      break;
    } else {
      trackline_PID(20, 12, 0.05, 8);
    }
  }
  ao();
}
void Circle_Left_PID() {
  while (true) {
    if (analog(0) < Ref_LL) {
      break;
    } else {
      trackline_PID(20, 12, 0.03, 6);
    }
  }
  ao();
}
void close_cross_detect() {
  while (true) {
    if (analog(1) < Ref_L && analog(2) < Ref_R) {
      break;
    } else {
      trackline(20);
    }
  }
}

void cross(int time) {
  fd2(20, 21);
  delay(time);
  ao();
}
void backward(int time) {
  bk2(21, 20);
  delay(time);
  ao();
}
void left_cross(int time){
  tl(20);
  delay(time);
  ao();
}
void right_cross(int time){
  tr(20);
  delay(time);
  ao();
}
void track_T(int speed1, int time1) {
  for (int i = 1; i <= time1; i++) {
    trackline(speed1);
  }
  ao();
  delay(1000);
}
void Skip_Square_Left(int count) {
  for (int i = 1; i <= count; i++) {
    Skip_Side_Line(1, 500, 20, 3, Ref_RR);
    R90(200, 2);
    Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  }
}
void Skip_Square_Right(int count) {
  for (int i = 1; i <= count; i++) {
    Skip_Side_Line(1, 525, 20, 0, Ref_LL);
    L90(200, 1);
    L90(1,1);
    Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  }
}