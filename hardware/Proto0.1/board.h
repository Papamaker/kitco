#ifndef _BOARD_H
#define _BOARD_H

#include "Wire.h"
#include "UI.h"

// Etat tampon des boutons (position des bits définies ci-après)
byte etatBoutons = 0;

// touches
#define TOUCHE_HAUT   1
#define TOUCHE_DROITE 2
#define TOUCHE_BAS    4
#define TOUCHE_GAUCHE 8

#define TOUCHE_START  16
#define TOUCHE_SELECT 32

// A: à gauche B: à droite
#define TOUCHE_A      128
#define TOUCHE_B      64

//LEDs
#define LED_BLEUE_1 128
#define LED_JAUNE_2 64
#define LED_VERTE_3 32
#define LED_ROUGE_4 16

// Buzzer
#define BUZZER 2

// Rétro-éclairage
#define LCD_LIGHT 11

// Etat des LEDs
byte etatLEDs = 0;

// intensité du rétro-éclairage
byte retroLcd = 100;

// permet de lire l'état des boutons et de le stocker en variable globale 
void litBoutons() {
   Wire.beginTransmission(0x20);
   Wire.write(0x13); // set MCP23017 memory pointer to GPIOB address
   Wire.endTransmission();
   Wire.requestFrom(0x20, 1); // request one byte of data from MCP20317
   etatBoutons = Wire.read(); // store the incoming byte into boutons 
   etatBoutons = (~etatBoutons)&0xFF;
 
}


// est-ce que les touches sont appuyées 1: oui 0: non
char toucheHaut()   { return (etatBoutons & TOUCHE_HAUT); }
char toucheBas()    { return (etatBoutons & TOUCHE_BAS); }
char toucheGauche() { return (etatBoutons & TOUCHE_GAUCHE); } 
char toucheDroite() { return (etatBoutons & TOUCHE_DROITE); } 
char toucheStart()  { return (etatBoutons & TOUCHE_START); } 
char toucheSelect() { return (etatBoutons & TOUCHE_SELECT); } 
char toucheA()      { return (etatBoutons & TOUCHE_A); } 
char toucheB()      { return (etatBoutons & TOUCHE_B); } 


// met à jour les 4 LEDs par le MCP23017
void setLEDs(byte leds) {
  Wire.beginTransmission(0x20);
  Wire.write(0x12); // address port A
  Wire.write(leds);  // value to send
  Wire.endTransmission();
  etatLEDs = leds;

}

// met à jour une LED
void setLED(byte ledValue, byte state) { 
  if (state) { 
    setLEDs(etatLEDs | ledValue);
  } else {
    setLEDs(etatLEDs & (~ledValue));  
  }
}

void setLEDBleue1(byte state) { setLED(LED_BLEUE_1,state);}
void setLEDJaune2(byte state) { setLED(LED_JAUNE_2,state);}
void setLEDVerte3(byte state) { setLED(LED_VERTE_3,state);}
void setLEDRouge4(byte state) { setLED(LED_ROUGE_4,state);}


// Pour ajuster l'éclairage à pourcent(age) 
void setLcdLight(unsigned char pourcent) {
  if (pourcent > 100) {
  	pourcent = 100;
  }
  
  if (pourcent < 0) {
  	pourcent = 0;
  }
   
  // sets the value (range from 0 to 255):
  int value = (255*(100-pourcent))/100;
  analogWrite(LCD_LIGHT, value);
  
  retroLcd = pourcent;
  
}

// toute l'inistialisation dépendant du matériel
void initBoard() {

  // le buzzer
  pinMode(BUZZER,OUTPUT);

  // La partie du MCP23017
  Serial.begin(9600);
  Wire.begin(); // wake up I2C bus
  Wire.beginTransmission(0x20);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.endTransmission(); 
  Wire.beginTransmission(0x20);
  Wire.write(0x0D); // GPPUB register
  Wire.write(0xFF); // activer les pullups du registre B
  Wire.endTransmission();
  
  // Le rétro-éclairage du LCD
  setLcdLight(retroLcd);
  
}

#endif
