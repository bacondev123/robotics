//Stable PID tuning list:
//    maxspeed        Kp           Ki          Kd
//      20       ||   3      ||   0.5   ||    0.02
//      40       ||   6.75   ||   0.5   ||    0.10

// PID OVERHAULED V2.5 by Phoenix_Editz

float I = 0;
float previous_error = 0;
float error = 0;


// Notice how dt is now passed as an argument! No more duplicate micros()
float Calculate_PID(float Kp, float Ki, float Kd, float dt) {

  float P = error;

  if (abs(error) <= 1) {  // Careful: Depending on scaling, you might need a different bounds here.
    I += error * dt;
    I = constrain(I, -100, 100);
  } else {
    I = 0;
  }

  // Perfectly shares the single dt calculated by the wrapper function
  float D = (error - previous_error) / dt;

  float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

  // We DO NOT update `previous_error` here anymore, because
  // trackLine_PID uses previous_error right after Calculate_PID finishes!
  // We will move `previous_error = error` to the very bottom of the wrapper loop.

  return PID_Value;
}

// Change global variable to float!
float last_position = 1500.0;

float readLine() {  // Change return type to float
  long sum_position = 0;
  long total_weight = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    long line_strength = sensorRef[i] - sensorValue[i];

    if (line_strength > 0) {
      sum_position += (line_strength * (i * 1000L));
      total_weight += line_strength;
    }
  }

  float calculated_position = 1500.0;  // Float now

  if (total_weight <= 0) {
    if (last_position >= (CENTER_POS - 250) && last_position <= (CENTER_POS + 250)) {
      calculated_position = CENTER_POS;
    } else if (last_position < (CENTER_POS - 250)) {
      calculated_position = MIN_POS;
    } else {
      calculated_position = MAX_POS;
    }
  } else {
    // Force float division! If we don't use (float), C++ will just divide it as whole numbers
    calculated_position = (float)sum_position / (float)total_weight;
  }

  // Pure floating-point math preserves INFINITE sub-pixel resolution!
  last_position = (last_position * 0.6) + (calculated_position * 0.4);

  return last_position;
}

// Static throttle tracker shared smoothly
static float current_base_speed = 15.0;  

