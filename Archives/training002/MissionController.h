#include "Arduino.h"
#pragma once
#include "Config.h"
#include "Hardware.h"
#include "MotionEngine.h"
#include "Missions.h"

class MissionController {
private:
  Hardware& robot;
  MotionEngine motion;

  enum class RobotState { RUN_MISSION,
                          TRACKING,
                          TRACKING_SIDE,
                          SKIPPING,
                          SPINNING,
                          SERVO_DROPPING,
                          WAITING,
                          STOPPED };
  RobotState currentState = RobotState::STOPPED;

  const Step* activeMission;
  int stepIndex = 0;

  int p1_target = 0;
  int p2_target = 0;
  int count = 0;

  unsigned long last_loop_time = 0;
  unsigned long actionTimer = 0;
  int actionStep = 0;  // The master key: Action phases!

  void executeNextMissionStep() {
    Step currentStep = activeMission[stepIndex];
    p1_target = currentStep.p1;
    p2_target = currentStep.p2;
    count = 0;
    actionStep = 0;

    switch (currentStep.action) {
      case Cmd::TRACK_CROSS:
        currentState = RobotState::TRACKING;
        actionTimer = millis();  // Start safety blind timer
        break;
      case Cmd::TRACK_SIDE:
        currentState = RobotState::TRACKING_SIDE;
        actionTimer = millis();
        break;
      case Cmd::SKIP_CROSS:
        currentState = RobotState::SKIPPING;
        actionTimer = millis();
        break;
      case Cmd::SPIN_LEFT:
      case Cmd::SPIN_RIGHT:
        currentState = RobotState::SPINNING;
        break;
      case Cmd::SERVO_DROP:
        currentState = RobotState::SERVO_DROPPING;
        break;
      case Cmd::WAIT:
        currentState = RobotState::WAITING;
        actionTimer = millis();
        break;
      case Cmd::END:
        currentState = RobotState::STOPPED;
        break;
    }
    stepIndex++;
  }

public:
  MissionController(Hardware& hw)
    : robot(hw) {}

  void startMission(const Step* missionArray) {
    activeMission = missionArray;
    stepIndex = 0;
    motion.reset();
    currentState = RobotState::RUN_MISSION;
  }

