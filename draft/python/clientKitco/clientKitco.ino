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

  // Allumer la LED verte
  if (toucheA()) {
    LEDVerte(ALLUME);
    Serial.println("LED VERTE!");
  } else {
    LEDVerte(ETEINTE);
  }
  if (toucheB()) {
    LEDRouge(ALLUME);
    Serial.println("LED ROUGE!");
  } else {
    LEDRouge(ETEINTE);
  }
}
