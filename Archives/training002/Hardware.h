#pragma once
#include <ATX2.h>
#include "Config.h"

class Hardware {
private:
    float last_position = Config::CENTER_POS;
    int sensorValue[Config::NUM_SENSORS];
    int sensorRef[Config::NUM_SENSORS] = { 783, 784, 656, 681 }; 

public:
    void init() {
        XIO();
        OK();
        servo(Config::SERVO_PIN, Config::SERVO_UP_ANGLE);
        glcdMode(1);
        glcdClear();
    }

    // --- SENSORS & LINE MATH ---
    void readSensors() {
        for (int i = 0; i < Config::NUM_SENSORS; i++) {
            sensorValue[i] = analog(Config::SENSOR_PINS[i]);
        }
    }

    bool isCross() { return (sensorValue[0] < sensorRef[0]) && (sensorValue[3] < sensorRef[3]); }
    bool isWhite(int index) { return sensorValue[index] >= sensorRef[index]; }
    bool isBlack(int index) { return sensorValue[index] < sensorRef[index]; }

    float readLine() { 
        long sum_position = 0;
        long total_weight = 0;
        for (int i = 0; i < Config::NUM_SENSORS; i++) {
            long line_strength = sensorRef[i] - sensorValue[i];
            if (line_strength > 0) {
                sum_position += (line_strength * (i * 1000L));
                total_weight += line_strength;
            }
        }
        float calculated_position = Config::CENTER_POS; 
        if (total_weight <= 0) {
            if (last_position >= (Config::CENTER_POS - 250) && last_position <= (Config::CENTER_POS + 250)) calculated_position = Config::CENTER_POS;
            else if (last_position < (Config::CENTER_POS - 250)) calculated_position = Config::MIN_POS;
            else calculated_position = Config::MAX_POS;
        } else {
            calculated_position = (float)sum_position / (float)total_weight;
        }
        last_position = (last_position * 0.6f) + (calculated_position * 0.4f);
        return last_position;
    }

    // --- MOTORS & BATTERY ---
    float getBatteryVoltage() {
        static float filtered_voltage = 0.0f;
        float instant_voltage = (analog(Config::BATTERY_PIN) * Config::BOARD_VCC / 1023.0f) * Config::DIVIDER_RATIO;
        if (filtered_voltage <= 1.0f) { filtered_voltage = instant_voltage; } 
        filtered_voltage = (filtered_voltage * 0.95f) + (instant_voltage * 0.05f); 
        return filtered_voltage;
    }

    float compensateSpeed(float baseSpeed) {
        float currentV = getBatteryVoltage();
        if (currentV < 5.0f) return baseSpeed; 
        float multiplier = Config::TARGET_VOLTAGE / currentV;
        return constrain(baseSpeed * multiplier, baseSpeed * 0.8f, baseSpeed * 1.3f); 
    }

    void setMotors(float leftRaw, float rightRaw) {
        fd2(round(constrain(compensateSpeed(leftRaw), -100, 100)), round(constrain(compensateSpeed(rightRaw), -100, 100)));
    }
    void stopMotors() { ao(); }

    // --- SERVO & KNOB ---
    void setServo(int angle) { servo(Config::SERVO_PIN, angle); }
    int readKnob() { return knob(0, 180); }
    void printKnob(int val) { glcd(1, 0, "Knob= %d   ", val); }
};