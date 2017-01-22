// Tutoriel pour le jeu Sokoban - Etape 1 : gestion des graphismes 

// Nécessaire pour l'environnement Kitco
#include "kitco.h"

#define NB_LIGNES_NIVEAUX 8
#define NB_COLONNES_NIVEAUX 4

char niveau[NB_LIGNES_NIVEAUX][NB_COLONNES_NIVEAUX] = 
{ {'M','M','M','M'},
  {'M','@','.','M'},
  {'M','$',' ','M'},
  {'M',' ',' ','M'},
  {'M',' ','$','M'},
  {'M','.',' ','M'},
  {'M',' ',' ','M'},
  {'M','M','M','M'}};

// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(0);

  lcdBegin();

  
}

void dessineMur(int ligne, int colonne) {
  creerRectangle(colonne*4,ligne*4,colonne*4+4,ligne*4+3,1,NOIR);
}
void dessineJoueur(int ligne, int colonne) {
  creerRectangle(colonne*4+1,ligne*4,colonne*4+2,ligne*4+3,1,NOIR);
  creerRectangle(colonne*4,ligne*4+1,colonne*4+3,ligne*4+2,1,NOIR);
  creerRectangle(colonne*4+1,ligne*4+1,colonne*4+2,ligne*4+2,1,BLANC);
}
void dessineJoueurDestination(int ligne, int colonne) {
  creerRectangle(colonne*4+1,ligne*4,colonne*4+2,ligne*4+3,1,NOIR);
  creerRectangle(colonne*4,ligne*4+1,colonne*4+3,ligne*4+2,1,NOIR);
}
void dessineCaisse(int ligne, int colonne) {
  creerRectangle(colonne*4,ligne*4,colonne*4+3,ligne*4+3,0,NOIR);
}
void dessineCaisseDestination(int ligne, int colonne) {
  creerRectangle(colonne*4,ligne*4,colonne*4+3,ligne*4+3,0,NOIR);
  setPixel(colonne*4+2,ligne*4+1,NOIR);
  setPixel(colonne*4+1,ligne*4+2,NOIR);
}
void dessineDestination(int ligne, int colonne) {
  creerRectangle(colonne*4+1,ligne*4+1,colonne*4+2,ligne*4+2,0,NOIR);
}

void afficheGraphismes() {

  // on efface l'écran
  effacerEcran(BLANC);

  // On affiche chacune des cases du niveau
  for (int ligne=0;ligne<NB_LIGNES_NIVEAUX;ligne++) {
    for (int colonne=0;colonne<NB_COLONNES_NIVEAUX;colonne++) {
       switch (niveau[ligne][colonne]) {
         case 'M': 
            dessineMur(ligne,colonne);
            break;
         case '@':
            dessineJoueur(ligne,colonne);
            break;
         case '+':
            dessineJoueurDestination(ligne,colonne);
            break;
         case '$':
            dessineCaisse(ligne,colonne);
            break;
         case '*':
            dessineCaisseDestination(ligne,colonne);
            break;
         case '.':
            dessineDestination(ligne,colonne);
            break;
         case ' ':
            // pour une case vide, on ne dessine rien
            break;
         default:
            break; 
       }
    }
  }
    // on affiche le resultat de nos commandes
    rafraichirEcran();
}

// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {

  afficheGraphismes();
  // joueBruitages();
  // surveilleTouches();
  // logiqueJeu();

}
