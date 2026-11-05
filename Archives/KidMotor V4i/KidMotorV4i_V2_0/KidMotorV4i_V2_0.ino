#include "HardwareEngine.h"
#include "Config.h"
// #include <PathFinder.h>
// #include <PathFinderV2.h>
#include <PathFinderV3.h>


// ==== ESSENTIAL GLOBAL MEMORY ====
int sensorRef[Config::NUM_SENSORS] = { 2048, 2048 };
int sensorValue[Config::NUM_SENSORS];

float I = 0;
float previousError = 0;
float lineError = 0;
float lastPosition = Config::CENTER_POS;
float currentBaseSpeed = 15.0;

// ==== EXECUTION LOGIC ====
void setup() {

  systemInit();  // Boots hardware, OLED, and RP2040 settings instantly

  waitForStart();
  // Mission_1();
  // showAnalogLive();
  calibrateSensors();

  // Execute Mission 1
  // runMission1();

  // testMotorAndBuzzerPins();
  // testLeftMotorAndBuzzer();
  // calibrateSensors();

  // scanButtonPin();
  // assignmentBackAndForward();
  // displayPrint("1")
  // drawLetterY();


  // waitForStart();
  // fd2(20,20);
  // delay(500);
  // ao();

  // assignmentBackAndForward();

  // runExperiment();
}

void loop() {
  // trackLinePID(30, 4.0, 0.1, 0.03);
  // analogRead(27);
}
// =================================================================
// GUIDEBOOK 2-SENSOR LINE ENGINE (D4 = Pin 26, D5 = Pin 27)
// =================================================================

// 1. Follow line until an intersection cross is detected
void trackToCross(int speed = 35) {
  while (1) {
    readSensors();

    // Intersection: Both Left (0) and Right (1) see Black
    if (isBlack(0) && isBlack(1)) {
      break;
    }

    // 2-Sensor Line Tracking:
    if (isWhite(0) && isWhite(1)) {
      // Centered: Drive straight
      fd2(speed, speed);
    } else if (isBlack(0) && isWhite(1)) {
      // Drifting right -> Steer Left
      fd2(speed - 25, speed + 25);
    } else if (isWhite(0) && isBlack(1)) {
      // Drifting left -> Steer Right
      fd2(speed + 25, speed - 25);
    }

    delay(1);  // Protects the CPU watchdog
  }
  ao();  // Brake at the cross
}

// 2. Drive straight across the intersection to clear the black line
void passStraight(int speed = 35) {
  fd2(speed, speed);
  delay(200);  // Time to clear the cross line onto white
}

// 3. Turn Left 90 degrees at an intersection
void turnLeft90(int turnSpeed = 30) {
  // Nudge axle over the intersection center
  fd2(30, 30);
  delay(180);
  ao();
  delay(50);

  // Blind spin to clear previous line
  sl(turnSpeed);
  delay(220);

  // Spin until Left Sensor locks onto the black line
  while (isWhite(0)) {
    readSensors();
    delay(1);
  }
  // Settle when sensor centers
  while (isBlack(0)) {
    readSensors();
    delay(1);
  }
  ao();
  delay(50);
}

// 4. Turn Right 90 degrees at an intersection
void turnRight90(int turnSpeed = 30) {
  // Nudge axle over the intersection center
  fd2(30, 30);
  delay(180);
  ao();
  delay(50);

  // Blind spin to clear previous line
  sr(turnSpeed);
  delay(220);

  // Spin until Right Sensor locks onto the black line
  while (isWhite(1)) {
    readSensors();
    delay(1);
  }
  while (isBlack(1)) {
    readSensors();
    delay(1);
  }
  ao();
  delay(50);
}

// =================================================================
// ภารกิจ 1 (MISSION 1) EXECUTION ROUTINE
// =================================================================
void runMission1() {
  displayPrint("MISSION 1\nStarting...", 0, 0, true);
  beep();
  delay(500);

  // --- Step 1: Start -> Node 1 (Straight) ---
  displayPrint("Node 1\nStraight", 0, 0, true);
  trackToCross();
  passStraight();

  // --- Step 2: Node 1 -> Node 2 (Turn Right) ---
  displayPrint("Node 2\nTurn RIGHT", 0, 0, true);
  trackToCross();
  turnRight90();

  // --- Step 3: Node 2 -> Node 3 (Turn Left) ---
  displayPrint("Node 3\nTurn LEFT", 0, 0, true);
  trackToCross();
  turnLeft90();

  // --- Step 4: Node 3 -> Node 4 (Turn Left) ---
  displayPrint("Node 4\nTurn LEFT", 0, 0, true);
  trackToCross();
  turnLeft90();

  // --- Step 5: Node 4 -> Node 5 (Straight) ---
  displayPrint("Node 5\nStraight", 0, 0, true);
  trackToCross();
  passStraight();

  // --- Step 6: Node 5 -> Node 6 (Turn Left) ---
  displayPrint("Node 6\nTurn LEFT", 0, 0, true);
  trackToCross();
  turnLeft90();

  // --- Step 7: Node 6 -> Node 7 (Straight) ---
  displayPrint("Node 7\nStraight", 0, 0, true);
  trackToCross();
  passStraight();

  // --- Step 8: Node 7 -> Node 8 (Stop at Goal) ---
  displayPrint("Node 8\nGOAL REACHED!", 0, 0, true);
  trackToCross();
  ao();  // Full stop at Node 8

  beep();
  delay(100);
  beep();
  displayPrint("MISSION 1\nCOMPLETE!", 0, 15, true);
}

// Draws the letter 'Y' made out of the digit '1' using for loops
void drawLetterY() {
  // Step 1: Clear the display first
  displayPrint("", 0, 0, true);

  // Step 2: Draw the upper diagonal arms converging to the center
  // i = 0 -> Left (x: 40, y: 5),  Right (x: 80, y: 5)
  // i = 1 -> Left (x: 50, y: 15), Right (x: 70, y: 15)
  // i = 2 -> Center Junction (x: 60, y: 25)
  for (int i = 0; i <= 2; i++) {
    int y = 5 + (i * 10);
    int xLeft = 40 + (i * 10);
    int xRight = 80 - (i * 10);

    // false = do not clear screen, keep previous characters
    displayPrint("1", xLeft, y, false);

    // Draw right arm (skip duplicate at the center junction when i == 2)
    if (i < 2) {
      displayPrint("1", xRight, y, false);
    }
  }

  // Step 3: Draw the vertical stem going straight down from the junction
  // y = 35, 45, 55 at x = 60
  for (int y = 35; y <= 55; y += 10) {
    displayPrint("1", 60, y, false);
  }
}


// ==========================================
// ASSIGNMENT: FORWARD & BACKWARD VARIATION
// ==========================================
void assignmentBackAndForward() {
  // 1. Initial Prompt
  displayPrint("Please SW1", 0, 0, true);
  waitForStart();

  // 2. Drive Forward
  displayPrint("Forward", 0, 0, true);
  fd2(15, 15);
  delay(3000);  // 3 seconds forward
  ao();
  delay(500);  // Brief pause before reversing

  // 3. Drive Backward
  displayPrint("Backward", 0, 0, true);
  fd2(-20, -20);  // Negative power moves reverse
  delay(3000);    // 3 seconds backward
  ao();

  // 4. Complete -> Ready for next cycle
  displayPrint("Complete!\nPlease SW1", 0, 0, true);
  delay(1000);
}