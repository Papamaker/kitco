// Programme CFG.HEX
// menu principal accessible à l'allumage de la console (laisser appuyé sur BAS en même temps que POWER)
// 
// - réglage contraste
// - réglage rétro-éclairage
// - choix du programme à téléchager : et flash de celui-ci

// ajouter LED allumée quand lecture SD 
// stocker valeurs contraste + LED en EEPROM
// gérer l'appel multiple à printDirectory et menu chargement



// pour l'API kitco
#include "kitco.h"

#include <SPI.h>
#include <SD.h>
//#include "SdFat.h"
//SdFat SD;


File root;
char fichiers[6][8]; // liste static des 100 max fichiers .HEX sur la carte
byte nbFichiers = 0;
byte page = 0; // page pour l'affichage des fichiers

#define load_game (*((void(*)(const char* filename))(0x7ffc/2)))

#define MENU_PRINCIPAL 0
#define MENU_CONTRASTE 1
#define MENU_LUMIERE   2
#define MENU_APPLIS    3
#define MENU_INFOS     3

byte etat  = MENU_PRINCIPAL;
byte index = 0;


void printDirectory() {

  LEDVerte(ALLUME);

  // pour etre bien au début même pour les appels suivants
  nbFichiers = 0;
  
  // Allumer la LED verte pour montrer que l'on lit les données
  byte ind=0;
  while (true) {

    File entry =  root.openNextFile();

    if (! entry) {
      // no more files
      root.rewindDirectory();
      debug("No more files");
      break;
    }

    debug("checking File:");
    debug(entry.name());
      
    if (entry.isDirectory()) {
    } else {
      byte longueur=0;
      //char * strFichier = entry.name();
      while (entry.name()[longueur]!=0){
        longueur++; // calcul de la longueur du fichier
      }
      
      if (longueur>=5) { // Assez pour avoir une extension
        if ((entry.name()[longueur-4] == '.') &&
            (entry.name()[longueur-3] == 'H') &&
            (entry.name()[longueur-2] == 'E') &&
            (entry.name()[longueur-1] == 'X')) {
          if ( (nbFichiers >= page*6 && 
            nbFichiers < (page+1)*6) ) {
              for (byte k = 0;k < longueur - 4;k++) {
                fichiers[nbFichiers][k] = entry.name()[k];    
              }
            }
            fichiers[nbFichiers][longueur-4] = 0;
            nbFichiers++;
        }
      }
    }
    entry.close();
  }

  //éteindre la LED verte

  LEDVerte(ETEINTE);
}




void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  debug("Initializing SD card...");
  
  if (!SD.begin(A1)) {
    effacerEcran(BLANC);
    ecrirEcran("Erreur SD",3,20,BLANC);
    rafraichirEcran();  
    delay(3000);
    index = 0;
    etat = MENU_PRINCIPAL;  
    return;
  }
  
  debug("initialization done.");
  root = SD.open("/");

  //printDirectory();
  //delay(5000);
  //page=1;
  // root = SD.open("/");

  //printDirectory();
  
  //delay(5000);
  //page=1;
  //printDirectory();
  
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
  ecrirEcran("Eclairage",3,8,index==1?cligno:NOIR);
  ecrirEcran("Changer prog.",3,16,index==2?cligno:NOIR);
  ecrirEcran("A propos",3,24,index==3?cligno:NOIR);
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
    if (!(index == 0 || index == 1 || index == 2)) {
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
      if ((index % 6) == 5) {
        page ++ ;
        printDirectory();
      }
      index = index + 1;
      frequenceBuzzer(2500,100);
    }

    //si touche Haut
    if (toucheHaut() && index > 0) {
      if ((index % 6) == 0) {
        page -- ;
        printDirectory();
      }
      index = index - 1;
      frequenceBuzzer(2500,100);      
    }
    
    // si touche A
    if (toucheA()) {
      return index;
    }


    // si touche B
    if (toucheB()) {
      return 0xFF;
    }
  }
    
}

void menuApplis() {


  printDirectory();

  debug("done!");

  byte choix=menu(fichiers,nbFichiers);
  if ( choix == 0xFF ) {
      index = 0;
      etat = MENU_PRINCIPAL;  
  } else {
      effacerEcran(BLANC);
      ecrirEcran("Chargement de",3,0,NOIR);
      ecrirEcran(fichiers[choix],3,8,NOIR);
      ecrirEcran("Patientez...",3,16,NOIR);
      rafraichirEcran();
      load_game(fichiers[choix]);
      //delay(3000);
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
