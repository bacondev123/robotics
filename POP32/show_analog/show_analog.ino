#include <POP32.h>
// *** กำหนดค่ากลางเซ็นเซอร์แต่ละตัวตามสไลด์
int mid_L = 2663;  // ค่าอ้างอิงกึ่งกลางเซ็นเซอร์ a1 (ซ้ายสุด)
int mid_l = 1814;  // ค่าอ้างอิงกึ่งกลางเซ็นเซอร์ a3 (ซ้ายใน)
int mid_r = 2813;  // ค่าอ้างอิงกึ่งกลางเซ็นเซอร์ a6 (ขวาใน)
int mid_R = 2554;  // ค่าอ้างอิงกึ่งกลางเซ็นเซอร์ a8 (ขวาสุด)

void setup() {
  oled.clear();
  oled.text(0, 0, "SW_OK");
  oled.show();
  waitSW_OK();
  beep();
  oled.clear();
  oled.text(0, 0, "RUN..");
  oled.show();
  // show_analog();
  // Operation();
  Experiment();
  // motor_tune(20,21.2);

  // sl(20);
  // delay(1000);
  // ao();
  // waitSW_OK();
  // sr(20);
  // delay(1000);
  // ao();

  // track_jc(20);
  // check_line();
  // ao();

  // waitSW_OK();
  // track_jc(20);

  // track_line(20);
  ao();
}

void loop() {
  // track(20);
}
