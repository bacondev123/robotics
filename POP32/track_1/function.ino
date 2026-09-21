void check_line(){  // เดินหน้าจนเจอเส้นดำ แล้วหยุด
  while(1){
    if (analog(1) < mid_L) { // ถ้าเซ็นเซอร์ตัวซ้ายสุดเจอดำ
      break; // ถ้าเซ็นเซอร์ตัวซ้ายสุด  เจอดำ จะออก loop
    }
    fd2(20,20); // ถ้าไม่เจอ เดินหน้า
  }
}
void track_line(int speed){   //เกาะเส้นทดสอบ
  while(1){track(speed);} // ให้เดินเกาะเส้น ไปเรื่อย ๆ ไม่มีจบ(ไม่หยุด)
}
void track(int speed){ // จะรับค่า speed มาด้วย * ความเร็วทดสอบให้ใช้ 20
// จะเป็นการทำงานให้เกาะเส้น
  if (analog(3) > mid_l && analog(6) > mid_r) { //** > เจอขาว < เจอดำ
    fd(speed);// เซ็นเซอร์ซ้ายและขวาเจอพื้นขาวให้ ตรงไป
    } else if (analog(3) < mid_l && analog(6) > mid_r) {
    tl(speed);// เซ็นเซอร์ซ้ายเจอดำให้ เลี้ยวซ้าย
    } else if (analog(3) > mid_l && analog(6) < mid_r) {
    tr(speed);// เซ็นเซอร์ขวาเจอดำให้ เลี้ยวขวา
    } else fd(speed);  // นอกนั้นให้ตรงไป  *** ต้องมีไม่งั้นเดินเพี๊ยน
}
void track_jc(int speed){  // เกาะเส้น เจอทางแยก หยุด
  while(1){ // จะ loop ไปตลอด แล้วใช้
    track(speed);// ให้เกาะเส้นไปเรี่อย ๆ
    if(analog(1) < mid_L || analog(8) < mid_R) { // L หรือ R เจอดำ
      break; // ถ้าเซ็นเซอร์ตัวซ้ายสุด หรือ ตัวขาวสุด เจอดำ จะออก loop
    }
  }
  ao(); //stop
}
void ssl(int speed){  // ***หมุนซ้ายด้วยความเร็ว speed 30 จนเจอเส้น
  sl(30);delay(300); //ให้หมุนไป 45 องศาก่อนเพื่อให้เซ็นเซอร์ a1 อยู่บนพื้นขาว
  while(analog(1)>mid_L){sl(speed);} // a1 เจอขาวทำ sl จนเจอดำ
  while(analog(3)>mid_l){sl(speed);} // a3 เจอขาวทำ sl จนเจอดำ
  while(analog(3)<mid_l){sl(speed);}	// a3 เจอดำทำ sl จนเจอขาว
  ao(); //stop
}
