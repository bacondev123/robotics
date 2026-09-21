//RunPath V5.0
#include <stdlib.h> // Needed for atoi()
#include <ctype.h>  // Needed for isdigit()

void RunPath(const char* path) {

  // Create a local, temporary, fixed-size buffer
  char buffer[64];
  strlcpy(buffer, path, sizeof(buffer));

  // split by spaces
  char* token = strtok(buffer, " ");

  while (token != NULL) {

    int len = strlen(token);

    // Get the first and last characters of the token
    char firstChar = token[0];
    char lastChar  = token[len - 1];

    // ---------------------------------------------------
    // SPECIAL COMMAND: VARIABLE SPEED WALK ("300W" or "3W")
    // ---------------------------------------------------
    // If the token is at least 2 characters, starts with a digit, and ends in 'W'
    if (len >= 2 && isdigit(firstChar) && lastChar == 'W') {
      
      // atoi grabs leading numbers ("300W" -> 300, "3W" -> 3)
      int duration = atoi(token);

      // Shrink logic: If speed is single digit, assume 1 = 100, 2 = 200, etc.
      // (This safely allows both "3W" and "300W")
      if (duration <= 9) {
          duration = duration * 100;
      }

      walkCross_StaticSpeed(duration);
    }

    // ---------------------------------------------------
    // 1-LETTER COMMANDS
    // ---------------------------------------------------
    else if (len == 1) {
      char cmd = token[0];

      if (cmd == 'S') skipCross_PID_Static(1);
      else if (cmd == 'T') trackCross_PID_Static();
      else if (cmd == 'w') walkCross_StaticSpeed(300);
      else if (cmd == 'W') walkCross_StaticSpeed(400);
    }

    // ---------------------------------------------------
    // 2-LETTER COMMANDS ([Letter][Sensor]) Example: "R1"
    // ---------------------------------------------------
    else if (len == 2) {
      // Because we put the "W" check first, we only process Letter+Digit here
      char cmd = token[0];
      int sensor = token[1] - '0';
      if (sensor < 0 || sensor > 9) sensor = 0;  // Safety check

      if (cmd == 'T') trackSideLine_PID_Static(sensor);
      else if (cmd == 'S') skipSideLine_PID(1, sensor);
      else if (cmd == 'L') spinLeft_Sensor(30, sensor);
      else if (cmd == 'R') spinRight_Sensor(30, sensor);
    }

    // ---------------------------------------------------
    // 3-LETTER COMMANDS ([Skip][Letter][Sensor]) Example: "3R1"
    // ---------------------------------------------------
    else if (len == 3) {
      int skipCount = token[0] - '0';
      char cmd = token[1];
      int sensor = token[2] - '0';

      if (skipCount < 1 || skipCount > 9) skipCount = 1;
      if (sensor < 0 || sensor > 9) sensor = 0;

      if (cmd == 'L') spinLeft_SensorLineSkip(30, sensor, skipCount);
      else if (cmd == 'R') spinRight_SensorLineSkip(30, sensor, skipCount);
    }

    // Grab the next command
    token = strtok(NULL, " ");
  }
}
// RunPath V4.0
// Pass by pointer, completely avoiding copying the original string into memory
// void RunPath(const char* path) {

//   // Create a local, temporary, fixed-size buffer to hold our path.
//   // (Adjust the size 64 if your paths might be longer than 63 characters).
//   char buffer[64];
//   strlcpy(buffer, path, sizeof(buffer));

//   // strtok splits a string based on a delimiter (in this case, a space)
//   char* token = strtok(buffer, " ");

//   // Loop until there are no more space-separated words
//   while (token != NULL) {

//     // strlen() replaces .length()
//     int len = strlen(token);

//     // ---------------------------------------------------
//     // 1-LETTER COMMANDS
//     // ---------------------------------------------------
//     if (len == 1) {
//       char cmd = token[0];  // Arrays replace .charAt()

//       if (cmd == 'S') skipCross_PID_Static(1);
//       else if (cmd == 'T') trackCross_PID_Static();
//       else if (cmd == 'w') walkCross_StaticSpeed(300);
//       else if (cmd == 'W') walkCross_StaticSpeed(400);
//     }

//     // ---------------------------------------------------
//     // 2-LETTER COMMANDS ([Letter][Sensor]) Example: "R1"
//     // ---------------------------------------------------
//     else if (len == 2) {
//       char cmd = token[0];
//       int sensor = token[1] - '0';
//       if (sensor < 0 || sensor > 9) sensor = 0;  // Safety check

//       if (cmd == 'T') trackSideLine_PID_Static(sensor);
//       else if (cmd == 'S') skipSideLine_PID(1, sensor);
//       else if (cmd == 'L') spinLeft_Sensor(30, sensor);
//       else if (cmd == 'R') spinRight_Sensor(30, sensor);

//     }

//     // ---------------------------------------------------
//     // 3-LETTER COMMANDS ([Skip][Letter][Sensor]) Example: "3R1"
//     // ---------------------------------------------------
//     else if (len == 3) {
//       int skipCount = token[0] - '0';
//       char cmd = token[1];
//       int sensor = token[2] - '0';

