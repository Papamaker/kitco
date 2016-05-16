// communication avec l'écran
#include <SPI.h>

//bibliothèques graphiques
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// gestion du I2C pour le MCP23017
#include "Wire.h"

// Les branchements du microcontrolleur
// Rétro-éclairage
#define LCD_LIGHT 11

// Le buzzer
#define BUZZER 2

// Les boutons
#define BOUTONGAUCHE 2
#define BOUTONDROITE 4
#define BOUTONA 32
#define BOUTONB 16

// décalage des briques
#define DECALX 10
#define DECALY 5

// Eléments de la raquette
#define YRAQ 44
#define TAILLERAQ 8
#define EPAISSEURRAQ 2
#define VITESSERAQ 2

//Etats de la balle
#define COLLEE 1
#define LIBRE 0

#define VITESSEBALLE 1

// Evenements sonores
#define DECOLLE 1
#define EXPLOSEBRIQUE 2
#define REBONDRAQ 3
#define REBONDMUR 4
#define PERDU 5
#define GAGNER 6

byte sonAJouer=0;


// 16 briques par lignes sur 10 lignes = 20 octets de données
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

byte lesNiveaux[5][10][2] = {
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
  { { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111} },
  { { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111},
    { B11111111,B11111111} },
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
byte niveau = 0;

// la position de la raquette
byte xRaq = 1;

// la position de la balle
byte xBalle = TAILLERAQ/2;
byte yBalle = YRAQ-1;
byte etatBalle = COLLEE;
byte dirBalle = 1;

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

    restart();

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
    sonAJouer = DECOLLE;
  } 

  // cheat code 
  if ( (boutons&BOUTONB) && (boutons&BOUTONA)) {
    sonAJouer = GAGNER; 
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
       sonAJouer = EXPLOSEBRIQUE;
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
         sonAJouer = REBONDRAQ;
       }

   // rebond sur les murs lattéraux
   if ( ((dirBalle == 1) || (dirBalle == 3)) && (xBalle >= 83) ) {
    if (dirBalle == 1) {
      dirBalle = 7;
    } else {
      dirBalle = 5;
    }
    sonAJouer = REBONDMUR;
   } else {
     if ( ((dirBalle == 5) || (dirBalle==7)) && (xBalle <= 0) ) {
       if (dirBalle == 5) {
         dirBalle = 3;
       } else {
         dirBalle = 1;
       }
       sonAJouer = REBONDMUR;
     }
   }

   // rebond sur le mur du haut
  if ( ((dirBalle == 1) || (dirBalle == 7)) && (yBalle <=0 )) {
     if (dirBalle == 1) {
       dirBalle = 3;
     } else {
       dirBalle = 5;
     }
     sonAJouer = REBONDMUR;
   }
   
   // perdu!
   if (yBalle >=48) {
     sonAJouer = PERDU;
   }
   
   // gagné!

  }
}



void son() {
  switch (sonAJouer) {
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
      niveau=0;
      delay(990);
      restart();
    break;
    case GAGNER:
      tone(BUZZER, 500,200);
      delay(100);
      tone(BUZZER, 500,200);
      delay(100);
      tone(BUZZER, 1000,200);
      delay(500);
      niveau++;
      restart();
      break;
      
    default:
    break;
  }
  delay(10);
  noTone(BUZZER);

  sonAJouer = 0;

}


// on reprend le niveau: réinitialiser tout sauf niv
void restart() {
  
  for (byte i=0;i<10;i++) {
    for (byte j=0;j<2;j++) {
      niv[i][j] = lesNiveaux[niveau][i][j];
    }
  }

  sonAJouer = 0;
  
  // la position de la raquette
  xRaq = 1;

  // la position de la balle
  xBalle = TAILLERAQ/2;
  yBalle = YRAQ-1;
  etatBalle = COLLEE;
  dirBalle = 1;
  
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
