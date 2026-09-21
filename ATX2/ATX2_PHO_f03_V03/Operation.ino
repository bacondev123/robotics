void Operation() {
  Start();

#if OPERATION_PATH == 1
  // Mission_1();
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
  executePathSequence("1L1 S 1R1 S ");
  delay(SEQUENCE_DELAY_MS);
  b1_stone_age_hybrid();
  delay(SEQUENCE_DELAY_MS-300);
  executePathSequence("S");
  delay(SEQUENCE_DELAY_MS);
  b3();
}

void Mission_2() {
  executePathSequence("1R2 S 1L2 S");
  delay(SEQUENCE_DELAY_MS);
  b2();
  delay(SEQUENCE_DELAY_MS-300);
  executePathSequence("S");
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
  executePathSequence("S");
}