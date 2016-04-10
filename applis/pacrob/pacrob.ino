// Le programme du jeu PAC-ROB
// auteur: Francois JACOB
// jeu éducatif d'apprentissage de la programmation sur la console a 5 euros

#include "board.h"
#include "sprites.h"
#include "niveaux.h"
//
// Les différents états
//
#define ETAT_INTRO 0
#define ETAT_AFFICHE_NIVEAU   1
#define ETAT_IDE              2
#define ETAT_RUN_PROGRAMME    3
#define ETAT_AFFICHE_AIDE     4
#define ETAT_GAGNE            5
#define ETAT_PERDU            6


#define ETAT_TEST 99
byte etat = ETAT_IDE;

#define PACDROITE 0
#define PACHAUT   1
#define PACGAUCHE 2
#define PACBAS    3
#define PACDEAD   4
byte pacEtat = PACDROITE;

// Le numéro du niveau en cours
byte niveau = 0;

//
// Les instructions du programme
//
#define HAU 0
#define BAS 1
#define DRO 2
#define GAU 3
#define SID 4
#define SIH 5
#define SIG 6
#define SIB 7

byte indexIde = 0;

// les dessins des commandes
const byte* cmdes[]= { 
  haut,
  bas,
  droite,
  gauche,
  pacdroite,
  pachaut,
  pacgauche,
  pacbas
};

// les textes des commandes
char* commandes[8] = {
  "haut",
  "bas",
  "droite",
  "gauche",
  "touche d",
  "touche h",
  "touche g",
  "touche b" 
};

byte tailleProg = 0;

// Le programme fait par le joueur
byte prog[100]; 

byte progIndex = 0;

// Le bmp du pacrob
const byte *pacrob;

// Le niveau en cours (tableau)
byte* niv;

void setup() {
  Serial.println(freeRam());
  initBoard(0); 
  niv = lesNivs[niveau];
}

// va executer le programme prog
void lancer() {
  Serial.println(freeRam());
  setLEDBleue1(HIGH);
  progIndex = 0;
  while (progIndex != tailleProg) {
    delay(1000);
    switch (prog[progIndex++]) {
      case HAU:
         instrHaut();
       break;
      case BAS:
        instrBas();
        break;
      case DRO:
        instrDroite();
        break;
      case GAU:
         instrGauche();
      default:
      break;
    }
  }
    setLEDBleue1(LOW);
  Serial.println(freeRam());
}

void crash() {
  pacEtat = PACDEAD;
}


void check() {
  tone(BUZZER,1000,50); 
  delay(50);
  noTone(BUZZER); 

  // si le pac-rob est sur la case coffre: on a gagné!
  if (niv[2] == niv[3]) {
   etat = ETAT_GAGNE;
  }
  
  if (pacEtat == PACDEAD) {
    tone(BUZZER,200,200);
    delay(200);
    noTone(BUZZER); 
    etat = ETAT_PERDU;
  }
  dessineNiveau();
}

// on fait avancer le pacrob
void instrHaut() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = niv[2] % longueur;
  byte yPac     = niv[2] / longueur;
  byte indexPac = niv[2] + 4;

  pacEtat = PACHAUT;
  if (!(niv[indexPac] & MH)) { // pas de mur: on peut avancer
     yPac--;
  } else {
    crash();
  }
  niv[2] = yPac*longueur + xPac;
  check();
  
}

// on fait avancer le pacrob
void instrBas() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = niv[2] % longueur;
  byte yPac     = niv[2] / longueur;
  byte indexPac = niv[2] + 4;

  pacEtat = PACBAS;
  if (!(niv[indexPac] & MB)) { // pas de mur: on peut avancer
     yPac++;
  } else {
    crash();
  }
  niv[2] = yPac*longueur + xPac;
  check();
}

// le pac va a gauche
void instrGauche() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = niv[2] % longueur;
  byte yPac     = niv[2] / longueur;
  byte indexPac = niv[2] + 4;

  pacEtat = PACGAUCHE;
  if (!(niv[indexPac] & MG)) { // pas de mur: on peut avancer
     xPac--;
  } else {
    crash();
  }
  niv[2] = yPac*longueur + xPac;
  check();
}

