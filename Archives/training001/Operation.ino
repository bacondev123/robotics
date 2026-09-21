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

  RunPath("S S 2L1 T");
  keep_down();
  RunPath("R2 S w 2R2 S S");
  keep_up();
}
void Mission_2() {
  RunPath("2R2 S S 2R2 T");
  keep_down();
  RunPath("L1 S w 2L1 S S");
  keep_up();
}
void Mission_3() {
  RunPath("2R2 S S S L1 S0");
  keep_down();
  RunPath("2R2 S3 R2 S S S");
  keep_up();
}
void Mission_4() {
  RunPath("2R2 S S S R2 S3");
  keep_down();
  RunPath("2L1 S0 L1 S S S");  // finish
}
void Mission_1_V2() {
  RunPath("S3 2R2 S L1 S  2L1 T");
  keep_down();
  RunPath("R2 S w 2R2 S R2 S3 w L1 S");
  keep_up();
}
void Mission_2_V2() {
  RunPath("2R2 S3 2R2 S L1 S 2R2 T");
  keep_down();
  RunPath("L1 S w 2L1 S R2 S3 w L1 S");
  keep_up();
}
void Mission_3_V2() {
  RunPath("2R2 S3 2Rs S L1 S S L1 S0");
  keep_down();
  RunPath("2R2 S3 R2 S S R2 S3 w L1 S");
  keep_up();
}
void Mission_4_V2() {
}
void Start() {
  RunPath("S");
}