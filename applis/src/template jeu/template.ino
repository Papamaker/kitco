// Template pourprogrammer un jeu  
// www.kitco.fr

// Nécessaire pour l'environnement Kitco
#include "kitco.h"

// Les constantes

// l'événement en cours  
byte evenement;

// Les événements du jeu

// Une partie commence
void nouvellePartie() {

}

// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(1);
  lcdBegin();
  setContrast(60);

  // remise à zero de la partie
  nouvellePartie();
}

// Gestion des graphismes
void afficheJeu(){

  clearDisplay(BLANC);

  updateDisplay();
}

// Gestion des appuis touches
void gestionTouches(){

}

// gestion de la logique du jeu
void logiqueJeu() {

}

void sonFinPartie() {
  frequenceBuzzer(1000,100);
  frequenceBuzzer(1000,100);
  frequenceBuzzer(1000,300);
  frequenceBuzzer(2000,1000);
}

void sonJeu() {
  switch (evenement) {
    default:
      delai(DUREE_SON);
      break;
  }

  // cas special fin de partie 
  if (evenement == FIN_PARTIE) {
    sonFinPartie();
    while (!touche());
    nouvellePartie();
  }
  
  // on remet à zero l'évènement
  evenement = RIEN;
  
}

// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {
  gestionTouches();
  logiqueJeu();
  afficheJeu();
  sonJeu();
}

