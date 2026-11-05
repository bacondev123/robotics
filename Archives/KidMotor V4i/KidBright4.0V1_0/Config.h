#pragma once

namespace Config {
    // ==== SENSORS MAP (ESP32 / Kidbright PINS) ====
    constexpr int NUM_SENSORS = 4;
    // IN1 = GPIO32 | IN2 = GPIO33 | IN3 = GPIO34 | IN4 = GPIO35
    constexpr int SENSOR_PINS[NUM_SENSORS] = { 32, 33, 34, 35 }; 
    constexpr float CENTER_POS = 1500.0f;
    constexpr float MAX_POS = 3000.0f;
    constexpr float MIN_POS = 0.0f;

    // ==== MOVEMENT & LOOP CONFIG ====
    constexpr int TURN_DELAY = 50;
    constexpr int LOOP_DELAY = 50;
    constexpr int LINE_SKIP = 500;
    constexpr int SMALL_LINE_SKIP = 150;
    constexpr bool USE_PID = 1;     // 1 = PID track, 0 = manual 
    constexpr bool DEBUG_MODE = 1;  // Print logs natively via PC cable

    // ==== ENGINE CORE SETUP ====
    constexpr float MIN_CORNER_SPEED = 14.0f;

    // ==== BATTERY SENSOR CONFIG ====
    constexpr int BATTERY_PIN = 36; // LDR analog channel repurposed
    constexpr float TARGET_VOLTAGE = 8.0f;
    constexpr float DIVIDER_RATIO = 2.0f; // MUST calibrate based on your hardware voltage splitter
}