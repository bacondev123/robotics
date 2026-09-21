#pragma once

namespace Config {
    // ==== SENSORS ====
    constexpr int NUM_SENSORS = 4;
    constexpr int SENSOR_PINS[NUM_SENSORS] = {0, 1, 2, 3};
    constexpr float CENTER_POS = 1500.0f;

    // ==== HARDWARE SPECS ====
    constexpr int BATTERY_PIN = 7;
    constexpr float TARGET_VOLTAGE = 8.0f;
    constexpr float BOARD_VCC = 5.0f;

    // ==== PID TUNING ====
    constexpr float KP_DEFAULT = 4.0f;
    constexpr float KI_DEFAULT = 0.1f;
    constexpr float KD_DEFAULT = 0.03f;
    constexpr float MIN_CORNER_SPEED = 14.0f;
    
    // ==== DELAYS & TIMERS ====
    constexpr unsigned long LOOP_RATE_MICROS = 5000; // 5ms loop
}