// le pac va a droite
void instrDroite() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = niv[2] % longueur;
  byte yPac     = niv[2] / longueur;
  byte indexPac = niv[2] + 4;

  pacEtat = PACDROITE;
  if (!(niv[indexPac] & MD)) { // pas de mur: on peut avancer
     xPac++;
  } else {
    crash();
  }
  niv[2] = yPac*longueur + xPac;
  check();
}

// Pour dessiner le niveau 
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
        display.drawLine(decalx+(c+1)*12,decaly+l*12,decalx+(c+1)*12,decaly+(l+1)*12,BLACK);

        // sinon ça déborde
        if (c == 6) {
          display.drawLine(decalx+(c+1)*12-1,decaly+l*12,decalx+(c+1)*12-1,decaly+(l+1)*12,BLACK);  
        }
      }
      if (niv[index] & MH) {
        display.drawLine(decalx+c*12,decaly+l*12,decalx+(c+1)*12,decaly+l*12,BLACK);
      }
      if (niv[index] & MB) {
        display.drawLine(decalx+c*12,decaly+(l+1)*12,decalx+(c+1)*12,decaly+(l+1)*12,BLACK);
      }
      if (((c!=xPac) || (l !=yPac)) && ((c!=xFin) || (l!=yFin)) && (niv[index]!=0)) {
        display.drawRect(decalx+c*12+5,decaly+l*12+5,2,2,BLACK);
      }
    }
  }


 switch (pacEtat) {
  case PACDROITE:
    pacrob = pacdroite;
    break;
  case PACHAUT:
    pacrob = pachaut;
    break;
  case PACGAUCHE:
    pacrob = pacgauche;
    break;
  case PACBAS:
    pacrob = pacbas;
    break;
  case PACDEAD:
    pacrob = dead;
    break;
  default:
    break;
 }

  // on dessine le pacman
  display.drawBitmap(decalx+2+xPac*12,decaly+2+yPac*12,pacrob,8,8,1);

  // on dessine la clé à l'arrivée
  if ((xPac != xFin) || (yPac != yFin)) {
    display.drawBitmap(decalx+2+xFin*12,decaly+2+yFin*12,coffre,8,8,1);
  }

  // 2 - afficher le code du programme
  for (int i=progIndex;i<tailleProg;i++) {
      display.drawBitmap(i*10,38,cmdes[prog[i]],8,8,1);
  }
  

  display.display();
}

void initNiveau() {
  niv[2]=14;
  pacEtat = PACDROITE;
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
  pacEtat = PACDROITE;
  
  display.clearDisplay();
  display.display();
  dessineNiveau();
  initNiveau();
  lancer();
  
  
  delay(500000);
}



void afficheIde() {

  // 1 - afficher le choix dans les commandes
  display.clearDisplay();
  for (byte i = indexIde;i<min(indexIde+4,sizeof(cmdes)/sizeof(const byte*));i++) {
    display.setCursor(0,8*(i-indexIde));
    if (i == indexIde) {
      display.setTextColor(WHITE,BLACK);
    } else {
      display.setTextColor(BLACK,WHITE);
    }
    display.print(commandes[i]);
  }

  // 2 - afficher le code du programme
  for (int i=0;i<tailleProg;i++) {
      display.drawBitmap(i*10,38,cmdes[prog[i]],8,8,1);
  }
  
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
  if (toucheBas() && (indexIde+1<sizeof(cmdes)/sizeof(const byte*))) {
    indexIde = indexIde + 1;    
  }
  if (toucheHaut() && (indexIde > 0)) {
    indexIde = indexIde - 1; 
  }
  if (toucheA()) {
    prog[tailleProg++] = indexIde;
  }
  if (toucheStart()) {
    etat = ETAT_AFFICHE_NIVEAU;
  }
  attendRelache();
}

void etatAfficheNiv() {
  dessineNiveau();
  attendRelache();
  attendTouche();
  if (!toucheSelect()) {
      attendRelache();
      etat = ETAT_IDE;
  } else {
    attendRelache();    
    lancer();
  }
}

void etatGagne() {
      tailleProg = 0;
      progIndex = 0;
      niveau++;
      niv = lesNivs[niveau];
      setLEDVerte3(HIGH);
      delay(1000);
      setLEDVerte3(LOW);
      pacEtat = PACDROITE;
      etat = ETAT_IDE;
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
    case ETAT_AFFICHE_NIVEAU:
      etatAfficheNiv();
      break;
    case ETAT_GAGNE:
      etatGagne();
    default: 
      break;
  }
}
