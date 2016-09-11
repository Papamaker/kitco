#include "board.h"

//#define MCP 

#ifdef MCP
#include <Wire.h>
#endif

void setup() {

 
  initBoard(true);

#ifdef MCP
  Wire.begin(); // wake up I2C bus
  Wire.beginTransmission(0x20);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.endTransmission();
#endif
 
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(3,display.height()/2-7);

  if (toucheHaut()) {
    setLEDVerte(HIGH);
    display.println("1");
    display.display();
    delay(100);
    setLEDVerte(LOW);  
  }

  if (toucheGauche()) {
    setLEDVerte(HIGH);
    display.println("2");
    display.display();
    delay(100);
    setLEDVerte(LOW);  
  }


  if (toucheBas()) {
    setLEDVerte(HIGH);
    display.println("3");
    display.display();
    delay(100);
    setLEDVerte(LOW);  
  }

  
  if (toucheDroite()) {
    setLEDVerte(HIGH);
    display.println("4");
    display.display();
    delay(100);
    setLEDVerte(LOW);  
  }


  if (toucheA()) {
    setLEDRouge(HIGH);
    display.println("A");
    display.display();
    delay(100);
    setLEDRouge(LOW);  
  }


  if (toucheB()) {
    setLEDRouge(HIGH);
    display.println("B");
    display.display();
    delay(100);
    setLEDRouge(LOW);  
  }



  display.display();

  
#ifdef MCP 
for (byte i=0;i<10;i++) {
  Wire.beginTransmission(0x20);
  Wire.write(0x12); // address port A
  Wire.write(0x7F);  // value to send
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(0x20);
  Wire.write(0x12); // address port A
  Wire.write(0x80);  // value to send
  Wire.endTransmission();
  delay(100);
}
#endif
  
}
