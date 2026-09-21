//Stable PID tuning list:
//    maxspeed        Kp           Ki          Kd
//      20       ||   3      ||   0.5   ||    0.02
//      40       ||   6.75   ||   0.5   ||    0.10

// PID OVERHAULED V2.5 by Phoenix_Editz


// Notice how dt is now passed as an argument! No more duplicate micros()
float Calculate_PID(float Kp, float Ki, float Kd, float dt) {
  float P = error;

  if (abs(error) <= 1) {
    I += error * dt;
    I = constrain(I, -100, 100);
  } else {
    I = 0;
  }

  // Calculate true derivative
  float D_raw = (error - previous_error) / dt;

  // ==== FIX: THE SHOCK ABSORBER ====
  // Hard snaps into B1 cause D_raw to skyrocket past +/-600.
  // This causes the violent sideways glitch. We clamp it down to prevent snapbacks!
  float D = constrain(D_raw, -150.0, 150.0);

  float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

  return PID_Value;
}
// float Calculate_PID(float Kp, float Ki, float Kd, float dt) {

//   float P = error;

//   if (abs(error) <= 1) {  // Careful: Depending on scaling, you might need a different bounds here.
//     I += error * dt;
//     I = constrain(I, -100, 100);
//   } else {
//     I = 0;
//   }

//   // Perfectly shares the single dt calculated by the wrapper function
//   float D = (error - previous_error) / dt;

//   float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

//   // We DO NOT update `previous_error` here anymore, because
//   // trackLine_PID uses previous_error right after Calculate_PID finishes!
//   // We will move `previous_error = error` to the very bottom of the wrapper loop.

//   return PID_Value;
// }

// Change global variable to float!
float last_position = 1500.0;

float readLine() {
  long sum_position = 0;
  long total_weight = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    long line_strength = sensorRef[i] - sensorValue[i];
    if (line_strength > 0) {
      sum_position += (line_strength * (i * 1000L));
      total_weight += line_strength;
    }
  }

  float calculated_position = 1500.0;

  if (total_weight <= 0) {
    // Tightened bounds: If it slightly misses the turn, force it to hunt aggressively
    if (last_position >= (CENTER_POS - 100) && last_position <= (CENTER_POS + 100)) {
      calculated_position = CENTER_POS;
    } else if (last_position < (CENTER_POS - 100)) {
      calculated_position = MIN_POS;
    } else {
      calculated_position = MAX_POS;
    }

    // FIX 1: If off-line completely, do NOT smooth the data. SNAP it hard
    // to give max reflex into sharp B1 right angles!
    last_position = calculated_position;
  } else {
    calculated_position = (float)sum_position / (float)total_weight;

    // Preserve INFINITE sub-pixel resolution during normal curves like B2
    last_position = (last_position * 0.6) + (calculated_position * 0.4);
  }

  return last_position;
}
// float readLine() {  // Change return type to float
//   long sum_position = 0;
//   long total_weight = 0;

//   for (int i = 0; i < NUM_SENSORS; i++) {
//     long line_strength = sensorRef[i] - sensorValue[i];

//     if (line_strength > 0) {
//       sum_position += (line_strength * (i * 1000L));
//       total_weight += line_strength;
//     }
//   }

//   float calculated_position = 1500.0;  // Float now

//   if (total_weight <= 0) {
//     if (last_position >= (CENTER_POS - 250) && last_position <= (CENTER_POS + 250)) {
//       calculated_position = CENTER_POS;
//     } else if (last_position < (CENTER_POS - 250)) {
//       calculated_position = MIN_POS;
//     } else {
//       calculated_position = MAX_POS;
//     }
//   } else {
//     // Force float division! If we don't use (float), C++ will just divide it as whole numbers
//     calculated_position = (float)sum_position / (float)total_weight;
//   }

//   // Pure floating-point math preserves INFINITE sub-pixel resolution!
//   last_position = (last_position * 0.6) + (calculated_position * 0.4);

