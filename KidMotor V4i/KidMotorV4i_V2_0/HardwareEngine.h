#pragma once
#include <Arduino.h>

// ====== ROBOT PINS (KidMotor V4 / RP2040) ======
#define OK_BUTTON 8       // User Switch SW1 (Pin 8)
#define SW2_BUTTON 9      // User Switch SW2 (Pin 9)
#define BUZZER_PIN 13     // Built-in Buzzer
// 13
// 14

// --- Motor Pins (DRV8833 H-Bridge) ---
// (Pins 4 & 5 are reserved for OLED I2C; 8 & 9 for SW1/SW2; 26 & 27 for Sensors)
#define M1A_PIN 17         // Left Motor IN1
#define M1B_PIN 16         // Left Motor IN2
#define M2A_PIN 13
#define M2B_PIN 14

// ==========================================
// FUNCTION PROTOTYPES
// ==========================================
void displayPrint(String text, int x = 0, int y = 0, bool clearScreen = true);
void initializeDisplay();
void readSensors();
bool isBlack(int i);
bool isWhite(int i);
bool isCenterDetected();
bool isCrossDetected();
void trackLinePID(float maxSpeed, float Kp, float Ki, float Kd);
void resetPID();
float compensateSpeed(float baseSpeed);
void testMotorAndBuzzerPins();
void showAnalogLive();


// Movement prototypes
void driveForward(int maxSpeed, unsigned long totalTimer);
void driveForwardTime(unsigned long timer);
void trackCross();
void skipCross(int count);
void trackSideLine(int sensorIndex);
void skipSideLine(int count, int sensorIndex);
void spinLeftToLine(int turningSpeed, int sensorIndex);
void spinRightToLine(int turningSpeed, int sensorIndex);
void spinLeftSkipLines(int baseSpeed, int sensorIndex, int skipCount);
void spinRightSkipLines(int baseSpeed, int sensorIndex, int skipCount);

// ==========================================
// HARDWARE ENGINE CORE
// ==========================================
inline void beep() {
  tone(BUZZER_PIN, 2000, 100); 
  delay(100);
}

inline void initializeHardware() {
  Serial.begin(115200);
  
  pinMode(M1A_PIN, OUTPUT);
  pinMode(M1B_PIN, OUTPUT);
  pinMode(M2A_PIN, OUTPUT);
  pinMode(M2B_PIN, OUTPUT);
  
  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(SW2_BUTTON, INPUT_PULLUP);

  // Ensure motors start stopped
  analogWrite(M1A_PIN, 0);
  analogWrite(M1B_PIN, 0);
  analogWrite(M2A_PIN, 0);
  analogWrite(M2B_PIN, 0);

  Serial.println("[SYSTEM] Hardware Initialized (KidMotor V4).");
}

inline void waitForStart() {
  Serial.println("[AWAITING TRIGGER] Press SW1 to proceed...");
  displayPrint("Please SW1", 0, 0, true);
  
  // Wait until SW1 (Pin 8) is pressed (Active LOW)
  while(digitalRead(OK_BUTTON) == HIGH) { delay(10); } 
  while(digitalRead(OK_BUTTON) == LOW)  { delay(10); } 
  
  beep();
  delay(200); 
}

// ==========================================
// DRV8833 DUAL-PWM MOTOR CONTROL
// ==========================================
inline void fd2(int speedLeft, int speedRight) {
  speedLeft = constrain(speedLeft, -100, 100);
  speedRight = constrain(speedRight, -100, 100);

  int pwmL = map(abs(speedLeft), 0, 100, 0, 255);
  int pwmR = map(abs(speedRight), 0, 100, 0, 255);

  // --- Left Motor (M1: Pins 17 & 16) ---
  if (speedLeft > 0) {
    analogWrite(M1A_PIN, pwmL);
    analogWrite(M1B_PIN, 0);
  } else if (speedLeft < 0) {
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, pwmL);
  } else {
    analogWrite(M1A_PIN, 0);
    analogWrite(M1B_PIN, 0);
  }

  // --- Right Motor (M2: Pins 14 & 13) ---
  if (speedRight > 0) {
    analogWrite(M2A_PIN, pwmR);
    analogWrite(M2B_PIN, 0);
  } else if (speedRight < 0) {
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, pwmR);
  } else {
    analogWrite(M2A_PIN, 0);
    analogWrite(M2B_PIN, 0);
  }
}

inline void ao() { fd2(0, 0); }
inline void sl(int speed) { fd2(-speed, speed); }
inline void sr(int speed) { fd2(speed, -speed); }

// ==========================================
// MASTER INITIALIZER
// ==========================================
inline void systemInit() {
  initializeHardware();
  initializeDisplay(); 
  analogReadResolution(12); // RP2040 12-bit ADC fix
  beep();
}