void trackLine_PID(float maxspeed, float Kp, float Ki, float Kd) {

  const unsigned long LOOP_RATE = 5000;
  static unsigned long last_loop_time = micros(); // Fixed startup reference offset!

  unsigned long current_time = micros();

  // If a massive menu/setup delay happened >1 second? Restart timers natively avoiding bomb blowup!
  if(current_time - last_loop_time > 1000000){ last_loop_time = current_time; return; }

  if (current_time - last_loop_time >= LOOP_RATE) {

    // 1. CALCULATE TRUE PROTECTED TIME INTERVAL PHYSICS
    float dt = (current_time - last_loop_time) / 1000000.0;
    if (dt <= 0) dt = 0.001;
    if (dt > 0.05) dt = 0.005; // HARD FIX: STOP MENU CRASH BOMB 

    last_loop_time = current_time;

    // 2. READ SUPER HIGH-RES LINE
    readSensors();
    float position = readLine();  

    error = (position - CENTER_POS) / 500.0;

    // 3. CURVE HEURISTICS (Raw step differences—absolutely NO "divide by dt" scaling multiplication jitters here!!)
    float delta_error = abs(error - previous_error);

    float error_weight = 4.0;       // Positional weight limits pulling
    float curve_weight = 10.0;      // Shifting edge bounds penalty scalar 

    // Absolute penalty metric scaling dynamically down standard power without division bouncing  
    float curve_severity = (abs(error) * error_weight) + (delta_error * curve_weight);
    float target_speed = maxspeed - curve_severity;

    // Fixed absolute stall bottom 
    const float MIN_CORNER_SPEED = 14.0;
    target_speed = constrain(target_speed, MIN_CORNER_SPEED, maxspeed);

    // ============================================
    // TRACTION CONTROL MOTOR SYSTEM 
    // Frame increment changes. Instead of dropping violently (-3.0 every 5ms!), 
    // it smoothly accelerates/brakes realisticly over physical fractions of real time so no wobbling happens.
    float accel_traction = 0.15;  // 0.15 unit shifts across cycles guarantees realistic straight bursts!
    float decel_brake    = 0.5;   // True proportional engine dropping instead of 0-dead constraints
    
    if (current_base_speed < target_speed) {
      current_base_speed += accel_traction;
      if (current_base_speed > target_speed) current_base_speed = target_speed;
    } else if (current_base_speed > target_speed) {
      current_base_speed -= decel_brake;  
      if (current_base_speed < target_speed) current_base_speed = target_speed;
    }
    // ============================================

    // 4. DEPLOY RAW ACCURATE CORNER ENGINE PROTOTYPING
    float PID_value = Calculate_PID(Kp, Ki, Kd, dt);

    float leftspeed_raw = current_base_speed + PID_value;
    float rightspeed_raw = current_base_speed - PID_value;

    // 5. SECURE OUTPUT BOUNDARIES
    float final_Left = compensateSpeed(leftspeed_raw);
    float final_Right = compensateSpeed(rightspeed_raw);

    final_Left = constrain(final_Left, -100, 100);
    final_Right = constrain(final_Right, -100, 100);

    // EXECTUTION COMMAND DIRECT RAW PROXY
    fd2(round(final_Left), round(final_Right));

    previous_error = error;
  }
}
void trackLineTime_PID_Acceleration(float max_speed, unsigned long total_timer) {
  // stable tuning values
  float Kp = 4.0;
  float Ki = 0.1;
  float Kd = 0.03;

  // Ramp configuration
  float true_max_speed = max_speed;
  float true_min_speed = 15;  // Must match your PID's MIN_CORNER_SPEED
  int ramp_up_time = 500;                      // ms to hit full speed (tune this)
  int ramp_down_time = 750;                    // ms before stopping to start braking (tune this)

  unsigned long start_time = millis();

  while (1) {
    unsigned long elapsed_time = millis() - start_time;

    // Time's up! Break out for a smooth stop.
    if (elapsed_time >= total_timer) break;

    unsigned long remaining_time = total_timer - elapsed_time;
    float current_speed = true_min_speed;

    // 1. RAMP UP PHASE (Accelerating off the starting line)
    if (elapsed_time <= ramp_up_time) {
      current_speed = true_min_speed + ((float)elapsed_time * (true_max_speed - true_min_speed) / ramp_up_time);
    }
    // 2. RAMP DOWN PHASE (Braking before the timer ends)
    else if (remaining_time <= ramp_down_time) {
      current_speed = true_min_speed + ((float)remaining_time * (true_max_speed - true_min_speed) / ramp_down_time);
      if (current_speed < true_min_speed) current_speed = true_min_speed;  // Floor protect
    }
    // 3. MAX CRUISE SPEED PHASE
    else {
      current_speed = true_max_speed;
    }

    // Feed the dynamically shifting speed limit into your PID
    trackLine_PID(current_speed, Kp, Ki, Kd);
  }

  // Graceful stop
  ao();  // or fd2(0,0) depending on your motor driver library
  PID_Reset();
}
void trackLineTime_PID_Static(float maxspeed, unsigned long timer) {

  float Kp = 3;
  float Ki = 0.5;
  float Kd = 0.02;
  unsigned long start_time = millis();

  while (millis() - start_time <= timer) {
    trackLine_PID(maxspeed, Kp, Ki, Kd);
  }

  fd2(0, 0);
}

void PID_Reset() {
  I = 0;
  error = 0;
  previous_error = 0;
  current_base_speed = 15.0;
}
void trackLine_PID_TUNE(float baseSpeed, float Kp, float Ki, float Kd) {

  const unsigned long LOOP_RATE = 5000;
  static unsigned long last_loop_time = 0;
  unsigned long current_time = micros();
  if (current_time - last_loop_time >= LOOP_RATE) {
    float dt = (current_time - last_loop_time) / 1000000.0;
    if (dt <= 0) dt = 0.001;

    last_loop_time = current_time;

    readSensors();
    float position = readLine();

    error = (position - CENTER_POS) / 500.0;


    float PID_value = Calculate_PID(Kp, Ki, Kd, dt);

    float left = baseSpeed + PID_value;
    float right = baseSpeed - PID_value;

    left = constrain(left, -100, 100);
    right = constrain(right, -100, 100);

    fd2(left, right);

    previous_error = error;
  }
}
// //Stable PID tuning list:
// //    maxspeed        Kp           Ki          Kd
// //      20       ||   3      ||   0.5   ||    0.02

