// Programme clignotant LED verte
// Fait clignoter la LED verte

// Nécessaire pour l'environnement Kitco
#include "kitco.h"



// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(0);
  
}

// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {

  // Allumer la LED verte
  LEDVerte(ALLUME);

  // attendre 500 milli-secondes
  delai(500);

  // Eteindre la LED verte
  LEDVerte(ETEINTE);

  // attendre 500 milli-secondes
  delai(500);
}
