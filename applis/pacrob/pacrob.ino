// Le programme du jeu PAC-ROB
// auteur: Francois JACOB
// jeu éducatif d'apprentissage de la programmation sur la console a 5 euros

#include "board.h"

//
// Les différents états
//
#define ETAT_INTRO 0
#define ETAT_AFFICHE_NIVEAU   1
#define ETAT_IDE              2
#define ETAT_RUN_PROGRAMME    3
#define ETAT_AFFICHE_AIDE     4

#define ETAT_TEST 99
byte etat = ETAT_IDE;

//
// Les éléments de description du labyrinthe
//
// MG = mur gauche 
#define MG   1
// MD = mur droite
#define MD   2
// MH = mur haut
#define MH   4
// MB = mur bas
#define MB   8
// FA = fantome
#define FA  16
// VI = Vitamine
#define VI  32

#define DROITE 0
#define HAUT   1
#define GAUCHE 2
#define BAS    3
#define DEAD   4
byte pacEtat = DEAD;

// Le numéro du niveau en cours
byte niveau = 0;

//
// Les instructions du programme
//
#define AVA 0
#define TGA 1
#define TDR 2
#define REP 3
#define FIN 4
#define SID 5
#define SIH 6
#define SIG 7
#define SIB 8
#define END 9    

byte indexIde = 0;

String commandes[10] = {
  "Avance",
  "Tourne gauche",
  "Tourne droite",
  String("Rep")+EAIGU+"te",
  "fin",
  "Touche droite",
  "Touche haut",
  "Touche gauche",
  "Touche bas",
  "Fin prog"
};

char cmdTxt[10] = {
  'a',
  'g',
  'd',
  'R',
  'f',
  'D',
  'H',
  'G',
  'B',
  'F'
};

char txtPrg[34];

byte tailleProg = 0;

// Le programme fait par le joueur
byte prog[100] = {
  AVA,
  AVA,
  TGA,
  AVA,
  TDR,
  AVA,
  TDR,
  AVA,
  TGA,
  AVA,
  AVA,
  AVA,
  TGA,
  AVA,
  AVA,
  TGA,
  AVA,
  TGA,
  AVA,
  TDR,
  AVA,
  TDR,
  AVA,
  TGA,
  AVA,
  AVA,
  AVA,
  TGA,
  AVA,
  TDR,
  AVA,
  TDR,
  AVA,
  END
};

byte progIndex = 0;

// Le bmp du pacrob

const byte *pacrob;

static const byte PROGMEM pacdroite[] =
{ B00111100,
  B01111110,
  B11011100,
  B11111000,
  B11111000,
  B11111100,
  B01111110,
  B00111100};

static const byte PROGMEM pachaut[] =
{ B00000000,
  B01000010,
  B11100111,
  B11111111,
  B11111111,
  B11011111,
  B01111110,
  B00111100};

static const byte PROGMEM pacgauche[] =
{ B00111100,
  B01111110,
  B00111011,
  B00011111,
  B00011111,
  B00111111,
  B01111110,
  B00111100};

static const byte PROGMEM pacbas[] =
{ B00111100,
  B01111110,
  B11011111,
  B11111111,
  B11111111,
  B11100111,
  B01000010,
  B00000000};

static const byte PROGMEM dead[] =
{ B00000000,
  B00111110,
  B01111111,
  B01001001,
  B01111111,
  B01110111,
  B00111110,
  B00101010};



// Le bmp de la clé (but à atteindre)
static const byte PROGMEM clef[] =
{ B00000000,
  B00001111,
  B00001110,
  B00001111,
  B00001100,
  B00111100,
  B00111100,
  B00111100};

// Le niveau en cours
byte niv[25]={7,3,14,0,
  MG|MH|MD,
  MG|MH,
  MB|MH,
  MB|MH,
  MH|MD,
  MG|MH,
  MH|MD,
  
  MB|MG,
  MB|MD,
  MG|MH,
  MH|MD,
  MG|MB,
  MB|MD,
  MG|MD,
  
  MG|MH|MB,
  MH|MB,
  MB|MD,
  MB|MG,
  MH|MB,
  MH|MB,
  MB|MD
  };