// // PID OVERHAULED V1.0 by Phoenix_Editz

// float I = 0;
// float previous_error = 0;
// float error = 0;
// unsigned long previous_time = 0;

// // float derivativeDisplay = 0;



// float Calculate_PID(float Kp, float Ki, float Kd) {
//   unsigned long current_time = micros();
//   float dt = (current_time - previous_time) / 1000000.0;
//   previous_time = current_time;

//   // if (dt <= 0 || dt > 1.0) return 0;
//   if (dt <= 0) dt = 0.001;


//   float P = error;

//   if (abs(error) <= 1) {
//     I += error * dt;
//     I = constrain(I, -100, 100);
//   } else {
//     I = 0;
//   }

//   float D = (error - previous_error) / dt;
//   // derivativeDisplay = D;

//   float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

//   previous_error = error;

//   return PID_Value;
// }

// float last_position = 1500;

// int readLine() {
//   long sum_position = 0;
//   long total_weight = 0;

//   for (int i = 0; i < NUM_SENSORS; i++) {

//     // THE TRUE ANALOG MAGIC!
//     // How much shadow is touching the sensor?
//     // Positive = Touching the line. Negative = Looking at floor.
//     long line_strength = sensorRef[i] - sensorValue[i];

//     // If it sees the line at all...
//     if (line_strength > 0) {
//       // It adds fractional, microscopic mathematical weights!
//       sum_position += (line_strength * (i * 1000L));
//       total_weight += line_strength;
//     }
//   }

//   // ===== GHOST CENTER & LINE LOSS MEMORY =====
//   if (total_weight <= 0) {
//     if (last_position >= (CENTER_POS - 250) && last_position <= (CENTER_POS + 250)) {
//       return CENTER_POS;
//     } else if (last_position < (CENTER_POS - 250)) {
//       return MIN_POS;
//     } else {
//       return MAX_POS;
//     }
//   }

//   // Calculate completely continuous sliding float (e.g. 1503, 1492)
//   last_position = sum_position / total_weight;

//   return last_position;
// }

// // Add this global or static variable to track your actual rolling speed
// static float current_base_speed = 15.0; // Start at minimum safe speed

// void trackLine_PID(float maxspeed, float Kp, float Ki, float Kd) {

//   const unsigned long LOOP_RATE = 5000;
//   static unsigned long last_loop_time = 0;

//   unsigned long current_time = micros();

// #if DEBUG_MODE
//   static unsigned long last_debug_time = 0;
//   const unsigned long DEBUG_RATE = 100000;
// #endif

//   if (current_time - last_loop_time >= LOOP_RATE) {

//     last_loop_time = current_time;

//     readSensors();
//     int position = readLine();

//     // 1. Calculate Error
//     error = (position - CENTER_POS) / 500.0;

//     const float SMALL_ERROR_TOLERANCE = 0.05;

//     if(abs(error) <= SMALL_ERROR_TOLERANCE){
//       error = 0.0;
//     }

//     // --- THE PRO CURVE AWARENESS SYSTEM ---

//     // How fast is the error changing? (This predicts a curve BEFORE it gets bad)
//     float delta_error = abs(error - previous_error);

//     // Tuning weights for speed penalties
//     float error_weight = 10.0;      // Penalty for being off-center
//     float derivative_weight = 60.0; // HEAVY penalty for the line running away quickly

//     // Calculate curve severity based on P and D
//     float curve_severity = (abs(error) * error_weight) + (delta_error * derivative_weight);

//     // What speed *should* we be going right now?
//     float target_speed = maxspeed - curve_severity;

//     // Ensure we never drop below a safe cornering stall speed
//     const float MIN_CORNER_SPEED = 15.0;
//     target_speed = constrain(target_speed, MIN_CORNER_SPEED, maxspeed);

//     // --- ASYMMETRIC VELOCITY RAMPING ---

//     // We want to slam the brakes, but roll onto the throttle smoothly.
// float accel_step = 0.8; // How fast it regains speed on straights (TUNE THIS)
// float decel_step = 4.0; // How hard it hits the brakes into corners (TUNE THIS)

