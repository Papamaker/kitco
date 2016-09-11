// Communication avec l'écran
#include <SPI.h>

// Bibliothèques graphiques
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Gestion du I2C pour le MCP23017
#include "Wire.h"

// Les branchements du microcontrolleur
// Rétro-éclairage
#define LCD_LIGHT 11

// Le buzzer
#define BUZZER 2

// Les branchements sur le MCP23017
// Les boutons
#define BOUTONGAUCHE 2
#define BOUTONDROITE 4
#define BOUTONA      32
#define BOUTONB      16

// Décalage des briques par rapport aux murs (en pixels)
#define DECALX 10
#define DECALY 5

// Eléments de la raquette (position verticale, taille, épaisseur et vitesse)
#define YRAQ         44
#define TAILLERAQ    8
#define EPAISSEURRAQ 2
#define VITESSERAQ   2

// Etats de la balle (collée ou non à la raquette)
#define COLLEE 1
#define LIBRE  0

// La vitesse de la balle
#define VITESSEBALLE 1

// Les évènements du jeu 
#define DECOLLE       1
#define EXPLOSEBRIQUE 2
#define REBONDRAQ     3
#define REBONDMUR     4
#define PERDU         5
#define GAGNER        6

byte evenement;


// Le niveau en cours: 16 briques par lignes sur 10 lignes = 20 octets de données
byte niv[10][2]={ { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111} };

// les 14 niveaux
byte lesNiveaux[14][10][2] = {
  { { B00000000,B00000000},
    { B00000000,B00000000},
    { B00001111,B11110000},
    { B00001111,B11110000},
    { B00001111,B11110000},
    { B00001111,B11110000},
    { B00001111,B11110000},
    { B00000000,B00000000},
    { B00000000,B00000000},
    { B00000000,B00000000}},
  { { B00000000,B00000000},
    { B00000100,B00010000},
    { B00010010,B00100100},
    { B00010111,B11110100},
    { B00011101,B11011100},
    { B00011111,B11111100},
    { B00001111,B11111000},
    { B00000100,B00010000},
    { B00001000,B00001000},
    { B00000000,B00000000} },
  { { B00000011,B11000000},
    { B00000100,B00100000},
    { B00001000,B00010000},
    { B00010010,B01001000},
    { B00010000,B00001000},
    { B00010100,B00101000},
    { B00010011,B11001000},
    { B00001001,B10010000},
    { B00000100,B00100000},
    { B00000011,B11000000} },
  { { B00000000,B00000000},
    { B00110000,B00001100},
    { B01011111,B11111010},
    { B10010000,B00001001},
    { B10010000,B00001001},
    { B10010000,B00001001},
    { B01011111,B11111010},
    { B00110000,B00001100},
    { B00000000,B00000000},
    { B00000000,B00000000} },
  { { B00010010,B10010000},
    { B00110010,B10011000},
    { B01111111,B11111100},
    { B11111111,B11111110},
    { B11111111,B11111110},
    { B11111111,B11111110},
    { B01111011,B10111100},
    { B00110011,B10011000},
    { B00010010,B10010000},
    { B00000000,B00000000} },
  { { B10000011,B11000110},
    { B10000001,B00101001},
    { B10000001,B00101001},
    { B10000001,B00101001},
    { B10000001,B00101001},
    { B10000001,B00101111},
    { B10000001,B00101001},
    { B10000001,B00101001},
    { B10000001,B00101001},
    { B11111011,B11001001} },
  { { B00000010,B00000000},
    { B00000111,B00000000},
    { B00001111,B10000000},
    { B00011111,B11000000},
    { B00111111,B11100000},
    { B01111111,B11110000},
    { B11100000,B00111000},
    { B00101001,B10100000},
    { B00100001,B10100000},
    { B00100001,B10100000} },
  { { B11101110,B11101110},
    { B10101010,B10101010},
    { B11101110,B11101110},
    { B10001010,B10001010},
    { B10001010,B10001010},
    { B00000000,B00000000},
    { B00000000,B00000000},
    { B00000000,B00000000},
    { B00000000,B00000000},
    { B00000000,B00000000} },
  { { B10001011,B10101000},
    { B11011010,B10101000},
    { B10101011,B10110000},
    { B10001010,B10101000},
    { B00000000,B00000000},
    { B01110111,B10000000},
    { B01000100,B10000000},
    { B01110111,B00000000},
    { B01000100,B10000000},
    { B01110100,B10000000} },
  { { B00000001,B00000000},
    { B00000011,B00011000},
    { B00000101,B00010100},
    { B00001001,B00010000},
    { B00001011,B00110000},
    { B00001011,B00110000},
    { B00011000,B00000000},
    { B00011000,B00000000},
    { B00000000,B00000000},
    { B00000000,B00000000} },
  { { B00000001,B11100000},
    { B00000110,B00010000},
    { B00001000,B01001000},
    { B00001000,B00010000},
    { B00010000,B00100000},
    { B00010000,B01000101},
    { B00001000,B00100000},
    { B00001000,B00010000},
    { B00000110,B01100000},
    { B00000001,B10000000} },
  { { B00000011,B11100000},
    { B00001100,B00011000},
    { B00010011,B01100100},
    { B00010011,B01100100},
    { B00010000,B00000100},
    { B00001000,B10001000},
    { B00000100,B00010000},
    { B00000101,B01010000},
    { B00000101,B01010000},
    { B00000101,B01010000} },
  { { B00000000,B00000000},
    { B00110000,B00001100},
    { B01001011,B11010010},
    { B01001100,B00110010},
    { B00110010,B01001100},
    { B00001000,B00010000},
    { B00001001,B10010000},
    { B00000100,B00100000},
    { B00000011,B11000000},
    { B00000000,B00000000} },
  { { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111} }
};

