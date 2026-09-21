#pragma once
#include "Config.h"

class MotionEngine {
private:
    float I = 0.0f;
    float previous_error = 0.0f;
    float current_base_speed = 15.0f;

public:
    void reset() {
        I = 0.0f;
        previous_error = 0.0f;
        current_base_speed = 15.0f;
    }

    struct MotorCommand { float left; float right; };

    MotorCommand calculateTracking(float maxspeed, float position, float dt, float Kp, float Ki, float Kd) {
        float error = (position - Config::CENTER_POS) / 500.0f;

        // Calculate PID purely based on positional error
        float P = error;
        if (abs(error) <= 1) I = constrain(I + (error * dt), -100, 100);
        else I = 0;
        float D = (error - previous_error) / dt;
        float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

        // ==========================================
        // SENSEI FIX: Dynamic Floor Lifting!
        // The further out the line is, the lower the base-speed floor goes!
        // At huge errors (> 2.0), allow the minimum corner speed to completely drop to 0
        // allowing the inner wheel to counter-rotate entirely backwards natively!
        // ==========================================
        float current_min_floor = Config::MIN_CORNER_SPEED;
        if (abs(error) >= 2.0f) { 
            current_min_floor = -35.0f; // Let the robot's base thrust collapse to negative, initiating absolute braking pivot
        }
        else if (abs(error) >= 1.0f) {
            current_min_floor = -10.0f;  // Let it crawl for regular sweeping curves
        }

        float delta_error = abs(error - previous_error);
        float curve_severity = (abs(error) * 4.0f) + (delta_error * 10.0f);
        
        // NOW, instead of hitting an arbitrary hard limit of 14, 
        // it gracefully shifts down as error gets worse.
        float target_speed = constrain(maxspeed - curve_severity, current_min_floor, maxspeed);

        // Traction Control
        if (current_base_speed < target_speed) current_base_speed = min(current_base_speed + Config::ACCEL_TRACTION, target_speed);
        else if (current_base_speed > target_speed) current_base_speed = max(current_base_speed - Config::DECEL_BRAKE, target_speed);

        previous_error = error;
        return { current_base_speed + PID_Value, current_base_speed - PID_Value };
    }
};