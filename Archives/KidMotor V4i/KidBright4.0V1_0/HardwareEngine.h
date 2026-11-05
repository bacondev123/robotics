#pragma once
#include <Arduino.h>

// ====== KIDBRIGHT PINS ======
#define OK_BUTTON 16
#define BUZZER_PIN 13

// ====== MOTOR SHIELD PINS ======
#define DIR1_PIN 26
#define PWM1_PIN 27
#define DIR2_PIN 32
#define PWM2_PIN 33

// ==========================================
// FUNCTION PROTOTYPES (The "Table of Contents")
// ==========================================
void displayPrint(String text, int x = 0, int y = 0, bool clearScreen = true);
void readSensors();
bool isBlack(int i);
bool isWhite(int i);
bool isCenterDetected();
bool isCrossDetected();
void trackLinePID(float maxSpeed, float Kp, float Ki, float Kd);
void resetPID();

// ==========================================
// HARDWARE ENGINE CORE
// ==========================================
inline void beep() {
  tone(BUZZER_PIN, 2000, 100);
  delay(100);
}

inline void initializeHardware() {
  Serial.begin(115200);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(PWM2_PIN, OUTPUT);
  pinMode(OK_BUTTON, INPUT_PULLUP);

  analogWrite(PWM1_PIN, 0);
  analogWrite(PWM2_PIN, 0);
  digitalWrite(DIR1_PIN, LOW);
  digitalWrite(DIR2_PIN, LOW);
  Serial.println("[SYSTEM] Hardware Initialized.");
}

inline void waitForStart() {
  Serial.println("[AWAITING TRIGGER] Press button to proceed...");
  displayPrint("Ready!\nPress Buttonaaaa...", 10, 20, true);

  while (digitalRead(OK_BUTTON) == HIGH) { delay(10); }
  while (digitalRead(OK_BUTTON) == LOW) { delay(10); }

  beep();
  displayPrint("Running...", 30, 25, true);
  delay(500);
}

// Motor Core
inline void fd2(int speedLeft, int speedRight) {
  speedLeft = constrain(speedLeft, -100, 100);
  speedRight = constrain(speedRight, -100, 100);

  int pwm_L = map(abs(speedLeft), 0, 100, 0, 255);
  int pwm_R = map(abs(speedRight), 0, 100, 0, 255);

  digitalWrite(DIR1_PIN, (speedLeft >= 0) ? HIGH : LOW);
  analogWrite(PWM1_PIN, pwm_L);

  digitalWrite(DIR2_PIN, (speedRight >= 0) ? HIGH : LOW);
  analogWrite(PWM2_PIN, pwm_R);
}

// Legacy Aliases
inline void ao() {
  fd2(0, 0);
}
inline void sl(int speed) {
  fd2(-speed, speed);
}
inline void sr(int speed) {
  fd2(speed, -speed);
}