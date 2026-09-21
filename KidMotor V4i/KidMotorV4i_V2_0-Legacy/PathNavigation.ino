// Instantiate the global graph pathfinder (supports up to 32 nodes, 8 edges each)
Pathfinder mapPlanner;

// Current tracked robot heading on the field
Direction currentRobotHeading = DIR_N;

// =================================================================
// 1. PHYSICAL TURN EXECUTOR (Translates TurnAction to Motors)
// =================================================================
void executeTurnAction(TurnAction action) {
  switch (action) {
    case ACTION_STRAIGHT:
      Serial.println("[NAV] Action: Straight");
      displayPrint("Action:\nStraight", 0, 20, false);
      // Small forward push to clear the current intersection node
      driveForward(20, Config::SMALL_LINE_SKIP);
      break;

    case ACTION_RIGHT_90:
      Serial.println("[NAV] Action: Right 90 deg");
      displayPrint("Action:\nTurn RIGHT 90", 0, 20, false);
      spinRightToLine(30, 1); // Adjust sensor index as needed for your line alignment
      break;

    case ACTION_LEFT_90:
      Serial.println("[NAV] Action: Left 90 deg");
      displayPrint("Action:\nTurn LEFT 90", 0, 20, false);
      spinLeftToLine(30, 0);
      break;

    case ACTION_UTURN_180:
      Serial.println("[NAV] Action: U-Turn 180 deg");
      displayPrint("Action:\nU-TURN 180", 0, 20, false);
      // Spin until line is cleared, then lock onto the next line
      sl(30);
      delay(250);
      spinLeftToLine(30, 0);
      break;

    case ACTION_RIGHT_45:
      Serial.println("[NAV] Action: Right 45 deg");
      displayPrint("Action:\nTurn RIGHT 45", 0, 20, false);
      spinRightToLine(25, 1);
      break;

    case ACTION_LEFT_45:
      Serial.println("[NAV] Action: Left 45 deg");
      displayPrint("Action:\nTurn LEFT 45", 0, 20, false);
      spinLeftToLine(25, 0);
      break;

    case ACTION_RIGHT_135:
      Serial.println("[NAV] Action: Right 135 deg");
      displayPrint("Action:\nTurn RIGHT 135", 0, 20, false);
      spinRightSkipLines(30, 1, 2);
      break;

    case ACTION_LEFT_135:
      Serial.println("[NAV] Action: Left 135 deg");
      displayPrint("Action:\nTurn LEFT 135", 0, 20, false);
      spinLeftSkipLines(30, 0, 2);
      break;

    default:
      Serial.println("[NAV] Action: Unknown (Holding Straight)");
      driveForward(20, Config::SMALL_LINE_SKIP);
      break;
  }
}

// =================================================================
// 2. STEP-BY-STEP PATH EXECUTION
// =================================================================
void executeNavigationPlan(const Pathfinder::PlanType &plan) {
  if (!plan.isValid || plan.nodeCount == 0) {
    Serial.println("[NAV ERROR] Cannot execute invalid plan!");
    displayPrint("ERROR:\nInvalid Plan!", 0, 0, true);
    beep();
    delay(1000);
    return;
  }

  Serial.printf("\n[NAV] Executing Plan with %d steps. Total Dist: %d\n", plan.stepCount, plan.totalDistance);

  for (uint8_t i = 0; i < plan.stepCount; i++) {
    const PathStep &step = plan.steps[i];

    // --- OLED & Serial Telemetry ---
    String oledHeader = "Step " + String(i + 1) + "/" + String(plan.stepCount);
    oledHeader += "\n" + String(step.fromNode) + " -> " + String(step.toNode);
    displayPrint(oledHeader, 0, 0, true);

    Serial.printf("\n--> Step %d/%d: Node %d -> Node %d\n", i + 1, plan.stepCount, step.fromNode, step.toNode);
    Serial.printf("    Target Heading: %d | Turn Deg: %d\n", step.targetHeading, step.turnAngleDeg);

    // 1. Perform orientation turn at current intersection node
    executeTurnAction(step.turn);

    // 2. Track line to next intersection node
    Serial.println("[NAV] Tracking segment to next node...");
    trackCross(); // Tracks the line and halts when intersection cross is detected

    // 3. Update active heading state
    currentRobotHeading = step.targetHeading;
  }

  // --- Goal Reached Sequence ---
  ao(); // Stop all motors
  beep();
  delay(100);
  beep();
  Serial.println("[NAV SUCCESS] Goal node reached successfully!");
  displayPrint("GOAL REACHED!\nDist: " + String(plan.totalDistance), 0, 10, true);
}

// =================================================================
// 3. USER-FRIENDLY ONE-LINER PATH RUNNER
// =================================================================
bool navigateTo(uint8_t startNode, uint8_t goalNode, Direction startHeading) {
  Pathfinder::PlanType plan;

  Serial.printf("\n[PLANNER] Searching path: Node %d -> Node %d (Heading %d)...\n", startNode, goalNode, startHeading);
  displayPrint("Planning Path...\n" + String(startNode) + " -> " + String(goalNode), 0, 0, true);

  bool found = mapPlanner.planPath(startNode, goalNode, startHeading, plan);

  if (!found || !plan.isValid) {
    Serial.println("[PLANNER FAILED] No viable path found between nodes!");
    displayPrint("Path FAILED!\nNo route found", 0, 0, true);
    beep();
    return false;
  }

  // Path planned successfully -> Execute movement
  executeNavigationPlan(plan);
  return true;
}

// Optional helper for dynamic runtime obstacle avoidance
void setEdgeBlocked(uint8_t u, uint8_t v, bool isBlocked) {
  mapPlanner.setEdgeEnabled(u, v, !isBlocked);
  Serial.printf("[MAP] Edge (%d <-> %d) %s\n", u, v, isBlocked ? "BLOCKED" : "OPEN");
}