  void run() {
    unsigned long current_time = micros();

    // Prevent large time jump errors (like after pausing/starting)
    if (current_time - last_loop_time > 1000000) {
      last_loop_time = current_time;
      return;
    }

    if (current_time - last_loop_time >= Config::LOOP_RATE) {
      // Sub-pixel DeltaTime Physics calculation
      float dt = constrain((current_time - last_loop_time) / 1000000.0f, 0.001f, 0.05f);
      last_loop_time = current_time;

      robot.readSensors();
      float position = robot.readLine();

      switch (currentState) {
        case RobotState::RUN_MISSION:
          executeNextMissionStep();
          break;

          // ===============================================
          // SENSEI FIX 1: DEBOUNCED TRACKING
          // ===============================================
          // ===============================================
          // STANDARD CROSS TRACKING
          // p1_target = 0 (Normal Kp), 1 (Aggressive Kp)
          // ===============================================
        case RobotState::TRACKING:
          {
            // SENSEI FIX: Dynamic Gear shifting!
            float active_Kp = (p1_target == 1) ? Config::KP_AGGRESSIVE : Config::KP_DEFAULT;

            auto motorOut = motion.calculateTracking(30.0f, position, dt, active_Kp, Config::KI_DEFAULT, Config::KD_DEFAULT);
            robot.setMotors(motorOut.left, motorOut.right);

            if (actionStep == 0) {
              if (millis() - actionTimer > 100) actionStep = 1;
            } else if (actionStep == 1) {
              // Standard condition: Waits for BOTH side sensors
              if (robot.isCross()) {
                robot.stopMotors();
                currentState = RobotState::RUN_MISSION;
              }
            }
            break;
          }

        // ===============================================
        // SIDE LINE TRACKING (Mimicking Track_Side_Line_PID)
        // p1_target = The Sensor Index to stop on (e.g. 0 for far Left)
        // p2_target = 0 (Normal Kp), 1 (Aggressive Kp)
        // ===============================================
        case RobotState::TRACKING_SIDE:
          {

            float active_Kp = (p2_target == 1) ? Config::KP_AGGRESSIVE : Config::KP_DEFAULT;

            auto motorOut = motion.calculateTracking(30.0f, position, dt, active_Kp, Config::KI_DEFAULT, Config::KD_DEFAULT);
            robot.setMotors(motorOut.left, motorOut.right);

            // Standard 100ms Debounce window
            if (actionStep == 0) {
              if (millis() - actionTimer > 100) actionStep = 1;
            } else if (actionStep == 1) {
              // EXCLUSIVE CONDITION: Looks ONLY at the exact side sensor we asked for!
              if (robot.isBlack(p1_target)) {
                robot.stopMotors();
                currentState = RobotState::RUN_MISSION;
              }
            }
            break;
          }

        // ===============================================
        // SENSEI FIX 2: SKIPPING BEHAVIOR (Matches your V1 exactly)
        // Phase 0: Escaping the starting intersection
        // Phase 1: Track safely down the line searching for NEXT cross
        // Phase 2: Shove the bot straight over the new line
        // ===============================================
        case RobotState::SKIPPING:
          {

            if (actionStep == 0) {
              robot.setMotors(20, 20);  // Push off! (Replicates SMALL_LINE_SKIP from V1)
              if (millis() - actionTimer > 150) {
                actionStep = 1;
              }
            } else if (actionStep == 1) {
              // Safety Line Track Mode! Let it navigate the corridor searching for Cross.
              auto motorOut = motion.calculateTracking(20.0f, position, dt, Config::KP_DEFAULT, Config::KI_DEFAULT, Config::KD_DEFAULT);
              robot.setMotors(motorOut.left, motorOut.right);

              if (robot.isCross()) {
                count++;
                actionStep = 2;  // Move to walk-over Phase
                actionTimer = millis();
              }
            } else if (actionStep == 2) {
              // We hit the line, push straight ahead for 250ms (mimicking walkCross_StaticSpeed(LINE_SKIP))
              robot.setMotors(20, 20);
              if (millis() - actionTimer > 250) {
                if (count >= p1_target) {
                  // Final Skip Achieved!
                  robot.stopMotors();
                  currentState = RobotState::RUN_MISSION;
                } else {
                  // Not enough skips yet! Do the cycle again!
                  actionStep = 0;
                  actionTimer = millis();
                }
              }
            }
            break;
          }

        // ===============================================
        // SENSEI FIX 3: TWO-STAGE DYNAMIC ESCAPE SPINNING
        // Instead of skipping instantly because we sit on black, we
        // wait for floor -> THEN search for Black again.
        // ===============================================
        case RobotState::SPINNING:
          {
            if (activeMission[stepIndex - 1].action == Cmd::SPIN_LEFT) robot.setMotors(-30, 30);
            else robot.setMotors(30, -30);

            // PHASE 0: Dynamic Escape. Spin blind until sensor clears the starting line (Wait for White!)
            if (actionStep == 0) {
              if (robot.isWhite(1) && robot.isWhite(2)) {  // Left Line + cleared White!
                actionStep = 1;
              }
            }
            // PHASE 1: Target Acquisition. Spin until we locate the New Line! (Wait for Black!)
            else if (actionStep == 1) {
              if (robot.isBlack(p2_target)) {
                count++;  // Mark a Spoke cleared!

                if (count >= p1_target) {  // Are we finally done spinning?
                  robot.stopMotors();
                  currentState = RobotState::RUN_MISSION;
                } else {
                  // Going again for multi-skipping lines? Fallback to blind escape Phase!
                  actionStep = 0;
                }
              }
            }
            break;
          }

        // (Servo code stays untouched; it perfectly proved my point to begin with!)
        case RobotState::SERVO_DROPPING:
          if (actionStep == 0) {
            robot.setServo(Config::SERVO_DOWN_ANGLE);
            actionTimer = millis();
            actionStep = 1;
          } else if (actionStep == 1 && millis() - actionTimer >= 1000) {
            robot.setServo(Config::SERVO_UP_ANGLE);
            robot.stopMotors();
            actionTimer = millis();
            actionStep = 2;
          } else if (actionStep == 2 && millis() - actionTimer >= 250) {
            currentState = RobotState::RUN_MISSION;
          }
          break;

        case RobotState::WAITING:
          robot.stopMotors();
          if (millis() - actionTimer >= (unsigned long)p1_target) {
            currentState = RobotState::RUN_MISSION;
          }
          break;

        case RobotState::STOPPED:
          robot.stopMotors();
          break;
      }
    }
  }

  bool isFinished() {
    return currentState == RobotState::STOPPED;
  }
};
