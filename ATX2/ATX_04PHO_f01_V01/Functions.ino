//เรียงช่องจากซ้ายไปขวา
/*
                      |
      --B3            |               --B4
    +---+             |             +---+
    |   |-------------+-------------|   |
    +---+             |             +---+
         \            |            / 
          \           |           /  
           \          |          /   
            \         |         /    
      --B1   \        |        /      --B3
    +---+     \       |       /     +---+
    |   |------+------+------+------|   |
    +---+     /       |       \     +---+
             /        |        \     
            /         |         \    
           /          |          \   
          /           |           \  
         /            |            \ 
        +-------------+-------------+
        |             |             |
      +---+         +---+         +---+
      | S |--S1     |   |--S2     | p |--S3
      +---+         +---+         +---+
*/
void S1_B1_S1() {
  skip_Side(1, 3);
  delay(500);
  rotate_Right(2, 300);
  skip_Line(1);
  delay(500);
  rotate_Left(1, 200);
  skip_Line(1);
  delay(1000);
  rotate_Left(0, 300);
  rotate_Left(1, 0);
  track_Cross(DEFAULT_SPEED);
  servo_drop();

  rotate_Left(1, 400);
  skip_Line(1);
  fowardDelay(50);
  delay(1000);
  rotate_Right(3, 200);
  delay(1000);
  rotate_Right(2, 0);
  skip_Line(1);
  delay(500);
  rotate_Right(2,200);
  skip_Side(1,3);
  delay(500);
  rotate_Left(1,200);
  skip_Line(1);

}