//       if (skipCount < 1 || skipCount > 9) skipCount = 1;
//       if (sensor < 0 || sensor > 9) sensor = 0;

//       if (cmd == 'L') spinLeft_SensorLineSkip(30, sensor, skipCount);
//       else if (cmd == 'R') spinRight_SensorLineSkip(30, sensor, skipCount);
//     }

//     // Grab the next command for the next iteration
//     token = strtok(NULL, " ");
//   }
// }
 
// void RunPath(String path) {
//   path = path + " ";
//   String action = "";

//   for (int i = 0; i < path.length(); i++) {

//     char character = path.charAt(i);

//     if (character == ' ') {
//       if (action != "") {

//         // ---------------------------------------------------
//         // 1-LETTER COMMANDS (No sensor argument needed)
//         // ---------------------------------------------------
//         if (action.length() == 1) {
//           char cmd = action.charAt(0);
//           if (cmd == 'S') skipCross_PID_Static(1);
//           else if (cmd == 'T') trackCross_PID_Static();
//           else if (cmd == 'w') walkCross_StaticSpeed(300);
//           else if (cmd == 'W') walkCross_StaticSpeed(400);
//         }

//         // ---------------------------------------------------
//         // 2-LETTER COMMANDS ([Letter][Sensor]) Example: "R1"
//         // ---------------------------------------------------
//         else if (action.length() == 2) {
//           char cmd = action.charAt(0);
//           int sensor = action.charAt(1) - '0';
//           if (sensor < 0 || sensor > 9) sensor = 0;  // Safety check

//           if (cmd == 'T') trackSideLine_PID_Static(sensor);
//           // skipSideLine_PID
//           else if (cmd == 'S') skipSideLine_PID(1, sensor);

//           // Fixed Speed! Using 30 instead of 0
//           else if (cmd == 'L') spinLeft_Sensor(30, sensor);
//           else if (cmd == 'R') spinRight_Sensor(30, sensor);
//         }

//         // ---------------------------------------------------
//         // 3-LETTER COMMANDS ([Skip][Letter][Sensor]) Example: "3R1"
//         // ---------------------------------------------------
//         else if (action.length() == 3) {
//           // Extract variables
//           int skipCount = action.charAt(0) - '0';
//           char cmd = action.charAt(1);
//           int sensor = action.charAt(2) - '0';

//           // Safety check limits
//           if (skipCount < 1 || skipCount > 9) skipCount = 1;
//           if (sensor < 0 || sensor > 9) sensor = 0;


//           // Execute commands (passing 30 as default turning speed)
//           if (cmd == 'L') spinLeft_SensorLineSkip(30, sensor, skipCount);
//           else if (cmd == 'R') spinRight_SensorLineSkip(30, sensor, skipCount);
//         }

//         action = "";  // Reset for the next command
//       }
//     } else {
//       action = action + character;
//     }
//   }
// }
void trackSideLine_PID_Static(int sensor) {
  float basespeed = 20;
  float Kp = 3;
  float Ki = 0.5;
  float Kd = 0.02;

  while (1) {
    readSensors();

    if (isBlack(sensor)) {
      break;
    } else {
      trackLine_PID(basespeed, Kp, Ki, Kd);
    }
  }
  ao();
  delay(LOOP_DELAY);
}
void trackSideLine_PID(int sensor, float Kp, float Ki, float Kd) {
  float basespeed = 20;

  while (1) {
    readSensors();

    if (isBlack(sensor)) {
      break;
    } else {
      trackLine_PID(basespeed, Kp, Ki, Kd);
    }
  }
  ao();
  delay(LOOP_DELAY);
}
void skipSideLine_PID(int count, int sensor) {
  for (int i = 1; i <= count; i++) {
    walkCross_CustomSpeed(20, SMALL_LINE_SKIP);
    trackSideLine_PID_Static(sensor);
    walkCross_StaticSpeed(LINE_SKIP);
  }
  ao();
  delay(LOOP_DELAY);
}
void skipCross_PID_Static(int count) {
  for (int i = 1; i <= count; i++) {
    walkCross_CustomSpeed(20, SMALL_LINE_SKIP);
    trackCross_PID_Static();
    walkCross_StaticSpeed(LINE_SKIP);
  }
  ao();
  delay(LOOP_DELAY);
}
void skipCross_PID_Static(int amount, void* placeholder = nullptr) {
  for (int i = 1; i <= amount; i++) {
    walkCross_CustomSpeed(20, SMALL_LINE_SKIP);
    trackCross_PID_Static();
    walkCross_StaticSpeed(LINE_SKIP);
  }
  ao();
  delay(LOOP_DELAY);
}