// if (current_base_speed < target_speed) {
//   // Smooth roll-on throttle
//   current_base_speed += accel_step;
//   if (current_base_speed > target_speed) current_base_speed = target_speed;
// }
// else if (current_base_speed > target_speed) {
//   // Hard braking
//   current_base_speed -= decel_step;
//   if (current_base_speed < target_speed) current_base_speed = target_speed;
// }

//     // --- STANDARD PID MIXING ---

//     // Calculate PID steering (your existing function)
//     float PID_value = Calculate_PID(Kp, Ki, Kd);

//     // Apply the dynamically ramping base speed to the steering
//     float leftspeed = current_base_speed + PID_value;
//     float rightspeed = current_base_speed - PID_value;

//     leftspeed = constrain(leftspeed, -100, 100);
//     rightspeed = constrain(rightspeed, -100, 100);

//     fd2(leftspeed, rightspeed);

// #if DEBUG_MODE
//     if (current_time - last_debug_time >= DEBUG_RATE) {
//       last_debug_time = current_time;
//       glcd(0, 0, "Err: %f", error);
//       glcd(1, 0, "Spd: %f", current_base_speed);
//     }
// #endif
//   }
// }
// void trackLineTime_PID_Acceleration(float max_speed, unsigned long total_timer) {
//   // stable tuning values
//   float Kp = 3.0;
//   float Ki = 0.5;
//   float Kd = 0.02;

//   // Ramp configuration
//   float true_max_speed = compensateSpeed(max_speed);
//   float true_min_speed = compensateSpeed(15);     // Must match your PID's MIN_CORNER_SPEED
//   int ramp_up_time = 500;      // ms to hit full speed (tune this)
//   int ramp_down_time = 750;    // ms before stopping to start braking (tune this)

//   unsigned long start_time = millis();

//   while (1) {
//     unsigned long elapsed_time = millis() - start_time;

//     // Time's up! Break out for a smooth stop.
//     if (elapsed_time >= total_timer) break;

//     unsigned long remaining_time = total_timer - elapsed_time;
//     float current_speed = true_min_speed;

//     // 1. RAMP UP PHASE (Accelerating off the starting line)
//     if (elapsed_time <= ramp_up_time) {
//       current_speed = true_min_speed + ((float)elapsed_time * (true_max_speed - true_min_speed) / ramp_up_time);
//     }
//     // 2. RAMP DOWN PHASE (Braking before the timer ends)
//     else if (remaining_time <= ramp_down_time) {
//       current_speed = true_min_speed + ((float)remaining_time * (true_max_speed - true_min_speed) / ramp_down_time);
//       if (current_speed < true_min_speed) current_speed = true_min_speed; // Floor protect
//     }
//     // 3. MAX CRUISE SPEED PHASE
//     else {
//       current_speed = true_max_speed;
//     }

//     // Feed the dynamically shifting speed limit into your PID
//     trackLine_PID(current_speed, Kp, Ki, Kd);
//   }

//   // Graceful stop
//   ao(); // or fd2(0,0) depending on your motor driver library
//   PID_Reset();
// }
// void trackLineTime_PID_Static(float maxspeed, unsigned long timer) {

//   float Kp = 3;
//   float Ki = 0.5;
//   float Kd = 0.02;
//   unsigned long start_time = millis();

//   while (millis() - start_time <= timer) {
//     trackLine_PID(maxspeed, Kp, Ki, Kd);
//   }

//   fd2(0, 0);
// }

// void PID_Reset() {
//   I = 0;
//   error = 0;
//   previous_error = 0;
//   previous_time = micros();
//   current_base_speed = 15.0;
// }

// //Stable PID tuning list:
// //    maxspeed        Kp           Ki          Kd
// //      20       ||   3      ||   0.5   ||    0.02

// // PID V5.0 by Phoenix_Editz

// float I = 0;
// float previous_error = 0;
// float error = 0;
// unsigned long previous_time = 0;

// // float derivativeDisplay = 0;

// // Add this global variable at the top of your code
// int last_position = 1500;

// float Calculate_PID(float Kp, float Ki, float Kd) {
//   unsigned long current_time = micros();
//   float dt = (current_time - previous_time) / 1000000.0;
//   previous_time = current_time;

//   // if (dt <= 0 || dt > 1.0) return 0;
//   if (dt <= 0) dt = 0.001;


//   float P = error;

