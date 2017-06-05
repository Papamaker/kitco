/* 
 *  Programme de démo qui dessine un rectangle dont la taille dépend des    
 *  deux capteurs analogiques branchés en A4/A5 (par exemple un joystick analogique)
 *  
 */
 
 #include "kitco.h"

byte sensorPin1 = A4;    // select the input pin for the potentiometer
byte sensorPin2 = A5;    // select the input pin for the potentiometer
long sensorValue1 = 0;
long sensorValue2 = 0;

long x = 0;
long y = 0;

void setup() {

  initialiserKitco(1);
  lcdBegin();
  setContrast(60);
}

void loop() {
  
  // read the value from the sensor:
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);

  x = (sensorValue1*84)/1024;
  y = (sensorValue2*48)/1024;
  
  clearDisplay(BLANC);
  
   
  creerRectangle(0, 0, x, y, true, NOIR);
  updateDisplay();

}
