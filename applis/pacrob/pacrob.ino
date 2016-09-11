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
#define ETAT_AIDE_IDE         7
#define ETAT_AIDE_NIVEAU      8

#define ETAT_TEST 99
byte etat = ETAT_INTRO;

#define PACDROITE 0
#define PACHAUT   1
#define PACGAUCHE 2
#define PACBAS    3
#define PACDEAD   4
byte pacEtat = PACDROITE;
// position de pacrob
byte posPac  = 0;

// Le numéro du niveau en cours
byte niveau = 0;

// la ligne sélectionnée dans l'ide
byte curseurIde = 0;

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
  haut2,
  bas2,
  droite2,
  gauche2
};

// les textes des commandes
char* commandes[5] = {
  "haut",
  "bas",
  "droite",
  "gauche"
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
  initBoard(1); 
  niv = lesNivs[niveau];
  posPac = niv[2];
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
  // si on n'est pas arrivé à la fin: perdu
  if (posPac!=niv[3]) {
    delay(500);
    crash();
    check();
  }
}

void crash() {
  pacEtat = PACDEAD;
}


void check() {
  tone(BUZZER,1000,50); 
  delay(50);
  noTone(BUZZER); 

  // si le pac-rob est sur la case coffre: on a gagné!
  if (posPac == niv[3]) {
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
  byte xPac     = posPac % longueur;
  byte yPac     = posPac / longueur;
  byte indexPac = posPac + 4;

  pacEtat = PACHAUT;
  if (!(niv[indexPac] & MH)) { // pas de mur: on peut avancer
     yPac--;
  } else {
    crash();
  }
  posPac = yPac*longueur + xPac;
  check();
  
}

// on fait avancer le pacrob
void instrBas() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = posPac % longueur;
  byte yPac     = posPac / longueur;
  byte indexPac = posPac + 4;

  pacEtat = PACBAS;
  if (!(niv[indexPac] & MB)) { // pas de mur: on peut avancer
     yPac++;
  } else {
    crash();
  }
  posPac = yPac*longueur + xPac;
  check();
}

// le pac va a gauche
void instrGauche() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = posPac % longueur;
  byte yPac     = posPac / longueur;
  byte indexPac = posPac + 4;

  pacEtat = PACGAUCHE;
  if (!(niv[indexPac] & MG)) { // pas de mur: on peut avancer
     xPac--;
  } else {
    crash();
  }
  posPac = yPac*longueur + xPac;
  check();
}

// le pac va a droite
void instrDroite() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = posPac % longueur;
  byte yPac     = posPac / longueur;
  byte indexPac = posPac + 4;

  pacEtat = PACDROITE;
  if (!(niv[indexPac] & MD)) { // pas de mur: on peut avancer
     xPac++;
  } else {
    crash();
  }
  posPac = yPac*longueur + xPac;
  check();
}

// Pour dessiner le niveau 
void dessineNiveau() {
  byte longueur = niv[0];
  byte hauteur  = niv[1];
  byte xPac     = posPac%longueur;
  byte yPac     = posPac/longueur; 
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
    afficheCodeProg(progIndex);
  

  display.display();
}

void initNiveau() {
  posPac=14;
  pacEtat = PACDROITE;
  progIndex = 0;
  dessineNiveau();

}

// affiche le prog à partir de la ligne pointeurProg
void afficheCodeProg(byte pointeurProg) {
    byte decalCmdes = 0;
  byte decal = 9;
  if (tailleProg>9) {
    decalCmdes = 4;  
    decal = 4;
  } 
  for (int i=pointeurProg;i<tailleProg;i++) {
      display.drawBitmap(i*decal,38,cmdes[prog[i]+decalCmdes],8,8,1);
  }

}

