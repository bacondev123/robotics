#pragma once

enum class Cmd {
  SKIP_CROSS,
  TRACK_CROSS,
  TRACK_SIDE,
  SPIN_LEFT,
  SPIN_RIGHT,
  SERVO_DROP,
  WAIT,
  END
};

struct Step {
  Cmd action;
  int p1;  // Parameter 1 (Skips, Spin Counts, or Wait Time)
  int p2;  // Parameter 2 (Target Sensor for spinning)
};

// --- MISSION 1 (Exact match to your original string!) ---
const Step Mission_1[] = {
  {Cmd::SPIN_LEFT, 1, 3},
  {Cmd::END, NULL, NULL},
  
  // { Cmd::TRACK_CROSS, 1, 0 },
  // { Cmd::TRACK_SIDE, 0, 1 },
  // { Cmd::WAIT, 300, NULL },
  // { Cmd::SPIN_LEFT, 1, 3 },
  // { Cmd::WAIT, 300, NULL },
  // { Cmd::TRACK_CROSS, 0, 0 },
  // { Cmd::END, NULL, NULL },
  // --- RunPath("S S 2L1 T") ---
  // {Cmd::SKIP_CROSS,  1, 0}, // S: Skip 1 cross
  // {Cmd::SKIP_CROSS,  1, 0}, // S: Skip 1 cross
  // {Cmd::SPIN_LEFT,   2, 1}, // 2L1: Spin Left until Sensor 1 sees black 2 times
  // {Cmd::TRACK_CROSS, 0, 0}, // T: Track line until next cross

  // // --- keep_down() ---
  // {Cmd::SERVO_DROP,  0, 0}, // Drops the servo to 170, waits 1s, resets to 90, waits 250ms

  // // --- RunPath("R2 S w 2R2 S S") ---
  // {Cmd::SPIN_RIGHT,  1, 2}, // R2: Spin Right until Sensor 2 sees black 1 time
  // {Cmd::SKIP_CROSS,  1, 0}, // S: Skip 1 cross
  // {Cmd::WAIT,      500, 0}, // w: Small wait/pause (set to 500ms, adjust as needed)
  // {Cmd::SPIN_RIGHT,  2, 2}, // 2R2: Spin Right until Sensor 2 sees black 2 times
  // {Cmd::SKIP_CROSS,  1, 0}, // S: Skip 1 cross
  // {Cmd::SKIP_CROSS,  1, 0}, // S: Skip 1 cross

  // // --- keep_up() ---
  // {Cmd::WAIT,     3000, 0}, // keep_up() was a 3-second delay, so we Wait 3000ms

  // {Cmd::END,         0, 0}  // Stop the robot and signal the Brain that Mission 1 is done
};

// --- MISSION 2, 3, and 4 Placeholders ---
// You will translate your original Mission 2, 3, and 4 into these arrays!
const Step Mission_2[] = {
  { Cmd::END, 0, 0 }
};

const Step Mission_3[] = {
  { Cmd::END, 0, 0 }
};

const Step Mission_4[] = {
  { Cmd::END, 0, 0 }
};