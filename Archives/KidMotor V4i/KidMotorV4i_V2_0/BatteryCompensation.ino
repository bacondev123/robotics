float getBatteryVoltage() {
  static float filteredVoltage = 0.0;
  float instantVoltage = (analogRead(Config::BATTERY_PIN) * 3.3 / 4095.0) * Config::DIVIDER_RATIO;

  if (filteredVoltage <= 1.0) { filteredVoltage = instantVoltage; }  
  filteredVoltage = (filteredVoltage * 0.95) + (instantVoltage * 0.05);
  return filteredVoltage;
}

float compensateSpeed(float baseSpeed) {
  float currentV = getBatteryVoltage();
  if (currentV < 5.0) return baseSpeed; 

  float multiplier = Config::TARGET_VOLTAGE / currentV;
  if (multiplier > 1.3) multiplier = 1.3;
  if (multiplier < 0.8) multiplier = 0.8;

  return baseSpeed * multiplier;
}

void printBatteryStatus() {
  float currentV = getBatteryVoltage(); 
  
  Serial.print("Core Voltage: ");
  Serial.print(currentV);
  Serial.println("V");
  
  if (currentV < 6.0) Serial.println("STATUS: WARNING LOW BATTERY!");
}