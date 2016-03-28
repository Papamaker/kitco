// Le programme du jeu PAC-ROB
// auteur: Francois JACOB
// jeu éducatif d'apprentissage de la programmation sur la console a 5 euros

#include "board.h"

// Les différents états
#define ETAT_INTRO 0
#define ETAT_AFFICHE_NIVEAU   1
#define ETAT_SAISIE_PROGRAMME 2
#define ETAT_RUN_PROGRAMME    3
#define ETAT_AFFICHE_AIDE     4

#define ETAT_TEST 99
byte etat = ETAT_TEST;

#define MUR_GAUCHE   1
#define MUR_DROITE   2
#define MUR_HAUT     4
#define MUR_BAS      8
#define FANTOME      16
#define VITAMINE     32

// Le numéro du niveau en cours
byte niveau = 0;


// Le programme fait par le joueur
char programme[100];

static const unsigned char PROGMEM pacman[] =
{ B00111100,
  B01111110,
  B11011100,
  B11111000,
  B11111000,
  B11111100,
  B01111110,
  B00111100};

static const unsigned char PROGMEM clef[] =
{ B00000000,
  B00001111,
  B00001110,
  B00001111,
  B00001100,
  B00111100,
  B00111100,
  B00111100};


void setup() {
  initBoard();  
}

void etatIntro() {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  for (char i=0;i<20;i++) { 
    display.clearDisplay();
    display.setCursor(3,i);
    display.println("  !PAC-ROB!");
    display.display();
    delay(50);
    
  }
  for (char i=0;i<10;i++) {
    display.drawRect(0+i,0+i,display.width()-2*i,display.height()-2*i,BLACK);
    display.display();
    delay(50);
    
  }
  delay(2000);
  attendRelache();
  display.clearDisplay();
  display.println("Pac-Rob est  un robot qui s'est perdu  dans un      labyrinthe...");
  display.display();
  attendTouche();
  attendRelache();
  display.clearDisplay();
  display.println(String("Envoie lui unprogramme    pour l'aider ")+AGRAVE+" s'"+EAIGU+"chapper ...");
  display.display();
  attendTouche();  
}

void etatTest() {
  display.clearDisplay();
  display.display();
  byte niv[12]={4,2,0,7,
  MUR_GAUCHE|MUR_HAUT|MUR_BAS,
  MUR_HAUT|MUR_BAS,
  MUR_HAUT|MUR_BAS,
  MUR_HAUT|MUR_BAS|MUR_DROITE,
  MUR_GAUCHE|MUR_HAUT|MUR_BAS,
  MUR_HAUT|MUR_BAS,
  MUR_HAUT|MUR_BAS,
  MUR_HAUT|MUR_BAS|MUR_DROITE};
  dessineNiveau(niv);
  delay(20000);
}


// Pour dessiner le niveau passé en paramètre
void dessineNiveau(byte *niveau) {
  byte longueur = niveau[0];
  byte hauteur  = niveau[1];
  byte xDebut   = niveau[2]%longueur;
  byte yDebut   = niveau[2]/longueur; 
  byte xFin     = niveau[3]%longueur;
  byte yFin     = niveau[3]/longueur;

  byte decalx=(84-(longueur*12))/2;
  byte decaly=(36-(hauteur*12))/2;
  
  display.clearDisplay();
  
  // un niveau est de 3 lignes (12 pixels par case) sur 7 lignes (une ligne pour écrire le programme)
  for (byte l=0;l<hauteur;l++) {
    for (byte c=0;c<longueur;c++) {
      byte index = l*longueur+c+4;
      if (niveau[index] & MUR_GAUCHE) {
        display.drawLine(decalx+c*12,decaly+l*12,decalx+c*12,decaly+(l+1)*12,BLACK);
      }
      if (niveau[index] & MUR_DROITE) {
        display.drawLine(decalx+(c+1)*12,decaly+l*12,decalx+(c+1)*12,decaly+(l+1)*12,BLACK);
      }
      if (niveau[index] & MUR_HAUT) {
        display.drawLine(decalx+c*12,decaly+(l+1)*12,decalx+(c+1)*12,decaly+(l+1)*12,BLACK);
      }
      if (niveau[index] & MUR_BAS) {
        display.drawLine(decalx+c*12,decaly+l*12,decalx+(c+1)*12,decaly+l*12,BLACK);
      }
      if (((c!=xDebut) || (l !=yDebut)) && ((c!=xFin) || (l!=yFin))) {
        display.drawRect(decalx+c*12+5,decaly+l*12+5,2,2,BLACK);
      }
    }
  }

  // on dessine le pacman au départ
  display.drawBitmap(decalx+2+xDebut*12,decaly+2+yDebut*12,pacman,8,8,1);

  // on dessine la clé à l'arrivée 
  display.drawBitmap(decalx+2+xFin*12,decaly+2+yFin*12,clef,8,8,1);

  

  display.display();
}





void loop() {
  switch (etat) {
    case ETAT_INTRO:
      etatIntro();
      break;
    case ETAT_TEST:
      etatTest();
      break;
    default: break;
  }
}