//   if (abs(error) <= 1) {
//     I += error * dt;
//     I = constrain(I, -100, 100);
//   } else {
//     I = 0;
//   }

//   float D = (error - previous_error) / dt;
//   // derivativeDisplay = D;

//   float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

//   previous_error = error;

//   return PID_Value;
// }


// int readLine() {
//   long sum_position = 0;
//   long total_weight = 0;

//   for (int i = 0; i < NUM_SENSORS; i++) {

//     // THE TRUE ANALOG MAGIC!
//     // How much shadow is touching the sensor?
//     // Positive = Touching the line. Negative = Looking at floor.
//     long line_strength = sensorRef[i] - sensorValue[i];

//     // If it sees the line at all...
//     if (line_strength > 0) {
//       // It adds fractional, microscopic mathematical weights!
//       sum_position += (line_strength * (i * 1000L));
//       total_weight += line_strength;
//     }
//   }

//   // ===== GHOST CENTER & LINE LOSS MEMORY =====
//   if (total_weight <= 0) {
//     if (last_position >= (CENTER_POS - 250) && last_position <= (CENTER_POS + 250)) {
//       return CENTER_POS;
//     } else if (last_position < (CENTER_POS - 250)) {
//       return MIN_POS;
//     } else {
//       return MAX_POS;
//     }
//   }

//   // Calculate completely continuous sliding float (e.g. 1503, 1492)
//   last_position = sum_position / total_weight;

//   return last_position;
// }

// void trackLine_PID(float maxspeed, float Kp, float Ki, float Kd) {

//   const unsigned long LOOP_RATE = 5000;
//   static unsigned long last_loop_time = 0;

//   unsigned long current_time = micros();

// #if DEBUG_MODE
//   static unsigned long last_debug_time = 0;
//   const unsigned long DEBUG_RATE = 100000;
// #endif

//   if (current_time - last_loop_time >= LOOP_RATE) {

//     last_loop_time = current_time;

//     float brake_factor = 10;

//     readSensors();

//     int position = readLine();

//     error = (position - CENTER_POS) / 500.0;

//     float PID_value = Calculate_PID(Kp, Ki, Kd);

//     float corner_brake = abs(error) * (maxspeed / brake_factor);
//     float current_speed = maxspeed - corner_brake;

//     if (current_speed <= 15) {
//       current_speed = 15;
//     }

//     float leftspeed = current_speed + PID_value;
//     float rightspeed = current_speed - PID_value;

//     leftspeed = constrain(leftspeed, -100, 100);
//     rightspeed = constrain(rightspeed, -100, 100);

//     fd2(leftspeed, rightspeed);

// #if DEBUG_MODE
//     if (current_time - last_debug_time >= DEBUG_RATE) {
//       last_debug_time = current_time;


//       glcd(0, 0, "Err: %d", (int)error);
//       glcd(1, 0, "PID: %d", (int)PID_value);
//     }
// #endif

//     // glcd(1, 2, "Derivative = %!", derivativeDisplay);
//   }
// }
// void trackLineTime_PID_Static(float maxspeed, unsigned long timer) {

//   float Kp = 3;
//   float Ki = 0.5;
//   float Kd = 0.02;
//   unsigned long start_time = millis();

//   while (millis() - start_time <= timer) {
//     trackLine_PID(maxspeed, Kp, Ki, Kd);
//   }

//   fd2(0, 0);
// }

// void PID_Reset() {
//   I = 0;
//   error = 0;
//   previous_error = 0;
//   previous_time = micros();
// }




// //Stable PID tuning list:
// //    maxspeed        Kp           Ki             Kd
// //      20       ||   4     ||  PLACEHOLDER ||  PLACEHOLDER

// // PID V4.1 by Phoenix_Editz

// float I = 0;
// float previous_error = 0;
// float error = 0;
// unsigned long previous_time = 0;

// // Add this global variable at the top of your code
// int last_position = 1500;

// // Lookup Table mapping the 16 binary sensor states (0-15).
// // 100.0 triggers "Line Lost". 99.0 means "Skip / retain previous_error".
// float errorTable[16] = {
//   100.0, 3.0, 1.0, 2.0, -1.0, 99.0, 0.0, 99.0, -3.0, 99.0, 99.0, 99.0, -2.0, 99.0, 99.0, 0.0
// };

