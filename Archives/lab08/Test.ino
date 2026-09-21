void test_1() {
  // skip_close_cross(2, 500);
  Track_Side_Line_PID(3, 20, 8, 0.02, 3);
  delay(300);
  Track_Side_Line_PID(0, 20, 8, 0.02, 3);
  delay(300);
  Track_Side_Line_PID(3, 20, 8, 0.02, 3); 
  // skip_close_cross(1,500);
  // R90(0,1);
  // skip_close_cross(1,500);
  // delay(300);
  // Track_Side_Line_PID(3,20,8,0.02,5);
  // Track_Side_Line_PID(0 , 20, 8, 0.02, 5);
  // skip_close_cross(1,500);
  // L90(200,1);
  // skip_close_cross(1,500);
  // L90(200,3);
  // Track_Side_Line_PID(0, 20, 8, 0.02, 3);
  // Track_Side_Line_PID(3, 20, 8, 0.02, 3);
  // cross(300);
  // R90(200,2);
  // skip_close_cross(2,250);
  // delay(300);
  // R90(0,0);
  // Track_Side_Line_PID(3 ,20, 3, 0.02, 0.75);
  // cross(300);
  // Track_Side_Line_PID(3, 20, 8, 0.02, 0.75);
  // cross(250);
  // R90(200,2);
  // skip_close_cross(1,500);
  // R90(200,2);
  // skip_close_cross(2, 500);
  // R90(200, 2);
  // skip_close_cross(1, 500);
  // R90(200, 2);
  // Track_Side_Line_PID(3, 20, 8, 0.02, 3);
  // cross(200);
  // delay(200);
  // Track_Side_Line_PID(3, 20, 8, 0.02, 3);
  // cross(250);
  // R90(200, 2);
  // skip_close_cross(2, 500);
  // R90(200, 2);
  // skip_circle_right_test(2);
  // cross(300);
  // R90(200,2);
  // skip_circle_right_test(2);
}

void skip_circle_right_test(int count) {
  for(int i = 1; i <= count; i++){
    Track_Side_Line_PID(3, 20, 8, 0.02, 3);
    cross(150);
  }

}