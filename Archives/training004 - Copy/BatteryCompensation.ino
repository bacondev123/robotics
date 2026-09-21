// ==== ตั้งค่าความแม่นยำของฮาร์ดแวร์ ====
#define BATTERY_PIN 7
#define TARGET_VOLTAGE 8.0
#define BOARD_VCC 5.0      // *ถ้าใช้มัลติมิเตอร์วัดไฟ 5V บนบอร์ดได้เท่าไหร่ ให้เอามาแก้ตรงนี้ เช่น 4.95
#define DIVIDER_RATIO 2.0  // *ถ้าความต้านทานไม่เท่ากันเป๊ะ ให้ปรับเลขนี้จนกว่าหน้าจอจะโชว์ตรงกับมัลติมิเตอร์

float getBatteryVoltage() {
  static float filtered_voltage = 0.0;

  // Read once instantaneously
  float instant_voltage = (analog(BATTERY_PIN) * BOARD_VCC / 1023.0) * DIVIDER_RATIO;

  if (filtered_voltage <= 1.0) { filtered_voltage = instant_voltage; }  // Safety boot lock

  // Digital smoother that doesn't rely on delay
  filtered_voltage = (filtered_voltage * 0.95) + (instant_voltage * 0.05);

  return filtered_voltage;
}

float compensateSpeed(float baseSpeed) {
  float currentV = getBatteryVoltage();

  if (currentV < 5.0) return baseSpeed;  // ป้องกัน Error กรณีสายหลุด

  float multiplier = TARGET_VOLTAGE / currentV;

  // --- ระบบป้องกันความปลอดภัย (Safety Constraint) ---
  // ป้องกันไม่ให้ตัวคูณ สั่งเร่งความเร็วเกิน 30% (1.3) หรือลดความเร็วเกิน 20% (0.8)
  if (multiplier > 1.3) multiplier = 1.3;
  if (multiplier < 0.8) multiplier = 0.8;

  // ใช้ round() เพื่อความแม่นยำในการปัดเศษ แทนการใช้ (int)
  return baseSpeed * multiplier;
}
void checkBatteryRealTime() {
  float currentV = getBatteryVoltage();  // ดึงค่าจากฟังก์ชันที่เราเขียน
  int rawADC = analog(BATTERY_PIN);      // ดูค่าดิบที่บอร์ดอ่านได้ (0-1023)

  glcdClear();
  glcd(0, 0, "Battery Check");
  glcd(1, 0, "Raw ADC: %d", rawADC);      // ถ้าค่าเป็น 0 แสดงว่าสายหลุด
  glcd(2, 0, "Voltage: %f V", currentV);  // ค่าที่คำนวณได้ (ควรใกล้เคียง 8.4V)

  if (currentV < 6.0) {
    glcd(4, 0, "Status: LOW!!");
  } else {
    glcd(4, 0, "Status: OK");
  }
  delay(2000);
}
void setMotors(float leftRaw, float rightRaw) {
  fd2(round(constrain(compensateSpeed(leftRaw), -100, 100)), round(constrain(compensateSpeed(rightRaw), -100, 100)));
}