void trackCross_PID_Static() {
  float basespeed = 30;
  float Kp = 4.0;
  float Ki = 0.1;
  float Kd = 0.03;

  while (1) {
    readSensors();

    // int Ref_LL = 593; // temporary fix
    // int Ref_RR = 528; // temporary fix

    // bool Cross = (analog(0) < Ref_LL && analog(3) < Ref_RR); // temporary placeholder

    bool cross = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);

    if (cross) {
      break;
    }

    trackLine_PID(basespeed, Kp, Ki, Kd);
  }
  ao();
  PID_Reset();
  delay(LOOP_DELAY);
}
void trackCross_PID(float kp, float ki, float kd) {
  float basespeed = 20;


  while (1) {
    readSensors();

    // int Ref_LL = 593; // temporary fix
    // int Ref_RR = 528; // temporary fix

    // bool Cross = (analog(0) < Ref_LL && analog(3) < Ref_RR); // temporary placeholder

    bool cross = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);

    if (cross) {
      break;
    }
    trackLine_PID(basespeed, kp, ki, kd);
  }
  ao();
  PID_Reset();
  delay(LOOP_DELAY);
}
void trackCross_PID_Offset(float kp, float ki, float kd, unsigned long leave_time) {
  float basespeed = 20;
  
  // To stop the immediate-trigger glitch if placed on a line!
  unsigned long start_time = millis();

  // ============================================
  // PHASE 1: Find the cross intersection
  // ============================================
  while (1) {
    readSensors();

    bool cross = (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]);

    if (cross && (millis() - start_time > 1000)) {
      break;  // Cross hit! Break into Phase 2
    }
    
    trackLine_PID(basespeed, kp, ki, kd);
  }

  // ============================================
  // PHASE 2: The Cross-Leaving "Blind Walk" 
  // ============================================
  // Once the cross is detected, trigger the timer and lock motors forward
  unsigned long cross_time = millis();
  
  while (millis() - cross_time < leave_time) {
    
    // I am applying `basespeed + 2` to your right motor because I 
    // noticed your other code biases the right motor to walk straight!
    fd2(basespeed, basespeed + 2);
    
  }

  // ============================================
  // PHASE 3: Stop safely
  // ============================================
  ao();
  PID_Reset();
  delay(LOOP_DELAY);
}
void walkCross_CustomSpeed(int max_speed, unsigned long total_timer) {
  int true_min_speed = compensateSpeed(15);         // Prevent stalling
  int true_max_speed = compensateSpeed(max_speed);  // Max Speed
  int ramp_up_time = 150;                           // Time to hit full speed
  int ramp_down_time = 150;                         // Time before stopping to slow down

  unsigned long start_time = millis();

  while (1) {
    unsigned long elapsed_time = millis() - start_time;

    // Have we reached the total requested time? Break out!
    if (elapsed_time >= total_timer) break;

    unsigned long remaining_time = total_timer - elapsed_time;
    int current_speed = true_min_speed;  // Default to min gear

    // 1. RAMP UP PHASE (Steadily climbing from 15 to Max Speed)
    if (elapsed_time <= ramp_up_time) {
      current_speed = true_min_speed + (elapsed_time * (true_max_speed - true_min_speed) / ramp_up_time);
    }

    // 2. RAMP DOWN PHASE (Gliding down from Max Speed back to 15)
    else if (remaining_time <= ramp_down_time) {
      current_speed = true_min_speed + (remaining_time * (true_max_speed - true_min_speed) / ramp_down_time);
      if (current_speed < true_min_speed) current_speed = true_min_speed;  // Floor protect
    }

    // 3. MAX CRUISE SPEED PHASE
    else {
      current_speed = true_max_speed;
    }

    // Apply the brilliantly smoothed speed to the motors!
    // (Using your left/right motor +2 bias like before)
    fd2(current_speed, current_speed + 2);
  }

  // Graceful braking (because we already ramped down to 15, ao() is buttery smooth!)
  ao();
  PID_Reset();
  delay(LOOP_DELAY);
}
void walkCross_StaticSpeed(unsigned long timer) {
  int true_min_speed = compensateSpeed(5);
  int true_max_speed = compensateSpeed(20);
  int ramp_up_time = 150;
  int ramp_down_time = 200;

  unsigned long start_time = millis();
  while (1) {
    unsigned long elapsed_time = millis() - start_time;
    if (elapsed_time >= timer) break;
    unsigned long remaining_time = timer - elapsed_time;
    int current_speed = true_min_speed;
    if (elapsed_time <= ramp_up_time) {
      current_speed = true_min_speed + (elapsed_time * (true_max_speed - true_min_speed) / ramp_up_time);
    } else if (remaining_time <= ramp_down_time) {
      current_speed = true_min_speed + (elapsed_time * (true_max_speed - true_min_speed) / ramp_down_time);
    } else {
      current_speed = true_max_speed;
    }
    fd2(current_speed, current_speed + 2);
  }

  fd2(0, 0);
  ao();
  delay(LOOP_DELAY);
}

// void walkCross_CustomSpeed(int speed, unsigned long timer) {
//   unsigned long start_time = millis();
//   while (millis() - start_time <= timer) {
//     fd2(speed, speed + 2);
//   }

//   fd2(0, 0);
//   ao();
//   delay(LOOP_DELAY);
// }