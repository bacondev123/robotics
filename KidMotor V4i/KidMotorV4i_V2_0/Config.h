#pragma once

namespace Config {
    // ==== SENSORS MAP (RP2040 Pins) ====
    constexpr int NUM_SENSORS = 2; // Set to 2 based on your MicroBlock D4/D5 setup
    // D4 = GPIO 26 (ADC0) | D5 = GPIO 27 (ADC1)
    constexpr int SENSOR_PINS[NUM_SENSORS] = { 26, 27 }; 

    constexpr float CENTER_POS = 1500.0f;
    constexpr float MAX_POS = 3000.0f;
    constexpr float MIN_POS = 0.0f;

    // ==== MOVEMENT & LOOP TIMINGS ====
    constexpr int TURN_DELAY = 50;
    constexpr int LOOP_DELAY = 50;
    constexpr int LINE_SKIP = 500;
    constexpr int SMALL_LINE_SKIP = 150;
    constexpr bool USE_PID = 1;     
    constexpr bool DEBUG_MODE = 1;  
    constexpr float MIN_CORNER_SPEED = 14.0f;

    // ==== BATTERY CONFIG (RP2040 ADC Pin) ====
    constexpr int BATTERY_PIN = 28; // GPIO 28 (ADC2)
    constexpr float TARGET_VOLTAGE = 8.0f;
    constexpr float DIVIDER_RATIO = 2.0f;
}