// on convertit le programme en cours par une suite de char
// debut: pour forcer d'afficher la commande en cours sans décalage
void prog2txt(byte debut) {
  byte i = 0;
  
  for (i=0;i<7;i++) {
    if (progIndex+i+1-debut<tailleProg) {
      txtPrg[2*i]   = cmdTxt[prog[progIndex+i+1-debut]];
      if (txtPrg[2*i]!='F') {
        txtPrg[2*i+1] = ',';
      } else {
        txtPrg[2*i+1] = ' '; 
      }
    } else {
      txtPrg[2*i] = ' ';
      txtPrg[2*i+1] = ' ';
    }
  }
    
}


// va executer le programme prog
void lancer() {
  progIndex = 0;
  while (prog[progIndex] != END) {
    attendTouche();
    prog2txt(0);
    switch (prog[progIndex++]) {
      case AVA:
        avance();
        break;
      case TDR:
        tourneDroite();
        break;
      case TGA:
        tourneGauche();
        break;
      default:
      break;
    }
  }
}

void crash() {
  pacEtat = DEAD;
}


void setup() {
  initBoard(0);  
}

// on fait avancer le pacrob
void avance() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = niv[2] % longueur;
  byte yPac     = niv[2] / longueur;
  byte indexPac = niv[2] + 4;

  tone(BUZZER,1000,50);  

  switch (pacEtat) {
    case DROITE:
      if (!(niv[indexPac] & MD)) { // pas de mur: on peut avancer
         xPac++;
      } else {
        crash();
      }
    break;
    case HAUT:
      if (!(niv[indexPac] & MH)) { // pas de mur: on peut avancer
         yPac--;
      } else {
        crash();
      }
    break;
    case GAUCHE:
      if (!(niv[indexPac] & MG)) { // pas de mur: on peut avancer
         xPac--;
      } else {
        crash();
      }
    break;
    case BAS:
      if (!(niv[indexPac] & MB)) { // pas de mur: on peut avancer
         yPac++;
      } else {
        crash();
      }
    break;
  }
  
  niv[2] = yPac*longueur + xPac;
  if (pacEtat == DEAD) {
    tone(BUZZER,200,200);
  }
  dessineNiveau();
}

// le pacrob tourne à gauche
void tourneGauche() {
  
  switch (pacEtat) {
    case DROITE:
      pacEtat = HAUT;
    break;
    case HAUT:
      pacEtat = GAUCHE;
    break;
    case GAUCHE:
      pacEtat = BAS;
    break;
    case BAS:
      pacEtat = DROITE;
    break;
  }
  tone(BUZZER,2000,50);  
  dessineNiveau();
}


// le pacrob tourne à gauche
void tourneDroite() {
  switch (pacEtat) {
    case DROITE:
      pacEtat = BAS;
    break;
    case HAUT:
      pacEtat = DROITE;
    break;
    case GAUCHE:
      pacEtat = HAUT;
    break;
    case BAS:
      pacEtat = GAUCHE;
    break;
  }
  tone(BUZZER,2500,50);  
  dessineNiveau();
}