//   return last_position;
// }

// Static throttle tracker shared smoothly
static float current_base_speed = 15.0;

void trackLine_PID(float maxspeed, float Kp, float Ki, float Kd) {

  const unsigned long LOOP_RATE = 5000;
  static unsigned long last_loop_time = micros();  // Fixed startup reference offset!

  unsigned long current_time = micros();

  // If a massive menu/setup delay happened >1 second? Restart timers natively avoiding bomb blowup!
  if (current_time - last_loop_time > 1000000) {
    last_loop_time = current_time;
    return;
  }

  if (current_time - last_loop_time >= LOOP_RATE) {

    // 1. CALCULATE TRUE PROTECTED TIME INTERVAL PHYSICS
    float dt = (current_time - last_loop_time) / 1000000.0;
    if (dt <= 0) dt = 0.001;
    if (dt > 0.05) dt = 0.005;  // HARD FIX: STOP MENU CRASH BOMB

    last_loop_time = current_time;

    // 2. READ SUPER HIGH-RES LINE
    readSensors();
    float position = readLine();

    error = (position - CENTER_POS) / 500.0;

    // 3. CURVE HEURISTICS (Raw step differences—absolutely NO "divide by dt" scaling multiplication jitters here!!)
    float delta_error = abs(error - previous_error);

    float error_weight = 4.0;   // Positional weight limits pulling
    float curve_weight = 10.0;  // Shifting edge bounds penalty scalar

    // Absolute penalty metric scaling dynamically down standard power without division bouncing
    // Absolute penalty metric scaling
    float curve_severity = (abs(error) * error_weight) + (delta_error * curve_weight);
    float target_speed = maxspeed - curve_severity;

    // Corner speeds: Tame, stable zero-turns that don't overshoot.
    float current_min_floor = 12.0;

    if (abs(error) >= 2) {
      current_min_floor = -5.0f;  // Perfect tight inner-wheel pivot brake for Z-chicanes
    } else if (abs(error) >= 1) {
      current_min_floor = 5.0f;
    }

    target_speed = constrain(target_speed, current_min_floor, maxspeed);

    // ============================================
    // THE BUG WAS HERE! BYPASS THE TRACTION DELAY:
    // When Max speed is 20, dragging 'current_base_speed' limits causes reverse
    // physics to persist for 300+ms AFTER completing a corner. Let's snap it instantly!
    // ============================================
    current_base_speed = target_speed;

    // 4. DEPLOY RAW ACCURATE CORNER ENGINE
    float PID_value = Calculate_PID(Kp, Ki, Kd, dt);

    // ==========================================
    // 🚦 MONDAY SAVIOR FIX: ANTI-SLIP TRACTION CONTROL 🚦
    // Physical motors on glossy vinyl tracks slip if given too much power on a corner.
    // If we hit a severe Z-chicane (error > 2), kill forward inertia and cap spin speed!
    // ==========================================
    if (abs(error) > 1.8) {
      current_base_speed = -5;  // KILLS forward momentum so it doesn't coast off-track

      // We clamp the violent PID so your left wheel maxes at +35 instead of +70.
      // It will turn purely 90-degrees with high friction and zero skidding!
      PID_value = constrain(PID_value, -35.0, 35.0);
    }
    // ==========================================

    float leftspeed_raw = current_base_speed + PID_value;
    float rightspeed_raw = current_base_speed - PID_value;

    // 5. SECURE OUTPUT BOUNDARIES
    float final_Left = compensateSpeed(leftspeed_raw);
    float final_Right = compensateSpeed(rightspeed_raw);

    final_Left = constrain(final_Left, -100, 100);
    final_Right = constrain(final_Right, -100, 100);

    // EXECUTE
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
  int ramp_up_time = 500;     // ms to hit full speed (tune this)
  int ramp_down_time = 750;   // ms before stopping to start braking (tune this)

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