// int readLine() {
//   int position = 0;
//   int activeSensors = 0;

//   for (int i = 0; i < NUM_SENSORS; i++) {
//     // Check if it sees the line
//     if (sensorValue[i] < sensorRef[i]) {
//       position += i * 1000;
//       activeSensors++;
//     }
//   }

//   // ===== GHOST CENTER & LINE LOSS MEMORY =====
//   if (activeSensors == 0) {
//     // 1. GHOST CENTER: If it was just near the center, keep going straight!
//     if (last_position >= 1250 && last_position <= 1750) {
//       return 1500;
//     }
//     // 2. LINE LOST LEFT: Snap completely to the left
//     else if (last_position < 1250) {
//       return 0;
//     }
//     // 3. LINE LOST RIGHT: Snap completely to the right
//     else {
//       return 3000;
//     }
//   }

//   // Calculate new average and save it to memory for next time
//   last_position = position / activeSensors;

//   return last_position;
// }

// float Calculate_PID(float Kp, float Ki, float Kd) {
//   unsigned long current_time = micros();
//   float dt = (current_time - previous_time) / 1000000.0;
//   previous_time = current_time;

//   if (dt <= 0 || dt > 1.0) return 0;

//   float P = error;

//   if (abs(error) <= 1) {
//     I += error * dt;
//     I = constrain(I, -100, 100);
//   } else {
//     I = 0;
//   }

//   float D = (error - previous_error) / dt;

//   float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

//   previous_error = error;

//   return PID_Value;
// }

// void Read_Sensor() {
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     sensorValue[i] = analog(sensorPins[i]);
//   }
// }

// void Check_Error() {

//   Read_Sensor();

//   int b0 = 0;
//   int b1 = 0;
//   int b2 = 0;
//   int b3 = 0;

//   if (sensorValue[0] < sensorRef[0]) b0 = 1;
//   if (sensorValue[1] < sensorRef[1]) b1 = 1;
//   if (sensorValue[2] < sensorRef[2]) b2 = 1;
//   if (sensorValue[3] < sensorRef[3]) b3 = 1;

//   // Convert reading to binary table state
//   int state = (b0 * 8) + (b1 * 4) + (b2 * 2) + (b3 * 1);
//   float new_error = errorTable[state];

//   // ===== GHOST CENTER MOVEMENT PATCH =====
//   if (new_error == 100.0) {  // ALL SENSORS LOST

//     // 1. If it was driving perfectly straight a millisecond ago:
//     // It's not lost! The thin line just slipped exactly between the 2 middle sensors.
//     if (previous_error == 0.0) {
//       error = 0;  // Ignore the gap and KEEP GOING STRAIGHT
//     }
//     // 2. If it drifted left, then dropped:
//     else if (previous_error <= -1.0) {
//       error = -4;
//     }
//     // 3. If it drifted right, then dropped:
//     else if (previous_error >= 1.0) {
//       error = 4;
//     }
//   }

//   // Normal error updating
//   else if (new_error != 99.0) {
//     error = new_error;
//   }
// }

// void trackLine_PID(float maxspeed, float Kp, float Ki, float Kd) {

//   float brake_factor = 10;

//   Check_Error();

//   float PID_value = Calculate_PID(Kp, Ki, Kd);

//   float corner_brake = abs(error) * (maxspeed / brake_factor);
//   float current_speed = maxspeed - corner_brake;

//   if (current_speed <= 15) {
//     current_speed = 15;
//   }

//   float leftspeed = current_speed + PID_value;
//   float rightspeed = current_speed - PID_value;

//   leftspeed = constrain(leftspeed, -100, 100);
//   rightspeed = constrain(rightspeed, -100, 100);

//   fd2(leftspeed, rightspeed);
// }
// void trackLineTime_PID_Static(float maxspeed, unsigned long timer) {

//   float Kp = 4;
//   float Ki = 0;
//   float Kd = 0.1;
//   unsigned long start_time = millis();

//   while (millis() - start_time <= timer) {
//     trackLine_PID(20, Kp, Ki, Kd);
//   }

//   fd2(0,0);
// }

// void PID_Reset() {
//   I = 0;
//   error = 0;
//   previous_error = 0;
//   previous_time = micros();
// }


// // PID V4.0 by Phoenix_Editz

