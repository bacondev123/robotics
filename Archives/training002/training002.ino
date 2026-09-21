#include "Config.h"
#include "Hardware.h"
#include "MissionController.h"
#include "Missions.h"

Hardware robotHardware;
MissionController robotBrain(robotHardware);

bool isTestMode = false;
int currentMissionNumber = 0; // 0 means we are waiting at the start line

void setup() {
    robotHardware.init();
    
    if (sw_OK()) { 
        // isTestMode = true;
        glcd(0, 0, "TEST MODE");
    } else {
        glcd(0, 0, "READY");
    }
}

void loop() {
    if (isTestMode) {
        int knobValue = robotHardware.readKnob();
        robotHardware.printKnob(knobValue);
        robotHardware.setServo(knobValue);
    } 
    else {
        // 1. Start the entire race sequence
        if (currentMissionNumber == 0 && sw_OK()) {
            currentMissionNumber = 1;
            robotBrain.startMission(Mission_1);
            glcd(0, 0, "RUNNING M1");
        }
        
        // 2. The Automatic Chainer
        // If we are currently running a mission AND the brain just hit "STOPPED"
        if (currentMissionNumber > 0 && robotBrain.isFinished()) {
            currentMissionNumber++; // Advance the tracker
            
            if (currentMissionNumber == 2) {
                robotBrain.startMission(Mission_2);
                glcd(0, 0, "RUNNING M2");
            }
            else if (currentMissionNumber == 3) {
                robotBrain.startMission(Mission_3);
                glcd(0, 0, "RUNNING M3");
            }
            else if (currentMissionNumber == 4) {
                robotBrain.startMission(Mission_4);
                glcd(0, 0, "RUNNING M4");
            }
            else if (currentMissionNumber > 4) {
                glcd(0, 0, "ALL DONE! ");
                currentMissionNumber = 0; // Reset so you can press OK to race again
            }
        }
        
        // 3. Keep the brain thinking
        robotBrain.run();
    }
}