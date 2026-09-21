// PID V3.3 by Phoenix_Editz

float I = 0;
float previous_error = 0;
float error = 0;

float Calculate_PID(float Kp, float Ki, float Kd) {
  float P = error;

  if (abs(error) <= 1) {
    I += error;
    I = constrain(I, -100, 100);
  } else {
    I = 0;
  }

  // ------------------------------------ disabled previous code
  // if(I > 100) I = 100;
  // if(I < -100) I = -100;
  // ------------------------------------ disabled previous code

  float D = error - previous_error;

  float PID_Value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_error = error;

  return PID_Value;
}

void Read_Sensor() {
  LL = analog(0);
  L = analog(1);
  C = analog(2);
  R = analog(3);
  RR = analog(4);
}

void Check_Error() {

  Read_Sensor();

  bool s0 = (LL < Ref_LL);
  bool s1 = (L < Ref_L);
  bool s2 = (C < Ref_C);
  bool s3 = (R < Ref_R);
  bool s4 = (RR < Ref_RR);

  if (s0 && s1 && s2 && s3 && s4)
    error = previous_error;

  else if (!s0 && !s1 && !s2 && !s3 && !s4) {  // line lost
    if (previous_error <= 0)
      error = -5;
    else
      error = 5;
  }

  else if (s0 && !s1 && !s2 && !s3 && !s4) error = -4;
  else if (s0 && s1 && !s2 && !s3 && !s4) error = -3;
  else if (!s0 && s1 && !s2 && !s3 && !s4) error = -2;
  else if (!s0 && s1 && s2 && !s3 && !s4) error = -1;
  else if (!s0 && !s1 && s2 && !s3 && !s4) error = 0;
  else if (!s0 && !s1 && s2 && s3 && !s4) error = 1;
  else if (!s0 && !s1 && !s2 && s3 && !s4) error = 2;
  else if (!s0 && !s1 && !s2 && s3 && s4) error = 3;
  else if (!s0 && !s1 && !s2 && !s3 && s4) error = 4;

  else
    error = previous_error;
}

void trackline_PID(float maxspeed, float Kp, float Ki, float Kd) {

  Check_Error();

  float PID_value = Calculate_PID(Kp, Ki, Kd);

  float corner_brake = abs(error) * (maxspeed/10);
  float current_speed = maxspeed - corner_brake;

  if (current_speed <= 15) {
    current_speed = 15;
  }

  float leftspeed = current_speed + PID_value;
  float rightspeed = current_speed - PID_value;

  leftspeed = constrain(leftspeed, -100, 100);
  rightspeed = constrain(rightspeed, -100, 100);

  fd2(leftspeed, rightspeed);
}

void PID_Reset() {
  I = 0;
  error = 0;
  previous_error = 0;
}