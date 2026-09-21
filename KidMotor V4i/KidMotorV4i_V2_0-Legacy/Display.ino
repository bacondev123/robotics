#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initializeDisplay() {
  // KidBright V4 external I2C chain usually uses SDA=21, SCL=22 (ESP32 Default).
  // NOTE: If your screen stays blank, your club might have wired it to the 
  // internal sensor bus instead. If so, uncomment the line below:
  // Wire.begin(4, 5); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("[ERROR] OLED Screen not found!"));
  } else {
    display.clearDisplay();
    display.setTextSize(1);              
    display.setTextColor(SSD1306_WHITE); 
    display.setCursor(0, 0);             
    display.println("OLED Ready!");
    display.display();                   
  }
}

// Notice the default values (= 0, = true) are removed here! 
// They are safely stored in HardwareEngine.h instead.
void displayPrint(String text, int x, int y, bool clearScreen) {
  if (clearScreen) display.clearDisplay();
  display.setCursor(x, y);
  display.print(text);
  display.display();
}