
// //Straight_PID V1 by Phoenix_Editz

// float gyro_I = 0;
// float gyro_previous_error = 0;
// float gyro_error = 0;

// float Read_Gyro(){
//   return 0.0; // temporary placeholder 
// }
// void Gyro_PID_Reset(){
//   gyro_I = 0;
//   gyro_previous_error = 0;
//   gyro_error = 0;
// }
// float Calculate_Gyro_PID(float Kp_Gyro, float Ki_Gyro, float Kd_Gyro){

//   float P = gyro_error;

//   if(abs(gyro_error) <= 3){
//     gyro_I += gyro_error;
//     gyro_I = constrain(gyro_I, -50, 50);
//   }else{gyro_I = 0;}

//   float D = gyro_error - gyro_previous_error;

//   float Gyro_PID_Value = (Kp_Gyro * P) + (Ki_Gyro * gyro_I) + (Kd_Gyro * D);

//   gyro_error = gyro_previous_error;

//   return Gyro_PID_Value;
// }
// void Straight_PID_Timer(unsigned long target_time, float target_heading, float maxspeed, float Kp, float Ki, float Kd){

//   Gyro_PID_Reset();

//   unsigned long start_time = millis();

//   while(true){
//     if(millis() - start_time >= target_time){
//       fd2(0,0);
//       break;
//     }
//     float current_heading = Read_Gyro();
//     gyro_error = target_heading - current_heading;

//     float Gyro_PID_Value = Calculate_Gyro_PID(Kp, Ki, Kd);

//     float leftspeed = maxspeed + Gyro_PID_Value;
//     float rightspeed = maxspeed - Gyro_PID_Value;

//     leftspeed = constrain(leftspeed, -100, 100);
//     rightspeed = constrain(rightspeed, -100, 100);

//     fd2(leftspeed,rightspeed);
//   }

// }