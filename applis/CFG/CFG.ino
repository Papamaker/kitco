// Programme CFG.HEX
// menu principal accessible à l'allumage de la console (laisser appuyé sur BAS en même temps que POWER)
// 
// - réglage contraste
// - réglage rétro-éclairage
// - choix du programme à téléchager : et flash de celui-ci

// pour l'API kitco
#include "/Users/francoisjacob/Dropbox/maker/github/macbookpro/consolea5euros/API/kitco.h"
//#include "C:\Users\jacobf\Documents\personnel\Dropbox\maker\github\macbookpro\consolea5euros\API\kitco.h"
#include <SD.h>

//#define load_game (*((void(*)(char*))0x7ffc))
#define load_game (*((void(*)(const char* filename))(0x7ffc/2)))

#define MENU_PRINCIPAL 0
#define MENU_CONTRASTE 1
#define MENU_LUMIERE 2
#define MENU_APPLIS 3
#define MENU_INFOS 3

byte etat = MENU_PRINCIPAL;
int index = 0;

void setup() {

  Serial.begin(9600);
  initialiserKitco(0);
  effacerEcran(BLANC);
  ecrirEcran("Configuration",3,20,NOIR);
  rafraichirEcran();
  delay(1000);


}

void menuPrincipal() { 
  effacerEcran(BLANC);
  byte cligno = (millis()/500)%2?BLANC:NOIR;
  ecrirEcran("Constraste",3,0,index==0?cligno:NOIR);
  ecrirEcran("LED Ecran",3,8,index==1?cligno:NOIR);
  ecrirEcran("Chargement",3,16,index==2?cligno:NOIR);
  ecrirEcran("Info",3,24,index==3?cligno:NOIR);
  rafraichirEcran();

  if (toucheHaut()) {
    if (index >0) { 
      index = index -1;
      frequenceBuzzer(2500,100);
    }
  }
  if (toucheBas()) {
    if (index<3) {
      index = index +1;
      frequenceBuzzer(2500,100);
    }
  }
  if (toucheA()) {
     etat = index +1;
  }
}

void menuContraste() {
      while (!toucheB()) {
          byte cligno = (millis()/500)%2?BLANC:NOIR;

        if (toucheDroite()) {
           if (contraste <127) contraste++;
        }

        if (toucheGauche()) {
          if (contraste > 0) contraste--;
        }
        contrasteEcran(contraste);
        effacerEcran(BLANC);
        ecrirEcran("-Constraste-",3,20,cligno);
        ligneEcran(0,30,contraste*LARGEUR_ECRAN/128,30,NOIR);
        rafraichirEcran();
    }
    etat = MENU_PRINCIPAL;
}

void menuLumiere() {
      while (!toucheB()) {
        byte cligno = (millis()/500)%2?BLANC:NOIR;
        if (toucheDroite()) {
           if (lumEcr <255) lumEcr++;
        }

        if (toucheGauche()) {
          if (lumEcr > 0) lumEcr--;
        }
        lumiereEcran(lumEcr);
        effacerEcran(BLANC);
        ecrirEcran("-Lumiere-",3,20,cligno);
        ligneEcran(0,30,lumEcr*LARGEUR_ECRAN/256,30,NOIR);
        rafraichirEcran();
    }
    etat = MENU_PRINCIPAL;
}


//fonction générale de menu (taille des éléments)
byte menu(char choix[][8],byte nbChoix) {

  byte index = 0;

  while (1) {

    // premier élément du tableau à afficher
    byte premier = 0;
    
    // afficher les choix
    if (!(index == 0 || index == 1 )) {
      premier = index - 3;
    }

    effacerEcran(BLANC);
    for (byte i = premier; i<min(premier+6,nbChoix); i++) {

      // si élément sélectionné => clignotant
      byte couleur = NOIR;
      if (i == index) {
        couleur = (millis()/500)%2?BLANC:NOIR;
      }

      const char *tmp = choix[i];
      ecrirEcran(tmp,3,8*(i-premier),couleur);
    }
    rafraichirEcran();

    // si touche Bas
    if (toucheBas() && index<(nbChoix-1)) {
      index = index +1;
      frequenceBuzzer(2500,100);
    }

    //si touche Haut
    if (toucheHaut() && index > 0) {
      index = index +1;
      frequenceBuzzer(2500,100);      
    }
    
    // si touche A
    if (toucheA()) {
      return index;
    }


    // si touche B
    if (toucheB()) {
      return -1;
    }
  }
    
}

