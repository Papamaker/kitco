// Programme clignotant LED verte
// Fait clignoter la LED verte

// Nécessaire pour l'environnement Kitco
#include "kitco.h"



// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(0);

  Serial.begin(9600);
  
}

// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {


  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte=='v') {
       LEDVerte(ALLUME);      
    }
    if (inByte=='r') {
       LEDRouge(ALLUME);      
    }
    if (inByte=='e') {
       LEDRouge(ETEINTE);
       LEDVerte(ETEINTE);
    }
  }
}