// float I = 0;
// float previous_error = 0;
// float error = 0;
// unsigned long previous_time = 0;

// // Add this global variable at the top of your code
// int last_position = 1500;

// int readLine() {
//   int position = 0;
//   int activeSensors = 0;

//   for (int i = 0; i < NUM_SENSORS; i++) {
//     // REMEMBER: Change < to > if your line is white on a black table!
//     if (sensorValue[i] < sensorRef[i]) {
//       position += i * 1000;
//       activeSensors++;
//     }
//   }

//   // If no sensors see the line
//   if (activeSensors == 0) {
//     // If it was last seen on the left half (0 to 1499), snap to 0
//     if (last_position < 1500) {
//       return 0;
//     }
//     // If it was last seen on the right half (1500 to 3000), snap to 3000
//     else {
//       return 3000;
//     }
//   }

//   // Calculate average and save it to memory for next time
//   last_position = position / activeSensors;

//   return last_position;
// }

// float Calculate_PID(float Kp, float Ki, float Kd) {
//   unsigned long current_time = millis();
//   float dt = (current_time - previous_time) / 1000.0;
//   previous_time = current_time;

//   if (dt <= 0) return 0;

//   float P = error;

//   if (abs(error) <= 1) {
//     I += error * dt;
//     I = constrain(I, -100, 100);
//   } else {
//     I = 0;
//   }

//   float D = (error - previous_error) / dt;

//   float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

//   previous_error = error;

//   return PID_Value;
// }

// void Read_Sensor() {
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     sensorValue[i] = analog(sensorPins[i]);
//   }
// }

// void Check_Error() {

//   Read_Sensor();

//   bool s0 = (sensorValue[0] < sensorRef[0]);
//   bool s1 = (sensorValue[1] < sensorRef[1]);
//   bool s2 = (sensorValue[2] < sensorRef[2]);
//   bool s3 = (sensorValue[3] < sensorRef[3]);

//   if (s0 && s1 && s2 && s3)
//     error = 0;

//   else if (!s0 && !s1 && !s2 && !s3) {  // line lost
//     if (previous_error <= 0)
//       error = -4;
//     else
//       error = 4;
//   }

//   else if (s0 && !s1 && !s2 && !s3) error = -3;
//   else if (s0 && s1 && !s2 && !s3) error = -2;
//   else if (!s0 && s1 && !s2 && !s3) error = -1;
//   else if (!s0 && s1 && s2 && !s3) error = 0;
//   else if (!s0 && !s1 && s2 && !s3) error = 1;
//   else if (!s0 && !s1 && s2 && s3) error = 2;
//   else if (!s0 && !s1 && !s2 && s3) error = 3;

//   else
//     error = previous_error;
// }

// void trackLine_PID(float maxspeed, float Kp, float Ki, float Kd) {

//   float brake_factor = 10;

//   Check_Error();

//   float PID_value = Calculate_PID(Kp, Ki, Kd);

//   float corner_brake = abs(error) * (maxspeed / brake_factor);
//   float current_speed = maxspeed - corner_brake;

//   if (current_speed <= 15) {
//     current_speed = 15;
//   }

//   float leftspeed = current_speed + PID_value;
//   float rightspeed = current_speed - PID_value;

//   leftspeed = constrain(leftspeed, -100, 100);
//   rightspeed = constrain(rightspeed, -100, 100);

//   fd2(leftspeed, rightspeed);
// }

// void PID_Reset() {
//   I = 0;
//   error = 0;
//   previous_error = 0;
//   previous_time = millis();
// }

// // PID V3.5 by Phoenix_Editz

// float I = 0;
// float previous_error = 0;
// float error = 0;

// float Calculate_PID(float Kp, float Ki, float Kd) {
//   float P = error;

//   if (abs(error) <= 1) {
//     I += error;
//     I = constrain(I, -100, 100);
//   } else {
//     I = 0;
//   }

//   float D = error - previous_error;

//   float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

//   previous_error = error;

//   return PID_Value;
// }

// void Read_Sensor() {
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     sensorValue[i] = analog(sensorPins[i]);
//   }
// }

// void Check_Error() {

//   Read_Sensor();

