// Communication avec l'écran
#include <SPI.h>

// Bibliothèques graphiques
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Les caractères accentués pour les chaines de caractère
#define EAIGU  ((char)0x82)
#define AGRAVE ((char)0x85)


// Les branchements du microcontrolleur
// Rétro-éclairage
#define LCD_LIGHT 5

//NOKIA
#define DCNOKIA 10
#define CSNOKIA 9

// Le buzzer
#define BUZZER A0

//LEDs
#define LED_VERTE A3
#define LED_ROUGE A2


// Les boutons
#define TOUCHE_HAUT   4
#define TOUCHE_BAS    7
#define TOUCHE_GAUCHE  6
#define TOUCHE_DROITE 8
#define TOUCHE_A      2
#define TOUCHE_B      3
char toucheHaut()   { return (LOW == digitalRead(TOUCHE_HAUT)); }
char toucheBas()    { return (LOW == digitalRead(TOUCHE_BAS)); }
char toucheGauche() { return (LOW == digitalRead(TOUCHE_GAUCHE)); } 
char toucheDroite() { return (LOW == digitalRead(TOUCHE_DROITE)); } 
char toucheA()      { return (LOW == digitalRead(TOUCHE_A)); } 
char toucheB()      { return (LOW == digitalRead(TOUCHE_B)); } 



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

byte contrast = 60;

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

// initialisation matérielle de l'écran (SPI MATERIEL)
Adafruit_PCD8544 display = Adafruit_PCD8544(DCNOKIA, CSNOKIA, -1);

// initialisation matérielle de l'écran (SPI LOGICIEL)
//Adafruit_PCD8544 display = Adafruit_PCD8544(13,11,DCNOKIA, CSNOKIA, -1);

void setLEDVerte(byte state) {
  digitalWrite(LED_VERTE,state);
}

void setLEDRouge(byte state) {
  digitalWrite(LED_ROUGE,state);
}

// séquence de démarrage pour tester LEDs et écran et buzzer
void animDebut() {
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(3,display.height()/2-7);
    display.println(" Papamaker.fr");
    String test = String("   Pr")+EAIGU+"sente";
    display.println(test);
    display.display();
    setLEDVerte(HIGH);
    setLEDRouge(LOW);
    tone(BUZZER,1000,500);
    delay(500);
    setLEDVerte(LOW);
    setLEDRouge(HIGH);
    tone(BUZZER,1500,500);
    delay(500);
    setLEDVerte(HIGH);
    setLEDRouge(LOW);
    tone(BUZZER,2000,500);
    delay(500);
    setLEDVerte(LOW);
    setLEDRouge(LOW);
    tone(BUZZER,2500,500);
    delay(1000);
    noTone(BUZZER);
    delay(500);
    
    setLEDVerte(HIGH);
    setLEDRouge(HIGH);
    tone(BUZZER,4000,500);
    delay(1000);
    noTone(BUZZER);
    setLEDVerte(LOW);
    setLEDRouge(LOW);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(3,display.height()/2-7);
    display.println("Casse-briques ");
    display.display();
    
    delay(2000);
}


// L'initialisation du code
void setup() {

  // set as outputs
  DDRB |= (1 << DDB7);

  // reset LCD
  PORTB &= ~(1 << PORTB7);
  delay(500);
  PORTB |= (1 << PORTB7);

    pinMode(LED_VERTE,OUTPUT);

    pinMode(LED_ROUGE,OUTPUT);


  // Les boutons(pull-ups + input)
  pinMode(TOUCHE_HAUT,INPUT);
  digitalWrite(TOUCHE_HAUT,HIGH);
  pinMode(TOUCHE_DROITE,INPUT);
  digitalWrite(TOUCHE_DROITE,HIGH);
  pinMode(TOUCHE_BAS,INPUT);
  digitalWrite(TOUCHE_BAS,HIGH);
  pinMode(TOUCHE_GAUCHE,INPUT);
  digitalWrite(TOUCHE_GAUCHE,HIGH);
  pinMode(TOUCHE_A,INPUT);
  digitalWrite(TOUCHE_A,HIGH);
  pinMode(TOUCHE_B,INPUT);
  digitalWrite(TOUCHE_B,HIGH);


  // initialisation de l'écran
  display.begin();
  display.setContrast(contrast);

  // activer le retro-éclairage
  analogWrite(LCD_LIGHT, 0);

  // initialisation du buzzer
  pinMode(BUZZER,OUTPUT);


  if (toucheA()) {
    while (!toucheB()) {
      digitalWrite(LED_VERTE,HIGH);
      if (toucheHaut()) {
         if (contrast <127) contrast++;
      }

      if (toucheBas()) {
         if (contrast >0) contrast--;
      }
      display.setContrast(contrast);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(BLACK);
      display.setCursor(3,display.height()/2-7);
      display.println("-Constraste-");
      display.drawLine(0,30,contrast*display.width()/128,30,BLACK);
      display.display();
    }
  }
        digitalWrite(LED_VERTE,LOW);

  animDebut();
  
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
  display.drawRect(xBalle,yBalle,2,2,BLACK);
    
  }

  //on affiche l'écran
  display.display();
}


// La gestion des contrôles
void controles() {

  byte boutons=0;

  if (toucheHaut()) {
    if (contrast <127) contrast++;
      display.setContrast(contrast);
  }

  if (toucheBas()) {
    if (contrast >0) contrast--;
      display.setContrast(contrast);
  }
 
  // si on appuie sur gauche et qu'on est pas au bord: se déplacer
  if ( (toucheGauche()) && (xRaq>1)) {
    xRaq=xRaq-VITESSERAQ;
    if (etatBalle == COLLEE) {
      xBalle = xBalle - VITESSERAQ; 
    }
  }

  if ( (toucheDroite()) && (xRaq<(83-TAILLERAQ)) ) {
    xRaq=xRaq+VITESSERAQ;
    if (etatBalle == COLLEE) {
      xBalle = xBalle + VITESSERAQ; 
    }
  }

  if ( (toucheA()) && (etatBalle == COLLEE)) {
    etatBalle = LIBRE; 
    dirBalle = 1;
    evenement = DECOLLE;
  } 

  // cheat code 
  if ( (toucheB()) && (toucheA())) {
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

    for (byte i =0; i<8;i++) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(BLACK);
      display.setCursor(12-i,display.height()/2-7);
      display.println(String("Niveau ")+(niveau+1));
      display.display();
      tone(BUZZER,500*i,150);
      delay(150);
    }
    delay(2000);
  
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
  delay(30);
  // gestion du son
  son();
  
  // gestion des contrôles
  controles();
  
  // logique du jeu 
  logiqueJeu();
}
