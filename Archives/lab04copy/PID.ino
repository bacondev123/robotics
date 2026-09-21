float P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0;
int error = 0;
int basespeed;

void Calculate_PID(float Kp, float Ki, float Kd) {
  P = error;
  I += error;
  if(I > 100) I = 100;
  if(I < -100) I = -100;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
}
void Read_Sensor() {
  LL = analog(0);
  L = analog(1);
  R = analog(2);
  RR = analog(3);
}
void Check_Error() {

  Read_Sensor();

  int s0 = (LL < Ref_LL);
  int s1 = (L  < Ref_L);
  int s2 = (R  < Ref_R);
  int s3 = (RR < Ref_RR);

  if     (s0 && !s1 && !s2 && !s3) error = -3;
  else if(s0 && s1  && !s2 && !s3) error = -2;
  else if(!s0 && s1 && !s2 && !s3) error = -1;
  else if(!s0 && s1 && s2 && !s3)  error = 0;
  else if(!s0 && !s1 && s2 && !s3) error = 1;
  else if(!s0 && !s1 && s2 && s3)  error = 2;
  else if(!s0 && !s1 && !s2 && s3) error = 3;
}
void trackline_PID(float basespeed, float KP, float KI, float KD){
  Check_Error();
  Calculate_PID(KP, KI, KD);

  int leftspeed = basespeed + PID_value;
  int rightspeed = basespeed - PID_value;
  
  fd2(leftspeed, rightspeed);

}