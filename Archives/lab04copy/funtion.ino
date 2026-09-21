void track_CR(int speed3){ //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed1
// จะเดินตามเส้นไปจนกว่า analog(0) จะเจอเส้นดำ
while(1){
    if (analog(3) < Ref_RR) {  //  ถ้า analog(0) < Ref_LL คือ เจอดำ
      break;                // ให้ ออกจาก loop
    }
    trackline(speed3); // ฟังก์ชันนี้ จะทำให้หุ่นเดินเกาะเส้น แต่ลูปจะทำให้เดินเกาะเส้นไปเรื่่อยๆ
   }
   ao();delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void track_CL(int speed2){ //ฟังก์ชั่นจะมีการส่งค่าพารามิเตอร์ ความเร็ว speed1
// จะเดินตามเส้นไปจนกว่า analog(0) จะเจอเส้นดำ
while(1){
    if (analog(0) < Ref_LL) {  //  ถ้า analog(0) < Ref_LL คือ เจอดำ
      break;                // ให้ ออกจาก loop
    }
    trackline(speed2); // ฟังก์ชันนี้ จะทำให้หุ่นเดินเกาะเส้น แต่ลูปจะทำให้เดินเกาะเส้นไปเรื่่อยๆ
   }
   ao();delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}


void R8(int sensor){
  R = analog(sensor);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  while(R>Ref_R){  // ถ้าค่า R>Ref_R คือ R เจอพื้นขาว จะทำ
    sr(30);
    R = analog(sensor);  // เก็บค่าที่อ่านได้จาก analog(3)* ถ้าไม่มี โปรแกรมจะไม่ทำงาน
  }
  ao();delay(200); // stop motor
}
void fd_c(int sensor) {               // เดินหน้าจนกว่าจะเจอเส้น แล้ว หยุด
  while (1) {               // ให้ทำคำสั่ง วนไปตลอดไม่หยุด
    if (analog(sensor) < Ref_LL) {  //  ถ้า analog(0) < 600 คือ เจอดำ
      break;                // ให้ ออกจาก loop
    }
    fd2(20,21);  // ถ้ายังไม่ออกจาก loop ก็จำทำงาน วนซ้ำ ไปตลอด
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void fd_c2(int sensor) {               // เดินหน้าจนกว่าจะเจอเส้น แล้ว หยุด
  while (1) {               // ให้ทำคำสั่ง วนไปตลอดไม่หยุด
    if (analog(sensor) < Ref_LL) {  //  ถ้า analog(0) < 600 คือ เจอดำ
      break;                // ให้ ออกจาก loop
    }
    fd2(20,35);  // ถ้ายังไม่ออกจาก loop ก็จำทำงาน วนซ้ำ ไปตลอด
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void fd_c3(int sensor) {               // เดินหน้าจนกว่าจะเจอเส้น แล้ว หยุด
  while (1) {               // ให้ทำคำสั่ง วนไปตลอดไม่หยุด
    if (analog(sensor) < Ref_LL) {  //  ถ้า analog(0) < 600 คือ เจอดำ
      break;                // ให้ ออกจาก loop
    }
    fd2(35,25);  // ถ้ายังไม่ออกจาก loop ก็จำทำงาน วนซ้ำ ไปตลอด
  }
  ao();
  delay(500);  // เมื่อ ออก loop มาแล้ว ก็ให้หยุดมอเตอร์
}
void track_side_line(int speed4, int sensor, int ref){
  while(1){
    if(analog(sensor) < ref){
      break;
    }else
    {trackline(speed4);}
  }
}
void Skip_Side_Line(int count, int cross_duration, int line_speed, int sensor, int ref){
  for(int i = 1; i <= count; i++){
    track_side_line(line_speed, sensor, ref);
    cross(cross_duration);
    delay(100);
  }
}
void track_cross_PID1(){
  while(true){
    if(analog(0)< Ref_LL && analog(3) < Ref_RR){
      break;
    }else {
    trackline_PID(20,2.5,0.01,0.75);
    }
  }
  ao();
}
void Skip_Cross_PID(int count){
  for(int i = 1; i <= count; i++){
    track_cross_PID1();
    cross(500);
  }
}
void Cross_PID(int count){
  for(int i = 1; i <= count; i++){
    cross(500);
    track_cross_PID1();
  }
}
void skip_close_cross(int count, int duration){
  for(int i = 1; i <= count; i++){
    close_cross_detect();
    cross(duration);
  }
}
void close_cross_detect(){
  while(true){
    if(analog(1)<Ref_L && analog(2)<Ref_R){
      break;
    }else
    {trackline(20);}
  }
}

void cross(int time){
  fd2(20,21);
  delay(time);
  ao();
}
void backward(int time){
  bk2(21,20);
  delay(time);
  ao();
}
void track_T(int speed1, int time1){
  for(int i = 1; i <= time1; i++){
    trackline(speed1);
  }
  ao();delay(1000);
}