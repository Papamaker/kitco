// Le jeu de Pong 
// www.kitco.fr

// Nécessaire pour l'environnement Kitco
#include "kitco.h"

// Les constantes
// Positions et tailles des requettes et de la balle  
#define X_RAQUETTE_DROITE  79
#define X_RAQUETTE_GAUCHE  5 
#define EPAISSEUR_RAQUETTE 2
#define EPAISSEUR_BALLE    1
#define LONGUEUR_RAQUETTE  10

// La position du scores
#define X_SCORE_GAUCHE 34
#define X_SCORE_DROITE 46
#define Y_SCORE        2

// Les couleur arrière plan et avant plan (inverser BLANC et NOIR pour avoir le jeu en mode inversé)
#define STYLEFG BLANC
#define STYLEBG NOIR

// La durée des sons du jeu  
#define DUREE_SON 10

// La longueur des raquettes (elles diminuent en cours de jeu)
byte longueurRaquette;

// La position des raquettes
char yRaquetteDroite;
char yRaquetteGauche;

// la position de la balle
char xBalle;
char yBalle;

// l'événement en cours  
byte evenement;

// Les événements du jeu
#define RIEN 0;
#define POINT_JOUEUR_GAUCHE 1
#define POINT_JOUEUR_DROITE 2
#define REBOND_MUR 3
#define REBOND_JOUEUR_DROITE 4
#define REBOND_JOUEUR_GAUCHE 5
#define FIN_PARTIE 6

// la direction de la balle 1=NE 2=E 3=SE 4=SO 5=O 6=NO
byte directionBalle;
#define DIR_NE 1
#define DIR_E  2
#define DIR_SE 3
#define DIR_SO 4
#define DIR_O  5
#define DIR_NO 6

// la vitesse (en nombre de pixels par cycle)
byte vitesseBalle;

// le score
byte scoreDroite;
byte scoreGauche;

// la balle est remise au centre pour un nouvel échange
void nouvelEchange() {
  
  // initialiser la position de la balle
  xBalle = LARGEUR_ECRAN / 2 - EPAISSEUR_BALLE / 2 - 5;
  yBalle = HAUTEUR_ECRAN / 2 - EPAISSEUR_BALLE / 2;

  // les raquettes reprennent leur taille initiale
  longueurRaquette = LONGUEUR_RAQUETTE;

  // direction de la balle aléatoire 
  directionBalle = (millis() % 5) + 1 ;
}

// Une partie commence
void nouvellePartie() {

  // initialiser les  positions des raquettes (au milieu)
  yRaquetteDroite = (HAUTEUR_ECRAN / 2) - (longueurRaquette / 2);
  yRaquetteGauche = yRaquetteDroite;

  // remise à zéro des scores
  scoreDroite=0;
  scoreGauche=0;

  // On commence un nouvel échange
  nouvelEchange();
}

// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(1);
  lcdBegin();
  setContrast(60);

  if (toucheDroite()) {
    // un appui sur droite au démarrage lance le mode cauchemard => vitesse doublée!!!
    vitesseBalle = 2;
  } else {
    vitesseBalle = 1;
  }

  // remise à zero de la partie
  nouvellePartie();
}

// Gestion des graphismes
void afficheJeu(){

  clearDisplay(STYLEBG);

  // On dessine la raquette droite
  creerRectangle(X_RAQUETTE_DROITE, yRaquetteDroite, X_RAQUETTE_DROITE + EPAISSEUR_RAQUETTE, yRaquetteDroite + longueurRaquette, true, STYLEFG);

  // On dessine la raquette gauche
  creerRectangle(X_RAQUETTE_GAUCHE, yRaquetteGauche, X_RAQUETTE_GAUCHE - EPAISSEUR_RAQUETTE, yRaquetteGauche + longueurRaquette, true, STYLEFG);

  // On dessine la balle
  creerRectangle(xBalle,yBalle,xBalle+EPAISSEUR_BALLE,yBalle+EPAISSEUR_BALLE,false,STYLEFG);
  
  // On dessine le filet
  ligneEcran(LARGEUR_ECRAN / 2, 0, LARGEUR_ECRAN / 2, HAUTEUR_ECRAN, STYLEFG);

  // On affiche le score
  setChar(48+scoreGauche,X_SCORE_GAUCHE ,Y_SCORE, STYLEFG);
  setChar(48+scoreDroite,X_SCORE_DROITE ,Y_SCORE, STYLEFG);
  
  updateDisplay();
}