// Pour dessiner le niveau passé en paramètre
void dessineNiveau() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = niv[2]%longueur;
  byte yPac     = niv[2]/longueur; 
  byte xFin     = niv[3]%longueur;
  byte yFin     = niv[3]/longueur;

  byte decalx=(84-(longueur*12))/2;
  byte decaly=(36-(hauteur*12))/2;
  
  display.clearDisplay();
  
  // un niveau est de 3 lignes (12 pixels par case) sur 7 lignes (une ligne pour écrire le programme)
  for (byte l=0;l<hauteur;l++) {
    for (byte c=0;c<longueur;c++) {
      byte index = l*longueur+c+4;
      if (niv[index] & MG) {
        display.drawLine(decalx+c*12,decaly+l*12,decalx+c*12,decaly+(l+1)*12,BLACK);
      }
      if (niv[index] & MD) {
        display.drawLine(decalx+(c+1)*12-1,decaly+l*12,decalx+(c+1)*12-1,decaly+(l+1)*12,BLACK);
      }
      if (niv[index] & MH) {
        display.drawLine(decalx+c*12,decaly+l*12,decalx+(c+1)*12,decaly+l*12,BLACK);
      }
      if (niv[index] & MB) {
        display.drawLine(decalx+c*12,decaly+(l+1)*12,decalx+(c+1)*12,decaly+(l+1)*12,BLACK);
      }
      if (((c!=xPac) || (l !=yPac)) && ((c!=xFin) || (l!=yFin))) {
        display.drawRect(decalx+c*12+5,decaly+l*12+5,2,2,BLACK);
      }
    }
  }


 switch (pacEtat) {
  case DROITE:
    pacrob = pacdroite;
    break;
  case HAUT:
    pacrob = pachaut;
    break;
  case GAUCHE:
    pacrob = pacgauche;
    break;
  case BAS:
    pacrob = pacbas;
    break;
  case DEAD:
    pacrob = dead;
    break;
  default:
    break;
 }

  // on dessine le pacman
  display.drawBitmap(decalx+2+xPac*12,decaly+2+yPac*12,pacrob,8,8,1);

  // on dessine la clé à l'arrivée
  if ((xPac != xFin) || (yPac != yFin)) {
    display.drawBitmap(decalx+2+xFin*12,decaly+2+yFin*12,clef,8,8,1);
  }

  // on affiche le txt du programme en cours
  display.setCursor(0,38);
  display.setTextColor(WHITE,BLACK);
  display.print(txtPrg[0]);
  display.setTextColor(BLACK,WHITE);
  display.print(&txtPrg[1]);
  

  display.display();

  // TMP
  delay(500);
}

void initNiveau() {
  niv[2]=14;
  pacEtat = DROITE;
  prog2txt(1);
  progIndex = 0;
  dessineNiveau();

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
  niv[2]=14;
  pacEtat = DROITE;
  
  display.clearDisplay();
  display.display();
  dessineNiveau();
  avance();
  avance();
  avance();
  initNiveau();
  lancer();
  
  
  delay(500000);
}



void afficheIde() {

  // 1 - afficher le choix dans les commandes
  display.clearDisplay();
  for (byte i = indexIde;i<min(indexIde+4,sizeof(commandes)/sizeof(String));i++) {
    display.setCursor(0,8*(i-indexIde));
    if (i == indexIde) {
      display.setTextColor(WHITE,BLACK);
    } else {
      display.setTextColor(BLACK,WHITE);
    }
    display.print(commandes[i]);
  }

  // 2 - afficher le texte du programme
  // on affiche le txt du programme en cours
  display.setCursor(0,38);
  display.setTextColor(BLACK,WHITE);
  display.print(txtPrg);
  display.setTextColor(WHITE,BLACK);
  display.print("_");

  
  display.display();
}

// 
// L'affichage de l'IDE :
// la liste des commandes disponibles sur les 3 premières lignes et le programme sur la 4ième ligne
// appui start => affichage niveau
// Haut/Bas: se déplacer dans la liste des instructions disponibles
// Gauche: effacer la dernière instruction
// Droite: ???
// A: ajouter l'instruction
// B: ???
// select: Help
void etatIde() {
  afficheIde(); 
  attendTouche();
  if (toucheBas() && (indexIde+1<sizeof(commandes)/sizeof(String))) {
    indexIde = indexIde + 1;    
  }
  if (toucheHaut() && (indexIde > 0)) {
    indexIde = indexIde - 1; 
  }
  if (toucheA()) {
    prog[tailleProg++] = indexIde;
      prog2txt(1);
  }
  attendRelache();
}


void loop() {
  switch (etat) {
    case ETAT_INTRO:
      etatIntro();
      break;
    case ETAT_TEST:
      etatTest();
      break;
    case ETAT_IDE:
      etatIde();
      break;
    default: 
      break;
  }
}
