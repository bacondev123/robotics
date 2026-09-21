void b2() {
  // executePathSequence("T3");
  traverseIntersectionBlind(18, 0.5, 0.02, 0);
  // followUntilSensorDetectedPID(3, 10, 0.5, 0.02);
  // pivotLeftSkipLines(30, 3, 1);
  // driveForwardConstant(300);
  // followUntilSensorDetectedPID(0, 10, 0.5, 0.02);
  // skipCrossIntersections(1);
}

void b1_stone_age_hybrid() {
  followUntilSensorDetected(3);
  delay(300);
  spinLeft_Delay(300);  // Legacy turn function: Base 20 speed spin to guarantee no slipping!
  delay(300);
  fowardLineStop(15);   // Legacy stop function
  // followUntilSensorDetected(0);
  delay(300);
  pivotRightSkipLines(30, 2, 1);
  delay(300);
  driveForwardConstant(450);
  delay(300);
  pivotRightSkipLines(30, 2, 1);
  delay(300);
  // skipSideLineJunctions(1, 0);
  followUntilSensorDetected(0);
  delay(100);
  fowardLineStop(15);
  delay(300);
  pivotLeftSkipLines(30, 1, 1);
  fowardLineStop(15);
  delay(300);
  pivotLeftSkipLines(30, 2, 1);
  delay(300);
  driveForwardConstant(450);
  delay(300);
  pivotRightSkipLines(30, 1, 1);

  // spinLeft_Delay(100);
  // pivotLeftToLine(20, 0);
}

void b3() {
  executePathSequence("T");
}

void b4(const char* variations) {
  if (variations == "R") {
    executePathSequence("w 1R2 T w 1R1 W");
  } else if (variations == "2R") {
    executePathSequence("w 1R2 T w 1L2 T w 1R1 W");
  } else if (variations == "C2R") {
    executePathSequence("w 1R2 T w 1R2 T3 w 1R1 W");
  } else if (variations == "2C2R") {
    executePathSequence("w 1R2 T w 1R2 T3 w 1L1 T3 w 1R2 T W");
  } else if (variations == "3C2R") {
    executePathSequence("w 1R2 T3 w 1L2 T3 w 1R2 T3 w 1L1 T W 1R1");
  }
}

void b5(const char* variations) {
  if (variations == "R") {
    executePathSequence("w 1R2 T w 1R1 W");
  } else if (variations == "2R") {
    executePathSequence("w 1R2 T w 1L2 T3 W 1R1");
  } else if (variations == "L") {
    executePathSequence("w 1L1 T w 1L2 W");
  } else if (variations == "2L") {
    executePathSequence("w 1L1 T w 1R1 T0 W 1L2");
  }
}

void b6(const char* variations) {
  if (variations == "L") {
    executePathSequence("1L2 T0 w 1L1 T W");
  } else if (variations == "2L") {
    executePathSequence("1L2 T0 w 1R2 T0 w 1L1 T W");
  } else if (variations == "3L") {
    executePathSequence("1L2 T0 w 1R2 T0 200W T0 w 1L1 T W");
  } else if (variations == "4L") {
    executePathSequence("1L2 T0 w 1R2 T0 200W T0 200W T0 1L1 T W");
  }
}

void b7(const char* variations) {
  if (variations == "L") {
    executePathSequence("T W 1L1 T W");
  } else if (variations == "2L") {
    executePathSequence("T W 1L1 W 1R2 T3 W 1L2 T3 W");
  } else if (variations == "3L") {
    executePathSequence("T W 1L1 W 1R2 T3 W 1R1 T3 W 1L2 T W");
  } else if (variations == "4L") {
    executePathSequence("T W 1L1 W 1R2 T3 W 1R1 T3 W 1R1 T3 W 1L2 T W");
  } else if (variations == "R") {
    executePathSequence("T W 1R2 T W");
  } else if (variations == "2R") {
    executePathSequence("T W 1R2 W 1L1 T0 W 1R1 T W");
  } else if (variations == "3R") {
    executePathSequence("T W 1R2 W 1L1 T0 W 1L2 T0 W 1R1 T W");
  } else if (variations == "4R") {
    executePathSequence("T W 1R2 W 1L1 T0 W 1L2 T0 W 1R1 T0 W 1R1 T W");
  }
}