//   bool s0 = (sensorValue[0] < sensorRef[0]);
//   bool s1 = (sensorValue[1] < sensorRef[1]);
//   bool s2 = (sensorValue[2] < sensorRef[2]);
//   bool s3 = (sensorValue[3] < sensorRef[3]);

//   if (s0 && s1 && s2 && s3)
//     error = previous_error;

//   else if (!s0 && !s1 && !s2 && !s3) {  // line lost
//     if (previous_error <= 0)
//       error = -4;
//     else
//       error = 4;
//   }

//   else if (s0 && !s1 && !s2 && !s3) error = -3;
//   else if (s0 && s1 && !s2 && !s3) error = -2;
//   else if (!s0 && s1 && !s2 && !s3) error = -1;
//   else if (!s0 && s1 && s2 && !s3) error = 0;
//   else if (!s0 && !s1 && s2 && !s3) error = 1;
//   else if (!s0 && !s1 && s2 && s3) error = 2;
//   else if (!s0 && !s1 && !s2 && s3) error = 3;

//   else
//     error = previous_error;
// }

// void trackline_PID(float maxspeed, float Kp, float Ki, float Kd) {

//   Check_Error();

//   float PID_value = Calculate_PID(Kp, Ki, Kd);

//   float corner_brake = abs(error) * (maxspeed / 10);
//   float current_speed = maxspeed - corner_brake;

//   if (current_speed <= 15) {
//     current_speed = 15;
//   }

//   float leftspeed = current_speed + PID_value;
//   float rightspeed = current_speed - PID_value;

//   leftspeed = constrain(leftspeed, -100, 100);
//   rightspeed = constrain(rightspeed, -100, 100);

//   fd2(leftspeed, rightspeed);
// }

// void PID_Reset() {
//   I = 0;
//   error = 0;
//   previous_error = 0;
// }
// // PID V3.3 by Phoenix_Editz

// float I = 0;
// float previous_error = 0;
// float error = 0;

// float Calculate_PID(float Kp, float Ki, float Kd) {
//   float P = error;

//   if (abs(error) <= 1) {
//     I += error;
//     I = constrain(I, -100, 100);
//   } else {
//     I = 0;
//   }

//   // ------------------------------------ disabled previous code
//   // if(I > 100) I = 100;
//   // if(I < -100) I = -100;
//   // ------------------------------------ disabled previous code

//   float D = error - previous_error;

//   float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

//   previous_error = error;

//   return PID_Value;
// }

// void Read_Sensor() {
//   LL = analog(0);
//   L = analog(1);
//   R = analog(2);
//   RR = analog(3);
// }

// void Check_Error() {

//   Read_Sensor();

//   bool s0 = (LL < Ref_LL);
//   bool s1 = (L < Ref_L);
//   bool s2 = (R < Ref_R);
//   bool s3 = (RR < Ref_RR);

//   if (s0 && s1 && s2 && s3)
//     error = previous_error;

//   else if (!s0 && !s1 && !s2 && !s3) {  // line lost
//     if (previous_error <= 0)
//       error = -4;
//     else
//       error = 4;
//   }

//   else if (s0 && !s1 && !s2 && !s3) error = -3;
//   else if (s0 && s1 && !s2 && !s3) error = -2;
//   else if (!s0 && s1 && !s2 && !s3) error = -1;
//   else if (!s0 && s1 && s2 && !s3) error = 0;
//   else if (!s0 && !s1 && s2 && !s3) error = 1;
//   else if (!s0 && !s1 && s2 && s3) error = 2;
//   else if (!s0 && !s1 && !s2 && s3) error = 3;

//   else
//     error = previous_error;
// }

// void trackline_PID(float maxspeed, float Kp, float Ki, float Kd) {

//   Check_Error();

//   float PID_value = Calculate_PID(Kp, Ki, Kd);

//   float corner_brake = abs(error) * (maxspeed/10);
//   float current_speed = maxspeed - corner_brake;

//   if (current_speed <= 15) {
//     current_speed = 15;
//   }

//   float leftspeed = current_speed + PID_value;
//   float rightspeed = current_speed - PID_value;

//   leftspeed = constrain(leftspeed, -100, 100);
//   rightspeed = constrain(rightspeed, -100, 100);

//   fd2(leftspeed, rightspeed);
// }

// void PID_Reset() {
//   I = 0;
//   error = 0;
//   previous_error = 0;
// }
