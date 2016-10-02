#ifndef _BOARD_H
#define _BOARD_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "UI.h"
#include "utils.h"

// Le buzzer
#define BUZZER A0

// LEDs
#define LED_VERTE A3
#define LED_ROUGE A2
#define ALLUME HIGH
#define ETEINTE LOW

//SD CARD
#define CS_SD A1

// Les boutons
#define TOUCHE_HAUT   4
#define TOUCHE_BAS    7
#define TOUCHE_GAUCHE  6
#define TOUCHE_DROITE 8
#define TOUCHE_A      2
#define TOUCHE_B      3

byte contraste = 62;
byte lumEcr = 127;


// Software SPI (slower updates, more flexible pin options):
// pin 13 - Serial clock out (SCLK)
// pin 11 - Serial data out (DIN)
// pin 10 - Data/Command select (D/C)
// pin 9 - LCD chip select (CS)
// pin -1: pas de reset - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544( 10, 9, -1);

// Rétro-éclairage
#define LCD_LIGHT 5

// Etat des LEDs
byte etatLEDs = 0;

// intensité du rétro-éclairage
byte retroLcd = 100;


// séquence de démarrage pour tester LEDs et écran et buzzer
void animDebut() {
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(3,display.height()/2-7);
    display.println(" Kitco.fr");
    String test = String("   Pr")+EAIGU+"sente";
    display.println(test);
    display.display();

    setLEDBleue1(HIGH);
    tone(BUZZER,1000,500);
    delay(500);
    setLEDBleue1(LOW);
    setLEDJaune2(HIGH);
    tone(BUZZER,1500,500);
    delay(500);
    setLEDJaune2(LOW);
    setLEDVerte3(HIGH);
    tone(BUZZER,2000,500);
    delay(500);
    setLEDVerte3(LOW);
    setLEDRouge4(HIGH);
    tone(BUZZER,2500,500);
    delay(1000);
    setLEDRouge4(LOW);
    noTone(BUZZER);
    delay(500);
    
    setLEDBleue1(HIGH);
    setLEDJaune2(HIGH);
    setLEDVerte3(HIGH);
    setLEDRouge4(HIGH);
    tone(BUZZER,4000,500);
    delay(1000);
    noTone(BUZZER);
    setLEDBleue1(LOW);
    setLEDJaune2(LOW);
    setLEDVerte3(LOW);
    setLEDRouge4(LOW);
    
    delay(1000);
    
}


// permet de lire l'état des boutons et de le stocker en variable globale 
void litBoutons() {
 
}


// est-ce que les touches sont appuyées 1: oui 0: non
char toucheHaut()   { return (LOW == digitalRead(TOUCHE_HAUT)); }
char toucheBas()    { return (LOW == digitalRead(TOUCHE_BAS)); }
char toucheGauche() { return (LOW == digitalRead(TOUCHE_GAUCHE)); }
char toucheDroite() { return (LOW == digitalRead(TOUCHE_DROITE)); }
char toucheStart()  { return 0; }
char toucheSelect() { return 0; }
char toucheA()      { return (LOW == digitalRead(TOUCHE_A)); }
char toucheB()      { return (LOW == digitalRead(TOUCHE_B)); }


// met à jour les 4 LEDs par le MCP23017
void setLEDs(byte leds) {

}

// met à jour une LED
void setLED(byte ledValue, byte state) { 
}

void setLEDBleue1(byte state) { digitalWrite(LED_VERTE,state);}
void setLEDJaune2(byte state) { digitalWrite(LED_ROUGE,state);}
void setLEDVerte3(byte state) { digitalWrite(LED_VERTE,state);}
void setLEDRouge4(byte state) { digitalWrite(LED_ROUGE,state);}


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
void initBoard(byte anim) {

    // set as outputs
    DDRB |= (1 << DDB7);
    
    // reset LCD
    PORTB &= ~(1 << PORTB7);
    delay(500);
    PORTB |= (1 << PORTB7);

    
  // le buzzer
  pinMode(BUZZER,OUTPUT);

    pinMode(LED_VERTE,OUTPUT);
    pinMode(LED_ROUGE,OUTPUT);
    
    pinMode(A1,OUTPUT);
    
    // Les boutons(pull-ups + input)
    pinMode(TOUCHE_HAUT,INPUT);
    digitalWrite(TOUCHE_HAUT,HIGH);
    pinMode(TOUCHE_DROITE,INPUT);
    digitalWrite(TOUCHE_DROITE,HIGH);
    pinMode(TOUCHE_BAS,INPUT);
    digitalWrite(TOUCHE_BAS,HIGH);
    pinMode(TOUCHE_GAUCHE,INPUT);
    digitalWrite(TOUCHE_GAUCHE,HIGH);
    pinMode(TOUCHE_A,INPUT);
    digitalWrite(TOUCHE_A,HIGH);
    pinMode(TOUCHE_B,INPUT);
    digitalWrite(TOUCHE_B,HIGH);
    

    
    
  // La partie du MCP23017
  Serial.begin(9600);
  
  // Le rétro-éclairage du LCD
  setLcdLight(retroLcd);
  
  display.begin();
  
  // pour autoriser les accents
  display.cp437(true);

  display.setContrast(contraste);
  display.clearDisplay();   // clears the screen and buffer
    if (anim) {
      animDebut();
    }
  
}

#endif
