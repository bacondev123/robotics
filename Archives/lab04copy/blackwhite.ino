void Black() {
  R90(200, 2);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  L90(250, 2);
  Skip_Side_Line(1, 1, 20, 0, Ref_LL);
  delay(2000);
  L90(600, 1);
  Skip_Side_Line(1, 500, 20, 3, Ref_RR);
  R90(250, 1);
  R90(1, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(200, 1);
  ao();
}
void White() {
  L90(200, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  R90(200, 1);
  Skip_Side_Line(1, 1, 20, 0, Ref_LL);
  delay(2000);
  R90(600, 2);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  L90(250, 2);
  L90(1, 1);
  Skip_Side_Line(1, 500, 20, 0, Ref_LL);
  R90(200,2);
  ao();
}