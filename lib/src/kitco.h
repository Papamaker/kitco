//
//  kitco.h
//  
//
//  Created by francois Jacob on 11/09/2016.
//
//

#ifndef _KITCO_H_
#define _KITCO_H_
#include <SPI.h>
#include "LCD_Functions.h"


#define DEBUGFJA

#ifdef DEBUGFJA
#define debug(A) Serial.println(A)
#else
#define debug(A) ;
#endif


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

byte contraste = 60;
byte lumEcr = 0;


// Les fonctions utilisateurs en français
#define toucheHaut() (LOW == digitalRead(TOUCHE_HAUT))
#define toucheBas() (LOW == digitalRead(TOUCHE_BAS))
#define toucheGauche() (LOW == digitalRead(TOUCHE_GAUCHE))
#define toucheDroite() (LOW == digitalRead(TOUCHE_DROITE))
#define toucheA() (LOW == digitalRead(TOUCHE_A))
#define toucheB() (LOW == digitalRead(TOUCHE_B))

#define touche() (toucheHaut() || toucheBas() || toucheDroite() || toucheGauche() || toucheA() || toucheB()) 


#define LEDVerte(x) digitalWrite(LED_VERTE,x)
#define LEDRouge(x) digitalWrite(LED_ROUGE,x)


void lumiereEcran(byte c) {
    analogWrite(blPin, 255-c);
}

#define contrasteEcran(b) contraste = b; setContrast(b)


#define LARGEUR_ECRAN LCD_WIDTH
#define HAUTEUR_ECRAN LCD_HEIGHT 

#define NOIR BLACK
#define BLANC WHITE

#define effacerEcran(b) clearDisplay(b)
#define ecrireEcran(s,x,y,b) setStr(s,x,y,b)
#define ecrireLettre(c,x,y,b) setChar(c,x,y,b)
#define rafraichirEcran() updateDisplay()

#define frequenceBuzzer(f,d) tone(BUZZER,f,d);delay(d)
#define delai(d) delay(d)

// l'animation de début
// séquence de démarrage pour tester LEDs et écran et buzzer
void animDebut() {

    effacerEcran(BLANC);
    ecrireEcran("Kitco",28,20,NOIR);
    rafraichirEcran();
    
    LEDVerte(HIGH);
    LEDRouge(LOW);
    tone(BUZZER,1000,500);
    delay(500);
    LEDVerte(LOW);
    LEDRouge(HIGH);
    tone(BUZZER,1500,500);
    delay(500);
    LEDVerte(HIGH);
    LEDRouge(LOW);
    tone(BUZZER,2000,500);
    delay(500);
    LEDVerte(LOW);
    LEDRouge(LOW);
    tone(BUZZER,2500,500);
    delay(1000);
    noTone(BUZZER);
    delay(500);
    
    LEDVerte(HIGH);
    LEDRouge(HIGH);
    tone(BUZZER,4000,500);
    delay(1000);
    noTone(BUZZER);
    LEDVerte(LOW);
    LEDRouge(LOW);
    
    delay(2000);
}


// toute l'initalisation kitco (matérielle, etc)
void initialiserKitco(byte anim=true) {

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
    
    // initialisation de l'écran
    lcdBegin();
    contrasteEcran(contraste);
    
    // activer le retro-éclairage
    analogWrite(LCD_LIGHT, 0);
    
    // initialisation du buzzer
    pinMode(BUZZER,OUTPUT);
    
    if (anim) {
        animDebut();
    }
    

}





#endif
