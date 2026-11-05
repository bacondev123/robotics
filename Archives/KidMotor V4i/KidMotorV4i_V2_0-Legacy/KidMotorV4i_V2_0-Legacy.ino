#include "HardwareEngine.h"
#include "Config.h"
// #include <PathFinder.h>
// #include <PathFinderV2.h>
#include <PathFinderV3.h>
 

// ==== ESSENTIAL GLOBAL MEMORY ====
int sensorRef[Config::NUM_SENSORS] = { 2048, 2048}; 
int sensorValue[Config::NUM_SENSORS];

float I = 0;
float previousError = 0;
float lineError = 0; 
float lastPosition = Config::CENTER_POS;
float currentBaseSpeed = 15.0;  

// ==== EXECUTION LOGIC ====
void setup() {
  
  systemInit(); // Boots hardware, OLED, and RP2040 settings instantly
  
  // testMotorAndBuzzerPins();
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

  analogRead(27);
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
    int xLeft  = 40 + (i * 10);
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
  fd2(20, 20);
  delay(3000); // 3 seconds forward
  ao();
  delay(500);  // Brief pause before reversing

  // 3. Drive Backward
  displayPrint("Backward", 0, 0, true);
  fd2(-20, -20); // Negative power moves reverse
  delay(3000); // 3 seconds backward
  ao();

  // 4. Complete -> Ready for next cycle
  displayPrint("Complete!\nPlease SW1", 0, 0, true);
  delay(1000);
}