// numéro du niveau
byte niveau;

// la position horizontale de la raquette
byte xRaq;

// la position de la balle
byte xBalle;
byte yBalle;
byte etatBalle;
byte dirBalle;

// initialisation matérielle de l'écran
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);


// L'initialisation du code
void setup() {

  // initialisation de l'écran
  display.begin();

  // activer le retro-éclairage
  analogWrite(LCD_LIGHT, 0);

  // initialisation du MCP23017
  Serial.begin(9600);
  Wire.begin();                  // init bus I2C
  Wire.beginTransmission(0x20);
  Wire.write(0x00);              // registre IODIRA (pour nos LEDs)
  Wire.write(0x00);              // configurées en sortie
  Wire.endTransmission(); 
  Wire.beginTransmission(0x20);
  Wire.write(0x0D);              // registre GPPUB (les boutons)
  Wire.write(0xFF);              // activer les pullups
  Wire.endTransmission();

  // initialisation du buzzer
  pinMode(BUZZER,OUTPUT);

  // lancement du niveau 1
  niveau = 0;
  prochainNiveau();
}


// la gestion des graphismes
void afficheEcran() {

  // on commence par effacer l'écran
  display.clearDisplay();
  
  // On trace les murs
  display.drawLine(0,0,83,0,BLACK);
  display.drawLine(0,0,0,48,BLACK);
  display.drawLine(83,0,83,48,BLACK);
  
  // on affiche les briques
  for (byte ligne=0;ligne<10;ligne++) {
    for (byte groupe=0;groupe<2;groupe++) {
      for (byte col=0;col<8;col++) {
        if (niv[ligne][groupe] & (B10000000>>col)) {
          display.drawRect( DECALX+4*(groupe*8+col), DECALY+3*ligne, 3, 2, BLACK);
        }
      }
    }
    
  // on affiche la raquette
  display.drawRect( xRaq,YRAQ,TAILLERAQ,EPAISSEURRAQ,BLACK);

  // on affiche la balle
  display.drawRect(xBalle,yBalle,2,1,BLACK);
    
  }

  //on affiche l'écran
  display.display();
}


// La gestion des contrôles
void controles() {

  byte boutons=0;
   
  Wire.beginTransmission(0x20); // init I2C 
  Wire.write(0x13);             // registre des états des GPIOB (boutons)
  Wire.endTransmission();
  Wire.requestFrom(0x20, 1);    // lire un octet
  boutons = Wire.read();    // store the incoming byte into boutons 
  boutons = (~boutons)&0xFF;
 
  // si on appuie sur gauche et qu'on est pas au bord: se déplacer
  if ( (boutons&BOUTONGAUCHE) && (xRaq>1)) {
    xRaq=xRaq-VITESSERAQ;
    if (etatBalle == COLLEE) {
      xBalle = xBalle - VITESSERAQ; 
    }
  }

  if ( (boutons&BOUTONDROITE) && (xRaq<(83-TAILLERAQ)) ) {
    xRaq=xRaq+VITESSERAQ;
    if (etatBalle == COLLEE) {
      xBalle = xBalle + VITESSERAQ; 
    }
  }

  if ( (boutons&BOUTONA) && (etatBalle == COLLEE)) {
    etatBalle = LIBRE; 
    dirBalle = 1;
    evenement = DECOLLE;
  } 

  // cheat code 
  if ( (boutons&BOUTONB) && (boutons&BOUTONA)) {
    evenement = GAGNER; 
  }
}


