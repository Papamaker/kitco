//
//  kitco.h
//  
//
//  Created by francois Jacob on 11/09/2016.
//
//

#ifndef _KITCO_H_
#define _KITCO_H_
#include "LCD_Functions.h"

// Le buzzer
#define BUZZER A0

// LEDs
#define LED_VERTE A3
#define LED_ROUGE A2


// Les boutons
#define TOUCHE_HAUT   4
#define TOUCHE_BAS    7
#define TOUCHE_GAUCHE  6
#define TOUCHE_DROITE 8
#define TOUCHE_A      2
#define TOUCHE_B      3
char toucheHaut()   { return (LOW == digitalRead(TOUCHE_HAUT)); }
char toucheBas()    { return (LOW == digitalRead(TOUCHE_BAS)); }
char toucheGauche() { return (LOW == digitalRead(TOUCHE_GAUCHE)); }
char toucheDroite() { return (LOW == digitalRead(TOUCHE_DROITE)); }
char toucheA()      { return (LOW == digitalRead(TOUCHE_A)); }
char toucheB()      { return (LOW == digitalRead(TOUCHE_B)); }

byte contrast = 60;

void LEDVerte(byte state) {
    digitalWrite(LED_VERTE,state);
}

void LEDRouge(byte state) {
    digitalWrite(LED_ROUGE,state);
}


void initKitco() {

    
    pinMode(LED_VERTE,OUTPUT);
    
    pinMode(LED_ROUGE,OUTPUT);
    
    
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
    
    
    // initialisation de l'écran
    display.begin();
    display.setContrast(contrast);
    
    // activer le retro-éclairage
    analogWrite(LCD_LIGHT, 0);
    
    // initialisation du buzzer
    pinMode(BUZZER,OUTPUT);
    

}





#endif