void menuApplis() {
Sd2Card card;
SdVolume volume;
//SdFile root;

  debug("Initialisation carte SD...");
  // Lire la carte mémoire
  if (!card.init(SPI_HALF_SPEED, CS_SD)) {
      LEDRouge(ALLUME);
      effacerEcran(BLANC);
      ecrirEcran("Erreur SD",3,20,BLANC);
      rafraichirEcran();  
      delay(3000);
      index = 0;
      etat = MENU_PRINCIPAL;  
      return;
    }

    debug("Trouver le type de carte...");

    // print the type of card
    effacerEcran(BLANC);
    ecrirEcran("Type SD:",3,0,NOIR);

    switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      ecrirEcran("SD1",50,0,NOIR);
      break;
    case SD_CARD_TYPE_SD2:
      ecrirEcran("SD2",50,0,NOIR);
      break;
    case SD_CARD_TYPE_SDHC:
      ecrirEcran("SDHC",50,0,NOIR);
      break;
    default:
      ecrirEcran("??",50,0,NOIR);
      break;
  }
      rafraichirEcran();  
    
    debug("Volume.init...");
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
#if 1
if (!volume.init(card)) {
      LEDRouge(ALLUME);
      effacerEcran(BLANC);
      ecrirEcran("Erreur FAT",3,20,BLANC);
      rafraichirEcran();  
      delay(3000);
      index = 0;
      etat = MENU_PRINCIPAL;  
      return;
  }
#endif

    debug("SD begin");
    delay(2000);

  // afficher la liste des progs
  if (!SD.begin(A1)) {
      LEDRouge(ALLUME);
      effacerEcran(BLANC);
      ecrirEcran("Erreur SDBegin",3,20,BLANC);
      rafraichirEcran();  
      delay(3000);
      index = 0;
      etat = MENU_PRINCIPAL;  
      return;
  }
    debug("SD open");
    delay(2000);

  File root = SD.open("/");

    debug("opennextfile");
    delay(2000);
  // skipper les fichiers déjà lus
     // load_game("CFG");

  char fichiers[100][8]; // liste static des 100 max fichiers .HEX sur la carte
  int nbFichiers = 0;
    
  File entry = root.openNextFile();
  byte index=0;
  while (entry) { // Parcourir les fichiers à la racine
    byte longueur=0;
    char * strFichier = entry.name();
    while (strFichier[longueur]!=0) longueur++; // calcul de la longueur du fichier
      if (longueur>=5) { // Assez pour avoir une extension
        if ((strFichier[longueur-4] == '.') &&
            (strFichier[longueur-3] == 'H') &&
            (strFichier[longueur-2] == 'E') &&
            (strFichier[longueur-1] == 'X')) {
          for (byte k = 0;k < longueur - 4;k++) {
            fichiers[index][k] = strFichier[k];    
          }
            fichiers[index][longueur-4] = 0;
            nbFichiers++;
        }
      }
    }

  byte choix=menu(fichiers,nbFichiers);
  if ( choix == -1 ) {
      delay(3000);
      index = 0;
      etat = MENU_PRINCIPAL;  
  } else {
      effacerEcran(BLANC);
      ecrirEcran("Chargement de",3,0,NOIR);
      ecrirEcran(fichiers[choix],3,8,NOIR);
      ecrirEcran("Patientez...",3,16,NOIR);
      rafraichirEcran();
      // TODO FJA ajouter load_game(choix[i]);
      delay(3000);
  }
  

}

void loop() {

  // ETATS 
  switch (etat) {

  // MENU_PRINCIPAL
  case MENU_PRINCIPAL:
    menuPrincipal();
  break;
  
  // MENU CONTRAST
  case MENU_CONTRASTE:
    menuContraste();
  break;
  
  // MENU LUMIERE
  case MENU_LUMIERE:
    menuLumiere();
  break;
  
  // LISTE APPLIS
  case MENU_APPLIS:
    menuApplis();
  break; 

  default:
  break;
  }

}