void logiqueJeu() {

  if (etatBalle == LIBRE) {

    // Déplacement de la balle
    if ((dirBalle == 1) || (dirBalle == 2) || (dirBalle == 3)) {
      xBalle += VITESSEBALLE;
    }

    if ((dirBalle == 5) || (dirBalle==6) || (dirBalle==7)) {
      xBalle -= VITESSEBALLE;
    }

    if ((dirBalle == 3) || (dirBalle == 4) || (dirBalle == 5)) {
      yBalle += VITESSEBALLE;
    }

    if ((dirBalle == 7) || (dirBalle == 8) || (dirBalle == 1)) {
      yBalle -= VITESSEBALLE;
    }

   // case touchée
   byte yCase = (yBalle - DECALY)/3;
   byte xCase = (xBalle - DECALX)/4;

   // si la position correspond à une case potentielle
   if ( (xCase >= 0) && (xCase < 16) && (yCase < 10) && (yCase >= 0) ) {
     byte groupe = xCase / 8;
     byte col = xCase % 8;

     // Est-ce qu'une brique est présente à cet endroit
     if (niv[yCase][groupe] & (B10000000>>col)) {
       evenement = EXPLOSEBRIQUE;
       niv[yCase][groupe] = niv[yCase][groupe] & (~(B10000000>>col));

       // changement de direction
       switch (dirBalle) {
        case 1:
          if (xBalle < (DECALX+4*(groupe*8+col)+VITESSEBALLE)) {
            dirBalle = 7;
          } else {
            dirBalle = 3;
          }
          break;
        case 7:
          if (xBalle > (DECALX+4*(groupe*8+col)-VITESSEBALLE+TAILLERAQ)) {
            dirBalle = 1;
          } else {
            dirBalle = 5;
          }
          break;
        case 3:
          if (xBalle < (DECALX+4*(groupe*8+col)+VITESSEBALLE)) {
            dirBalle = 5;
          } else {
            dirBalle = 1;
          }
          break;
        case 5:
          if (xBalle > (DECALX+4*(groupe*8+col)-VITESSEBALLE+TAILLERAQ)) {
            dirBalle = 3;
          } else {
            dirBalle = 7;
          }
        default:
        break;
        }
     }
   }

   // rebond sur la raquette
   if ( ((dirBalle == 3) || (dirBalle == 5)) && 
       (yBalle >= YRAQ) && 
       (yBalle-VITESSEBALLE <= YRAQ) && 
       (xBalle>=xRaq) &&
       (xBalle<=xRaq+TAILLERAQ)) {
         if (dirBalle == 3) { 
           dirBalle = 1;
         } else {
           dirBalle = 7;
         }
         evenement = REBONDRAQ;
       }

   // rebond sur les murs lattéraux
   if ( ((dirBalle == 1) || (dirBalle == 3)) && (xBalle >= 83) ) {
    if (dirBalle == 1) {
      dirBalle = 7;
    } else {
      dirBalle = 5;
    }
    evenement = REBONDMUR;
   } else {
     if ( ((dirBalle == 5) || (dirBalle==7)) && (xBalle <= 0) ) {
       if (dirBalle == 5) {
         dirBalle = 3;
       } else {
         dirBalle = 1;
       }
       evenement = REBONDMUR;
     }
   }

   // rebond sur le mur du haut
  if ( ((dirBalle == 1) || (dirBalle == 7)) && (yBalle <=0 )) {
     if (dirBalle == 1) {
       dirBalle = 3;
     } else {
       dirBalle = 5;
     }
     evenement = REBONDMUR;
   }
   
   // perdu!
   if (yBalle >=48) {
     evenement = PERDU;
   }
   
   // gagné!
   byte gagne = true;
   for (byte l=0;l<10;l++) {
     for (byte c=0;c<2;c++) {
       if (niv[l][c] != 0) {
         gagne = false;
       }
     }
   }
   if (gagne) {
    evenement = GAGNER;
   }
   
  } 
}



void son() {
  switch (evenement) {
    case DECOLLE:
      tone(BUZZER,2000,10);
    break;
    case EXPLOSEBRIQUE:
      tone(BUZZER,1000,10);
    break;
    case REBONDRAQ:
      tone(BUZZER,500,10);
    break;
    case REBONDMUR:
      tone(BUZZER,4000,10);
      break;
    case PERDU:
      tone(BUZZER, 200,1000);
      delay(990);
      nouvelleVie();
    break;
    case GAGNER:
      tone(BUZZER, 500,200);
      delay(100);
      tone(BUZZER, 500,200);
      delay(100);
      tone(BUZZER, 1000,200);
      delay(500);
      prochainNiveau();
      break;
      
    default:
    break;
  }
  delay(10);
  noTone(BUZZER);

  evenement = 0;

}

// on remet le niveau à zéro
void prochainNiveau() {
  niveau++;
  for (byte i=0;i<10;i++) {
    for (byte j=0;j<2;j++) {
      niv[i][j] = lesNiveaux[niveau-1][i][j];
    }
  }
  nouvelleVie();
}

// on initialise le niveau
void nouvelleVie() {
  
  evenement = 0;
  
  // la position de la raquette
  xRaq = 1;

  // la position de la balle
  xBalle    = TAILLERAQ/2;
  yBalle    = YRAQ-1;
  etatBalle = COLLEE;
  dirBalle  = 1;
  
}


// La boucle principale
void loop() {

  // gestion du graphisme
  afficheEcran();
  
  // gestion du son
  son();
  
  // gestion des contrôles
  controles();
  
  // logique du jeu 
  logiqueJeu();
}