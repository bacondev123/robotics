#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 

void executePath(const char* path) {
  char buffer[64];
  strlcpy(buffer, path, sizeof(buffer));
  char* token = strtok(buffer, " ");

  while (token != NULL) {
    int len = strlen(token);
    char firstChar = token[0];
    char lastChar  = token[len - 1];

    if (len >= 2 && isdigit(firstChar) && lastChar == 'W') {
      int duration = atoi(token);
      if (duration <= 9) duration = duration * 100;
      driveForwardTime(duration);
    }
    else if (len == 1) {
      char cmd = token[0];
      if (cmd == 'S') skipCross(1);
      else if (cmd == 'T') trackCross();
      else if (cmd == 'w') driveForwardTime(300);
      else if (cmd == 'W') driveForwardTime(400);
    }
    else if (len == 2) {
      char cmd = token[0];
      int sensor = token[1] - '0';
      if (sensor < 0 || sensor > 9) sensor = 0;  

      if (cmd == 'T') trackSideLine(sensor);
      else if (cmd == 'S') skipSideLine(1, sensor);
      else if (cmd == 'L') spinLeftToLine(30, sensor);
      else if (cmd == 'R') spinRightToLine(30, sensor);
    }
    else if (len == 3) {
      int skipCount = token[0] - '0';
      char cmd = token[1];
      int sensor = token[2] - '0';

      if (skipCount < 1 || skipCount > 9) skipCount = 1;
      if (sensor < 0 || sensor > 9) sensor = 0;

      if (cmd == 'L') spinLeftSkipLines(30, sensor, skipCount);
      else if (cmd == 'R') spinRightSkipLines(30, sensor, skipCount);
    }
    token = strtok(NULL, " ");
  }
}