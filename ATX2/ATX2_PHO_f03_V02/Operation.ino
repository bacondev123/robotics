void Operation() {
  Start();

#if OPERATION_PATH == 1
  Mission_1();
  Mission_2();
  Mission_3();
  Mission_4();
#elif OPERATION_PATH == 2
  Mission_1_V2();
  Mission_2_V2();
  Mission_3_V2();
  Mission_4_V2();
#endif
}

void Mission_1() {
  // executePathSequence("S S ");
  // b1_stone_age_hybrid();
  // executePathSequence("S ");
  // b3();
}

void Mission_2() {
}

void Mission_3() {
}

void Mission_4() {
}

void Mission_1_V2() {
}

void Mission_2_V2() {
}

void Mission_3_V2() {
}

void Mission_4_V2() {
}

void Start() {
}