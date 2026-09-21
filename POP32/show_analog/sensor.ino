void show_analog() {
    while (1) {
    oled.text(0, 0, "a1=%d    ", analog(1));  // %d แสดงค่า a1
    oled.text(2, 0, "a3=%d    ", analog(3));  // %d แสดงค่า a3
    oled.text(4, 0, "a6=%d    ", analog(6));
    oled.text(6, 0, "a8=%d    ", analog(8));
    oled.show();
  }
}