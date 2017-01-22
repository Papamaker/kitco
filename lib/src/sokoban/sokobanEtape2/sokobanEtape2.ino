// Tutoriel pour le jeu Sokoban - Etape 1 : gestion des graphismes 

// Nécessaire pour l'environnement Kitco
#include "kitco.h"

#define NB_LIGNES_NIVEAUX 8
#define NB_COLONNES_NIVEAUX 4

// la position initiale du joueur (dépend du niveau)
int ligneJoueur   = 1;
int colonneJoueur = 1;

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


//
// LES GRAPHISMES
//

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

//
// LES CONTROLES
//
int relacheTouche = 0;

#define HAUT   1
#define BAS    2
#define DROITE 3
#define GAUCHE 4 

// fonction ligneSuivante
int ls(int direction, int nb) {
  switch (direction) {
    case HAUT:
      return ligneJoueur-nb;
      break;
    case BAS:
      return ligneJoueur+nb;
      break;
    default:
      return ligneJoueur;
      break;
  }
}

// fonction colonneSuivante
int cs(int direction, int nb) {
  switch (direction) {
    case DROITE:
      return colonneJoueur + nb;
      break;
    case GAUCHE:
      return colonneJoueur -nb;
      break;
    default:
      return colonneJoueur;
      break;
  }
}

// racourci case suivante:
#define SUIVANTE(DIR,NB) niveau[ls(DIR,NB)][cs(DIR,NB)]

void deplace(int dir) {

  // cas 1
  if (SUIVANTE(dir,1) == 'M') {
    return;
  }

  //cas 2
  if ( (SUIVANTE(dir,1) == '$' || SUIVANTE(dir,1) == '*') &&
       (SUIVANTE(dir,2) == '$' || SUIVANTE(dir,2) == '*')){
   return;
  }

  // cas 3
  if ( (SUIVANTE(dir,1) == '$' || SUIVANTE(dir,1) == '*') &&
       (SUIVANTE(dir,2) == 'M')) {
    return;
  }

  // à ce point du programme, on sait que le déplacement est autorisé  
  // Si la case du dessus contient une caisse on la déplace
  if (SUIVANTE(dir,1) == '$' || SUIVANTE(dir,1) == '*') {
    if (SUIVANTE(dir,2) == '.') {
      SUIVANTE(dir,2) = '*'; } else {
      SUIVANTE(dir,2) = '$';
    }
    if (SUIVANTE(dir,1) == '$') {
      SUIVANTE(dir,1) = ' '; } else {
      SUIVANTE(dir,1) = '.';
    }
  }

  // On déplace le joueur
  if (SUIVANTE(dir,1) == '.') {
   SUIVANTE(dir,1) = '+'; } else {
   SUIVANTE(dir,1) = '@'; 
  }

  // Le joueur n'est plus à la cas précédente
  if (niveau[ligneJoueur][colonneJoueur] == '+') {
    niveau[ligneJoueur][colonneJoueur] = '.'; } else {
    niveau[ligneJoueur][colonneJoueur] = ' '; 
  }

  ligneJoueur = ls(dir,1);
  colonneJoueur = cs(dir,1);
}

void surveilleTouches() {
  if (toucheHaut() && relacheTouche) {
    deplace(HAUT);
  }
  if (toucheBas() && relacheTouche) {
    deplace(BAS);
  }
  if (toucheDroite() && relacheTouche) {
    deplace(DROITE);
  }
  if (toucheGauche() && relacheTouche) {
    deplace(GAUCHE);
  }
  if (touche()) {
    relacheTouche = 0;
  } else {
    relacheTouche = 1;
  }
}



// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {

  afficheGraphismes();
  // joueBruitages();
   surveilleTouches();
  // logiqueJeu();

}
