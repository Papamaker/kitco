// Template de base pour tout programme Kitco:


// Nécessaire pour l'environnement Kitco
#include "kitco.h"


// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(1);
  lcdBegin();

  effacerEcran(BLANC);
  ecrireEcran("Bonjour!",0,0,NOIR);
  rafraichirEcran();

  
}

// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {

}
