int get_sensor_ref(int sensor){
  switch(sensor){
    case 0:return Ref_LL;
    case 1:return Ref_L;
    case 2:return Ref_R;
    case 3:return Ref_RR;
    default:return 0;
  }
}