// Gestion des appuis touches
void gestionTouches(){

  // touche Haut joueur gauche
  if (toucheHaut() && (yRaquetteGauche > 0)) {
    yRaquetteGauche--;
  }

  // touche bas joueur gauche
  if (toucheBas() && (yRaquetteGauche + longueurRaquette < HAUTEUR_ECRAN)) {
    yRaquetteGauche++;
  }
  
  // touche Haut joueur droite
  if (toucheA() && (yRaquetteDroite > 0)) {
    yRaquetteDroite--;
  }

  // touche bas joueur droite
  if (toucheB() && (yRaquetteDroite + longueurRaquette < HAUTEUR_ECRAN)) {
    yRaquetteDroite++;
  }
}

// gestion de la logique du jeu
void logiqueJeu() {

  // Est-ce qu'il y a un rebond sur la raquette droite
  if ( X_RAQUETTE_DROITE >= xBalle &&
       ((X_RAQUETTE_DROITE - xBalle) < vitesseBalle) &&
       (yRaquetteDroite <= yBalle) &&
       (yRaquetteDroite + longueurRaquette >= yBalle) ) {
    evenement = REBOND_JOUEUR_DROITE;
    if (directionBalle == DIR_NE) {
      directionBalle = DIR_NO;   
    } else {
      directionBalle = DIR_SO;
    }

    // on diminue la taille des raquettes quand le joueur droite touche la balle pour rendre le jeu plus difficile!
    longueurRaquette = max(1,longueurRaquette--);
  }

  // Est-ce qu'il y a un rebond sur la raquette gauche 
  if ( xBalle >= X_RAQUETTE_GAUCHE &&
       ((xBalle - X_RAQUETTE_GAUCHE) < vitesseBalle) &&
       (yRaquetteGauche <= yBalle) &&
       (yRaquetteGauche + longueurRaquette >= yBalle) ) {
    evenement = REBOND_JOUEUR_GAUCHE;
    if (directionBalle == DIR_SO) {
      directionBalle = DIR_SE;   
    } else {
      directionBalle = DIR_NE;
    }
  }

  // la balle bouge
  // déplacement horizontal
  switch (directionBalle) {
    case DIR_NE:
    case DIR_E:
    case DIR_SE:
      xBalle = xBalle + vitesseBalle;
      break;
    case DIR_SO:
    case DIR_O:
    case DIR_NO:
      xBalle = xBalle - vitesseBalle;
      break;
    default:
      break;
  }

  //déplacement vertical
  switch (directionBalle) {
    case DIR_NE:
    case DIR_NO:
      yBalle = yBalle - vitesseBalle;
      break;
    case DIR_SE:
    case DIR_SO:
      yBalle = yBalle + vitesseBalle;
      break;
    default:
      break;
  }

  // gestion des sorties de l'écran
  if (xBalle < 0) {
    evenement = POINT_JOUEUR_DROITE;
    scoreDroite = scoreDroite + 1;
  }

  if (xBalle > LARGEUR_ECRAN) {
    evenement = POINT_JOUEUR_GAUCHE;
    scoreGauche = scoreGauche + 1;
  }

  if (yBalle < 0) {
    yBalle = - yBalle;
    if (directionBalle == DIR_NE) {
      directionBalle = DIR_SE;
    } else {
      directionBalle = DIR_SO;
    }
    evenement = REBOND_MUR;
  }

  if (yBalle > HAUTEUR_ECRAN) {
    yBalle = 2*HAUTEUR_ECRAN - yBalle;
    if (directionBalle == DIR_SE) {
      directionBalle = DIR_NE;
    } else {
      directionBalle = DIR_NO;
    }
    evenement = REBOND_MUR;
  }

  if ( scoreDroite == 9 || scoreGauche == 9) {
    evenement = FIN_PARTIE;
  }
}

void sonFinPartie() {
  frequenceBuzzer(1000,100);
  frequenceBuzzer(1000,100);
  frequenceBuzzer(1000,300);
  frequenceBuzzer(2000,1000);
}

void sonJeu() {
  switch (evenement) {
    case REBOND_MUR:
      frequenceBuzzer(2000,DUREE_SON);
      break;
    case REBOND_JOUEUR_DROITE:
      frequenceBuzzer(1000,DUREE_SON);
      break;
    case REBOND_JOUEUR_GAUCHE:
      frequenceBuzzer(1200,DUREE_SON);
      
      break;
    case POINT_JOUEUR_DROITE:
      frequenceBuzzer(2500,DUREE_SON);
      nouvelEchange();
      break;
    case POINT_JOUEUR_GAUCHE:
      frequenceBuzzer(3000,DUREE_SON);
      nouvelEchange();
      break;
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

