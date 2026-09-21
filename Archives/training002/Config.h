#pragma once

namespace Config {
    // ==== SENSORS ====
    constexpr int NUM_SENSORS = 4;
    constexpr int SENSOR_PINS[NUM_SENSORS] = { 0, 1, 2, 3 };
    constexpr float CENTER_POS = 1500.0f;
    constexpr float MAX_POS = 3000.0f;
    constexpr float MIN_POS = 0.0f;

    // ==== HARDWARE & SERVO ====
    constexpr int BATTERY_PIN = 7;
    constexpr float TARGET_VOLTAGE = 8.0f;
    constexpr float BOARD_VCC = 5.0f; 
    constexpr float DIVIDER_RATIO = 2.0f;
    
    constexpr int SERVO_PIN = 1;
    constexpr int SERVO_UP_ANGLE = 90;
    constexpr int SERVO_DOWN_ANGLE = 170;

    // ==== PID & MOTION DEFAULTS ====
    constexpr unsigned long LOOP_RATE = 5000; // 5ms loop
    constexpr float MIN_CORNER_SPEED = 14.0f;
    constexpr float ACCEL_TRACTION = 0.4f;
    constexpr float DECEL_BRAKE = 2.4f;
    
    constexpr float KP_DEFAULT = 4.0f;
    constexpr float KP_AGGRESSIVE = 18.0f;
    constexpr float KI_DEFAULT = 0.1f;
    constexpr float KD_DEFAULT = 0.03f;
}