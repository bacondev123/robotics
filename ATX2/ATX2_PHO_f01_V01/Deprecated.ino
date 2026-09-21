#define BATTERY_PIN 7
#define TARGET_VOLTAGE 8.0
#define BOARD_VCC 5.0      
#define DIVIDER_RATIO 2.0  

float getBatteryVoltage() {
//   static float smoothedVoltage = 0.0;
//   float instantReadVolt = (analog(BATTERY_PIN) * BOARD_VCC / 1023.0) * DIVIDER_RATIO;

//   if (smoothedVoltage <= 1.0) { smoothedVoltage = instantReadVolt; }  

//   smoothedVoltage = (smoothedVoltage * 0.95) + (instantReadVolt * 0.05);

  return 8;
}

float compensateSpeed(float requestedPwmBase) {
  // float currentLiveVoltage = getBatteryVoltage();
  
  // if (currentLiveVoltage < 5.0) return requestedPwmBase;  

  // float resultantMultiplier = TARGET_VOLTAGE / currentLiveVoltage;

  // // Safeguards
  // if (resultantMultiplier > 1.3) resultantMultiplier = 1.3;
  // if (resultantMultiplier < 0.8) resultantMultiplier = 0.8;

  // return requestedPwmBase * resultantMultiplier;
  return requestedPwmBase;
}

void checkBatteryRealTime() {
  float liveSystemVolts = getBatteryVoltage();  
  int debugAdcRaw = analog(BATTERY_PIN);      

  glcdClear();
  glcd(0, 0, "Battery Check");
  glcd(1, 0, "Raw ADC: %d", debugAdcRaw);      
  glcd(2, 0, "Voltage: %f V", liveSystemVolts); 

  if (liveSystemVolts < 6.0) {
    glcd(4, 0, "Status: LOW!!");
  } else {
    glcd(4, 0, "Status: OK");
  }
  delay(2000);
}

void setMotors(float requestedLeft, float requestedRight) {
  fd2(round(constrain(compensateSpeed(requestedLeft), -100, 100)), round(constrain(compensateSpeed(requestedRight), -100, 100)));
}