void etatIntro() {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  for (char i=0;i<10;i++) { 
    display.clearDisplay();
    display.setCursor(1,i);
    display.println("  PAC-ROB");
    display.println(" dedicated by");
    display.println("   !LDA-IT!");
    display.display();
    delay(50);
    
  }
  for (char i=0;i<3;i++) {
    display.drawRect(0+i,0+i,display.width()-2*i,display.height()-2*i,BLACK);
    display.display();
    delay(50);
    
  }
  delay(2000);
  attendRelache();
  display.clearDisplay();
  display.println("PAC-ROB est  un robot qui s'est perdu  chez LDA...");
  display.display();
  attendTouche();
  attendRelache();
  display.clearDisplay();
  display.println(String("Envoie lui unprogramme    pour l'aider ")+AGRAVE+" s'"+EAIGU+"chapper ...");
  display.display();
  attendTouche();  
  etat = ETAT_AFFICHE_NIVEAU;
}

void etatTest() {
  posPac=14;
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
  
  for (byte i = indexIde-curseurIde;i<min(indexIde-curseurIde+4,sizeof(cmdes)/sizeof(const byte*));i++) {
    display.setCursor(0,8*(i-indexIde+curseurIde));
    if (i == indexIde) {
      display.setTextColor(WHITE,BLACK);
    } else {
      display.setTextColor(BLACK,WHITE);
    }
    display.print(commandes[i]);
  }
      display.setTextColor(BLACK,WHITE);

  // 2 - afficher le code du programme
  afficheCodeProg(0);
  
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
// B: retirer la derniere instruction
// select: Help
void etatIde() {
  afficheIde(); 
  attendTouche();
  if (toucheBas() && (indexIde+1<sizeof(cmdes)/sizeof(const byte*))) {
    if (curseurIde<3) {
      curseurIde++;
    }
    indexIde = indexIde + 1;    
  }
  if (toucheHaut() && (indexIde > 0)) {
    if (curseurIde>0) {
      curseurIde--;
    }
    indexIde = indexIde - 1; 
  }
  if (toucheA()) {
    prog[tailleProg++] = indexIde;
  }
  if (toucheStart() && (tailleProg>0)) {
    tailleProg--;
  }
  if (toucheSelect()) {
    etat = ETAT_AIDE_IDE;
  }
  if (toucheB()) {
    etat = ETAT_AFFICHE_NIVEAU;
  }
  attendRelache();
}

void etatAfficheNiv() {
  dessineNiveau();
  attendRelache();
  attendTouche();
  if (toucheA()) {
    attendRelache();    
    lancer();
  } else if (toucheStart()||toucheB()) {
    attendRelache();
    etat = ETAT_IDE;
  } else if (toucheSelect()) {
    etat = ETAT_AIDE_NIVEAU;
  }
  
}

void etatGagne() {
      tailleProg = 0;
      progIndex = 0;
      niveau++;
      niv = lesNivs[niveau];
      posPac = niv[2];
      setLEDVerte3(HIGH);
      tone(BUZZER,1000,300); 
      delay(300);
      noTone(BUZZER); 
      tone(BUZZER,3000,700); 
      delay(700);
      setLEDVerte3(LOW);
      pacEtat = PACDROITE;
      etat = ETAT_AFFICHE_NIVEAU;
}


void etatPerdu() {
      tailleProg = 0;
      progIndex = 0;
      setLEDRouge4(HIGH);
      tone(BUZZER,500,300); 
      delay(300);
      noTone(BUZZER); 
      tone(BUZZER,200,700); 
      delay(700);
      setLEDRouge4(LOW);
      pacEtat = PACDROITE;
      posPac = niv[2];
      etat = ETAT_AFFICHE_NIVEAU;
}

void etatAideIde() {
  attendRelache();
  display.clearDisplay();
  display.println("A:ajout cmde");
  display.println("B:voir niveau");
  display.println("start: suppr.");
  display.println("select:aide");
  display.display();
  attendTouche();
  attendRelache();
  etat = ETAT_IDE;
}

void etatAideNiveau() {
  attendRelache();
  display.clearDisplay();
  display.println("A:lancer");
  display.println("B:retour");
  display.println("start:retour");
  display.println("select:aide");
  display.display();
  attendTouche();
  attendRelache();
  etat = ETAT_AFFICHE_NIVEAU;
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
      break;
    case ETAT_PERDU:
      etatPerdu();
      break;
    case ETAT_AIDE_IDE:
      etatAideIde();
      break;
    case ETAT_AIDE_NIVEAU:
      etatAideNiveau();
      break;
    default: 
      break;
  }
}
