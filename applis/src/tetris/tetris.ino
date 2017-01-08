// Jeu de Tetris
// www.kitco.fr

// Nécessaire pour l'environnement Kitco
#include "kitco.h"
#include <EEPROM.h>

// 
// Gestion de la musique de fond
//

// le mode sonore
byte    cfgMusic      = 0;
#define MUET          0
#define SON_SEUL      1
#define MUSIQUE_SEULE 2
#define SON_MUSIQUE   3

// définition des fréquences des 8 gammes 13,14,15 = réservés===sur 4 bits 
#define SILENCE   0
#define NOTE_DO   1
#define NOTE_DO_  2
#define NOTE_RE   3
#define NOTE_RE_  4
#define NOTE_MI   5
#define NOTE_FA   6
#define NOTE_FA_  7
#define NOTE_SOL  8
#define NOTE_SOL_ 9
#define NOTE_LA  10
#define NOTE_LA_ 11
#define NOTE_SI  12

// l'octave codé sur 4 bits
#define NOTE_OCTAVE1 (0 << 4)
#define NOTE_OCTAVE2 (1 << 4)
#define NOTE_OCTAVE3 (2 << 4)
#define NOTE_OCTAVE4 (3 << 4)
#define NOTE_OCTAVE5 (4 << 4)
#define NOTE_OCTAVE6 (5 << 4)
#define NOTE_OCTAVE7 (6 << 4)
#define NOTE_OCTAVE8 (7 << 4)

// duree d'une ronde en millisecondes
#define DUREE_RONDE 4000

// longueur des notes (4 bits)   
#define NOTE_RONDE   1  
#define NOTE_BLANCHE 2  
#define NOTE_NOIRE   3  
#define NOTE_CROCHE  4  
#define NOTE_CROCHE2 5  
#define NOTE_CROCHE3 6  
#define NOTE_CROCHE4 7  

// note détachée sur 1 bit
#define NOTE_DETACHEE (0    << 4)
#define NOTE_LIEE     (1    << 4)

// Le morceau de musique
byte morceau[] = {NOTE_MI   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_LIEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_LIEE,
                  NOTE_FA   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_SOL  | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_FA   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_LIEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_CROCHE  | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_NOIRE   | NOTE_DETACHEE,
                  SILENCE , NOTE_NOIRE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE3, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_SOL_ | NOTE_OCTAVE3, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_NOIRE   | NOTE_DETACHEE,
                  SILENCE,NOTE_NOIRE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_RE   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_SI   | NOTE_OCTAVE3, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_DO   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_MI   | NOTE_OCTAVE4, NOTE_NOIRE   | NOTE_DETACHEE,
                  NOTE_LA   | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  NOTE_SOL_ | NOTE_OCTAVE4, NOTE_BLANCHE | NOTE_DETACHEE,
                  SILENCE , NOTE_BLANCHE
                  };

// l'index qui suit la partition
char indexMusique = 0;

// le décalage en millisec. au sein d'un même index
int decalNote     = 0;

// durée en millisecondes de la fenêtre dédiée à la musique
# define FENETREMUSIQUE 20

// durée du silence entre deux notes détachées
#define PAUSENOTE 40

int frequenceNote(byte note) {

   // on se base sur le LA3 (440Hz)
   int freqRes = 440;

   // 1 - calcul de la note correspondante (sans changer d'octave)
   if (((note&0xF) - NOTE_LA )>0) {
     for (byte i=0;i< ((note&0xF)-NOTE_LA);i++) {
      
        // pour chaque note d'écart, on monte d'un demi ton (multiplier la frequence par racine douzième de 2)
        freqRes = freqRes*1.05946;  
     }
   } else {
    
     // et réciproquement quand la note est plus basse que le LA
     for (byte i=0;i<(NOTE_LA-(note&0xF));i++) {
        freqRes = freqRes/1.05946;
     }
   }

  // 2 - changement pour le bon octave: multiplier ou diviser par deux la fréquence
  if ( (note&0xF0) > NOTE_OCTAVE3) {
    for (byte i=0;i< (((note&0xF0) - NOTE_OCTAVE3)>>4);i++) {
       freqRes = freqRes * 2;
    }
  } else {
    for (byte i=0;i< ((NOTE_OCTAVE3 - (note&0xF0))>>4);i++) {
       freqRes = freqRes / 2;
    }
  }
  return freqRes;
}

// la valeur de millis() enregistré la dernière fois
unsigned long lastMillis = 0;

void joueMusique() {

   if (lastMillis == 0) {
    lastMillis = millis();
   }
   unsigned long maintenant = millis();
   decalNote += maintenant - lastMillis;
    
   int dureeNote = DUREE_RONDE/(1<<(morceau[indexMusique+1]&0xF));
   
   // si decalNote est plus long que la note, on passe à la suivante
   if (decalNote > dureeNote) {
     decalNote = decalNote - dureeNote;
     indexMusique = (indexMusique + 2) % (sizeof(morceau)/sizeof(byte));

   }
  
    // doit-on jouer la note en question ou alors la pause (entre deux notes détachées)
    if ((decalNote < PAUSENOTE) && 
        (!(morceau[indexMusique+1] & NOTE_LIEE))) {
        noTone(BUZZER);
    } else {
       if ((morceau[indexMusique]&0xF)==SILENCE) {
         noTone(BUZZER);
       } else {
           if (cfgMusic) {
               tone(BUZZER,frequenceNote(morceau[indexMusique]));
           } else {
            noTone(BUZZER);
           }
       }
    }

   lastMillis = millis();
}


//
// LA LOGIQUE DU JEU
// 
// la vitesse du jeu
byte vitesse = 50;

// la vitesse selon le niveau de difficulté
byte tabVitesse[9] = {20,15,12,10,7,5,4,3,2};

//le compteur de cycle
long cycle = 0;

// la tempo pour afficher l'animation briques détruites
byte tempoDetruit = 10;

// le meilleur score
int record = 0;

// la durée de la fenêtre pour les effets sonores
#define DUREE_SON 20

//
// Les graphismes
//


// Les constantes
#define AIRE_JEU_X1 27
#define AIRE_JEU_Y1 0
#define AIRE_JEU_X2 56
#define AIRE_JEU_Y2 47

#define EST 0
#define SUD 1
#define OUEST 2
#define NORD 3

// position de départ de la piece
char colPiece=3;
char ligPiece=0;
char orientationPiece= EST;
byte typePiece=0;
byte typePieceSuivante=1;

// l'événement en cours  
byte evenement;

// Les événements du jeu
#define RIEN 0
#define BLOQUE_PIECE 1
#define DETRUIT 2
#define ROTATION 3
#define FIN_PARTIE 99


// Les écrans du jeu 
byte modeJeu = 0;
#define ACCUEIL   0
#define PARTIE    1
#define FINPARTIE 2
#define PAUSE     3

int score  = 0;
int lignes = 0;
byte niveau = 1;
byte nivDebut = 1;
byte eclair = 1;


byte detruit[16] = {0,0,0,0,0,0,0};
// les cases immobiles du tableau (y compris 4 lignes au sol (invisibles)
byte cases[20][10] = 
{{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1}};

// un pièce représentée sur 2 octets = 4 lignes de 4 bits
// 4 orientations (E,S,O,N)
// 7 pieces différentes I,O,T,S,Z,L,J
#define PIECE_I 0
#define PIECE_O 1
#define PIECE_T 2
#define PIECE_S 3
#define PIECE_Z 4
#define PIECE_L 5
#define PIECE_J 6
byte pieces[7][4][2]={
  { {B00001111,B00000000},{B01000100,B01000100},{B00001111,B00000000},{B01000100,B01000100} },
  { {B01100110,B00000000},{B01100110,B00000000},{B01100110,B00000000},{B01100110,B00000000} },
  { {B00100111,B00000000},{B01000110,B01000000},{B01110010,B00000000},{B00100110,B00100000} },
  { {B00110110,B00000000},{B01000110,B00100000},{B00110110,B00000000},{B01000110,B00100000} },
  { {B01100011,B00000000},{B00100110,B01000000},{B01100011,B00000000},{B00100110,B01000000} },
  { {B00101110,B00000000},{B01000100,B01100000},{B11101000,B00000000},{B11000100,B01000000} },
  { {B01000111,B00000000},{B01100100,B01000000},{B01110001,B00000000},{B00100010,B01100000} },
};



// La partie Setup concerne ce qui va être exécuté au démarrage de Kitco
void setup() {

  // Cette commande est nécessaire pour intialiser Kitco à son démarrage
  initialiserKitco(1);
  lcdBegin();
  setContrast(60);

  //mise à zéro du record
  // EEPROM.write(0,0); 
  // EEPROM.write(1,0);
  
  record = EEPROM.read(0)+EEPROM.read(1)*255;

}





#define dessineCase(l,c,dx,dy)   creerRectangle(AIRE_JEU_X1+(c)*3+dx,(l)*3+dy,AIRE_JEU_X1+(c)*3+2+dx,(l)*3+2+dy,false,NOIR)

// tracer la piece en cours
void tracePiece() {
  if (pieces[typePiece][orientationPiece][0]&B10000000) { dessineCase(ligPiece,colPiece,0,0);     }
  if (pieces[typePiece][orientationPiece][0]&B01000000) { dessineCase(ligPiece,colPiece+1,0,0);   }
  if (pieces[typePiece][orientationPiece][0]&B00100000) { dessineCase(ligPiece,colPiece+2,0,0);   }
  if (pieces[typePiece][orientationPiece][0]&B00010000) { dessineCase(ligPiece,colPiece+3,0,0);   }
  if (pieces[typePiece][orientationPiece][0]&B00001000) { dessineCase(ligPiece+1,colPiece,0,0);   }
  if (pieces[typePiece][orientationPiece][0]&B00000100) { dessineCase(ligPiece+1,colPiece+1,0,0); }
  if (pieces[typePiece][orientationPiece][0]&B00000010) { dessineCase(ligPiece+1,colPiece+2,0,0); }
  if (pieces[typePiece][orientationPiece][0]&B00000001) { dessineCase(ligPiece+1,colPiece+3,0,0); }
  if (pieces[typePiece][orientationPiece][1]&B10000000) { dessineCase(ligPiece+2,colPiece,0,0);   }
  if (pieces[typePiece][orientationPiece][1]&B01000000) { dessineCase(ligPiece+2,colPiece+1,0,0); }
  if (pieces[typePiece][orientationPiece][1]&B00100000) { dessineCase(ligPiece+2,colPiece+2,0,0); }
  if (pieces[typePiece][orientationPiece][1]&B00010000) { dessineCase(ligPiece+2,colPiece+3,0,0); }
  if (pieces[typePiece][orientationPiece][1]&B00001000) { dessineCase(ligPiece+3,colPiece,0,0);   }
  if (pieces[typePiece][orientationPiece][1]&B00000100) { dessineCase(ligPiece+3,colPiece+1,0,0); }
  if (pieces[typePiece][orientationPiece][1]&B00000010) { dessineCase(ligPiece+3,colPiece+2,0,0); }
  if (pieces[typePiece][orientationPiece][1]&B00000001) { dessineCase(ligPiece+3,colPiece+3,0,0); }
}

// tracer la piece suivante
void tracePieceSuivante() {
  #define LIGNECADRE 4
  #define COLCADRE   -6
  char decalY=0;
  char decalX=0;

if (typePieceSuivante == PIECE_I) decalY = -1;
  if (typePieceSuivante==PIECE_T||typePieceSuivante==PIECE_S||typePieceSuivante==PIECE_Z|| typePieceSuivante==PIECE_J) decalX = -1; 
  if (typePieceSuivante == PIECE_L) decalX = +1;
  
  if (pieces[typePieceSuivante][EST][0]&B10000000) { dessineCase(LIGNECADRE,COLCADRE,decalX,decalY);     }
  if (pieces[typePieceSuivante][EST][0]&B01000000) { dessineCase(LIGNECADRE,COLCADRE+1,decalX,decalY);   }
  if (pieces[typePieceSuivante][EST][0]&B00100000) { dessineCase(LIGNECADRE,COLCADRE+2,decalX,decalY);   }
  if (pieces[typePieceSuivante][EST][0]&B00010000) { dessineCase(LIGNECADRE,COLCADRE+3,decalX,decalY);   }
  if (pieces[typePieceSuivante][EST][0]&B00001000) { dessineCase(LIGNECADRE+1,COLCADRE,decalX,decalY);   }
  if (pieces[typePieceSuivante][EST][0]&B00000100) { dessineCase(LIGNECADRE+1,COLCADRE+1,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][0]&B00000010) { dessineCase(LIGNECADRE+1,COLCADRE+2,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][0]&B00000001) { dessineCase(LIGNECADRE+1,COLCADRE+3,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][1]&B10000000) { dessineCase(LIGNECADRE+2,COLCADRE,decalX,decalY);   }
  if (pieces[typePieceSuivante][EST][1]&B01000000) { dessineCase(LIGNECADRE+2,COLCADRE+1,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][1]&B00100000) { dessineCase(LIGNECADRE+2,COLCADRE+2,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][1]&B00010000) { dessineCase(LIGNECADRE+2,COLCADRE+3,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][1]&B00001000) { dessineCase(LIGNECADRE+3,COLCADRE,decalX,decalY);   }
  if (pieces[typePieceSuivante][EST][1]&B00000100) { dessineCase(LIGNECADRE+3,COLCADRE+1,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][1]&B00000010) { dessineCase(LIGNECADRE+3,COLCADRE+2,decalX,decalY); }
  if (pieces[typePieceSuivante][EST][1]&B00000001) { dessineCase(LIGNECADRE+3,COLCADRE+3,decalX,decalY); }
}

// indique s'il y a intersection avec les cases dispo
byte intersect (byte orient) {

return (( (pieces[typePiece][orient][0]&B10000000) && (cases[ligPiece][colPiece] || !((colPiece<10) && (colPiece>-1) && (ligPiece<16) && (ligPiece>-1) ))) || 
        ( (pieces[typePiece][orient][0]&B01000000) && (cases[ligPiece][colPiece+1] || !((colPiece<9) && (colPiece>-2) && (ligPiece<16) && (ligPiece>-1))))   ||
        ( (pieces[typePiece][orient][0]&B00100000) && (cases[ligPiece][colPiece+2] || !( (colPiece<8) && (colPiece>-3) && (ligPiece<16) && (ligPiece>-1) )))  ||
        ( (pieces[typePiece][orient][0]&B00010000) && (cases[ligPiece][colPiece+3] || !( (colPiece<7) && (colPiece>-4) && (ligPiece<16) && (ligPiece>-1) )))  ||
        ( (pieces[typePiece][orient][0]&B00001000) && (cases[ligPiece+1][colPiece] || !( (colPiece<10) && (colPiece>-1) && (ligPiece<15) && (ligPiece>-2) )))  ||
        ( (pieces[typePiece][orient][0]&B00000100) && (cases[ligPiece+1][colPiece+1] || !( (colPiece<9) && (colPiece>-2) && (ligPiece<15) && (ligPiece>-2)))) ||
        ( (pieces[typePiece][orient][0]&B00000010) && (cases[ligPiece+1][colPiece+2] || !( (colPiece<8) && (colPiece>-3) && (ligPiece<15) && (ligPiece>-2)))) ||
        ( (pieces[typePiece][orient][0]&B00000001) && (cases[ligPiece+1][colPiece+3] || !( (colPiece<7) && (colPiece>-4) && (ligPiece<15) && (ligPiece>-2)))) ||
        ( (pieces[typePiece][orient][1]&B10000000) && (cases[ligPiece+2][colPiece] || !( (colPiece<10) && (colPiece>-1) && (ligPiece<16) && (ligPiece>-3)  ))) ||
        ( (pieces[typePiece][orient][1]&B01000000) && (cases[ligPiece+2][colPiece+1] || !( (colPiece<9) && (colPiece>-2) && (ligPiece<16) && (ligPiece>-3)))) ||
        ( (pieces[typePiece][orient][1]&B00100000) && (cases[ligPiece+2][colPiece+2] || !( (colPiece<8) && (colPiece>-3) && (ligPiece<16) && (ligPiece>-3)))) ||
        ( (pieces[typePiece][orient][1]&B00010000) && (cases[ligPiece+2][colPiece+3] || !( (colPiece<7) && (colPiece>-4) && (ligPiece<16) && (ligPiece>-3)))) ||
        ( (pieces[typePiece][orient][1]&B00001000) && (cases[ligPiece+3][colPiece] || !( (colPiece<10) && (colPiece>-1) && (ligPiece<16) && (ligPiece>-4))))   ||
        ( (pieces[typePiece][orient][1]&B00000100) && (cases[ligPiece+3][colPiece+1] || !((colPiece<9) && (colPiece>-2) && (ligPiece<16) && (ligPiece>-4)))) ||
        ( (pieces[typePiece][orient][1]&B00000010) && (cases[ligPiece+3][colPiece+2] || !( (colPiece<8) && (colPiece>-3) && (ligPiece<16) && (ligPiece>-4)))) ||
        ( (pieces[typePiece][orient][1]&B00000001) && (cases[ligPiece+3][colPiece+3] || !( (colPiece<7) && (colPiece>-4) && (ligPiece<16) && (ligPiece>-4)))) );
}

// indique si la piece en cours repose sur qqchose
byte piecePose () {

return (( (pieces[typePiece][orientationPiece][0]&B10000000) && cases[ligPiece+1][colPiece])     || 
        ( (pieces[typePiece][orientationPiece][0]&B01000000) && cases[ligPiece+1][colPiece+1])   ||
        ( (pieces[typePiece][orientationPiece][0]&B00100000) && cases[ligPiece+1][colPiece+2])   ||
        ( (pieces[typePiece][orientationPiece][0]&B00010000) && cases[ligPiece+1][colPiece+3])   ||
        ( (pieces[typePiece][orientationPiece][0]&B00001000) && cases[ligPiece+2][colPiece])   ||
        ( (pieces[typePiece][orientationPiece][0]&B00000100) && cases[ligPiece+2][colPiece+1]) ||
        ( (pieces[typePiece][orientationPiece][0]&B00000010) && cases[ligPiece+2][colPiece+2]) ||
        ( (pieces[typePiece][orientationPiece][0]&B00000001) && cases[ligPiece+2][colPiece+3]) ||
        ( (pieces[typePiece][orientationPiece][1]&B10000000) && cases[ligPiece+3][colPiece])   ||
        ( (pieces[typePiece][orientationPiece][1]&B01000000) && cases[ligPiece+3][colPiece+1]) ||
        ( (pieces[typePiece][orientationPiece][1]&B00100000) && cases[ligPiece+3][colPiece+2]) ||
        ( (pieces[typePiece][orientationPiece][1]&B00010000) && cases[ligPiece+3][colPiece+3]) ||
        ( (pieces[typePiece][orientationPiece][1]&B00001000) && cases[ligPiece+4][colPiece])   ||
        ( (pieces[typePiece][orientationPiece][1]&B00000100) && cases[ligPiece+4][colPiece+1]) ||
        ( (pieces[typePiece][orientationPiece][1]&B00000010) && cases[ligPiece+4][colPiece+2]) ||
        ( (pieces[typePiece][orientationPiece][1]&B00000001) && cases[ligPiece+4][colPiece+3]) );
}

// ajouter la piece en cours dans les cases
void ajoutePiece() {
  if (pieces[typePiece][orientationPiece][0]&B10000000) { cases[ligPiece][colPiece]=1;     }
  if (pieces[typePiece][orientationPiece][0]&B01000000) { cases[ligPiece][colPiece+1]=1;   }
  if (pieces[typePiece][orientationPiece][0]&B00100000) { cases[ligPiece][colPiece+2]=1;   }
  if (pieces[typePiece][orientationPiece][0]&B00010000) { cases[ligPiece][colPiece+3]=1;   }
  if (pieces[typePiece][orientationPiece][0]&B00001000) { cases[ligPiece+1][colPiece]=1;   }
  if (pieces[typePiece][orientationPiece][0]&B00000100) { cases[ligPiece+1][colPiece+1]=1; }
  if (pieces[typePiece][orientationPiece][0]&B00000010) { cases[ligPiece+1][colPiece+2]=1; }
  if (pieces[typePiece][orientationPiece][0]&B00000001) { cases[ligPiece+1][colPiece+3]=1; }
  if (pieces[typePiece][orientationPiece][1]&B10000000) { cases[ligPiece+2][colPiece]=1;   }
  if (pieces[typePiece][orientationPiece][1]&B01000000) { cases[ligPiece+2][colPiece+1]=1; }
  if (pieces[typePiece][orientationPiece][1]&B00100000) { cases[ligPiece+2][colPiece+2]=1; }
  if (pieces[typePiece][orientationPiece][1]&B00010000) { cases[ligPiece+2][colPiece+3]=1; }
  if (pieces[typePiece][orientationPiece][1]&B00001000) { cases[ligPiece+3][colPiece]=1;   }
  if (pieces[typePiece][orientationPiece][1]&B00000100) { cases[ligPiece+3][colPiece+1]=1; }
  if (pieces[typePiece][orientationPiece][1]&B00000010) { cases[ligPiece+3][colPiece+2]=1; }
  if (pieces[typePiece][orientationPiece][1]&B00000001) { cases[ligPiece+3][colPiece+3]=1; }
}

// Gestion des graphismes
void afficheJeu(){

  effacerEcran(NOIR);
  
  //1 afficher le cadre du jeu
  creerRectangle(AIRE_JEU_X1, 0,AIRE_JEU_X2+1, HAUTEUR_ECRAN,true,BLANC);
 // ligneEcran(AIRE_JEU_X1-1, 0, AIRE_JEU_X1-1, HAUTEUR_ECRAN-1, NOIR);
  //ligneEcran(AIRE_JEU_X2+1, 0, AIRE_JEU_X2+1, HAUTEUR_ECRAN, NOIR);


    
  //2 afficher le carré qui affiche la pièce suivante
  creerRectangle(7, 7, 23, 22, true, BLANC);

  //2 bis afficher la pièce suivante
  tracePieceSuivante();
  
  //3 afficher le score
  creerRectangle(AIRE_JEU_X2+2,4,AIRE_JEU_X2+3+24,12,true,BLANC);
  ecrireLettre('0'+score%10,AIRE_JEU_X2+3+18,5,NOIR);
  if (score>=10) ecrireLettre('0'+(score/10)%10,AIRE_JEU_X2+3+12,5,NOIR);
  if (score>=100) ecrireLettre('0'+(score/100)%10,AIRE_JEU_X2+3+6,5,NOIR); else { ecrireLettre('S',AIRE_JEU_X2+3,5,NOIR); ecrireLettre(':',AIRE_JEU_X2+3+6,5,NOIR);}
  if (score>=1000) ecrireLettre('0'+(score/1000)%10,AIRE_JEU_X2+3,5,NOIR);
  //ecrireEcran("9999",AIRE_JEU_X2+3,5,NOIR);

  creerRectangle(AIRE_JEU_X2+2,14,AIRE_JEU_X2+3+23,22,false,BLANC);
  ecrireEcran("Niv ",AIRE_JEU_X2+3,15,NOIR);
  ecrireLettre('0'+niveau,AIRE_JEU_X2+3+18,15,NOIR);

  creerRectangle(AIRE_JEU_X2+2,24,AIRE_JEU_X2+3+23,32,false,BLANC);
  ecrireEcran("Lig:",AIRE_JEU_X2+3,25,NOIR);
  ecrireEcran("    ",AIRE_JEU_X2+3,35,NOIR);

  creerRectangle(AIRE_JEU_X2+2,34,AIRE_JEU_X2+3+23,42,false,BLANC);
  ecrireLettre('0'+lignes%10,AIRE_JEU_X2+3+18,35,NOIR);
  if (lignes>=10) ecrireLettre('0'+(lignes/10)%10,AIRE_JEU_X2+3+12,35,NOIR);
  if (lignes>=100) ecrireLettre('0'+(lignes/100)%10,AIRE_JEU_X2+3+6,35,NOIR); 
  if (lignes>=1000) ecrireLettre('0'+(lignes/1000)%10,AIRE_JEU_X2+3,35,NOIR);
  
  //4 afficher les briques posées
  for (byte c=0;c<10;c++) {
    for (byte l=0;l<16;l++) {
      if (cases[l][c]) {
        // dans le  cas de destruction de ligne, on affiche en clignotant les lignes détruites
        if ((evenement != DETRUIT) || (!detruit[l]) || (cycle%2)) {
           dessineCase(l,c,0,0);
        }
      }
    }  
  }
  
  //5 afficher la pièce en cours
  tracePiece();
  rafraichirEcran();
}

byte relache = true;

// Gestion des appuis touches
void gestionTouches(){

  // si la tempo est OK et que la case de gauche est libre, on prend en compte l'appui gauche
  if ( toucheGauche() ) { 
       colPiece--; 
       if (intersect(orientationPiece)) {
        colPiece++;
       }
  }

  // si la tempo est OK et que la case de droite est libre, on prend en compte l'appui droite
  if ( toucheDroite() ) { 
       colPiece++; 
       if (intersect(orientationPiece)) {
        colPiece--;
       }
  }
  
  if (toucheBas()) {
      
      // on teste si la pièce touche en dessous 
      if (piecePose()) { // alors on bloque la piece
          evenement = BLOQUE_PIECE;        
      } else {
          ligPiece++; 
      }
  }
  
  byte nouvelleOrientation = orientationPiece;
  if (toucheA()&&relache) { 
      nouvelleOrientation = (orientationPiece+3) % 4; 
  }
  
  if (toucheB()&&relache) { 
      nouvelleOrientation = (orientationPiece+1) % 4; 
  }

  if ((!toucheA()) && (!toucheB())) {
    relache = true;
  } else {
    relache = false;
  }
  
  if (!intersect(nouvelleOrientation)) {
     if (orientationPiece !=nouvelleOrientation) {
       evenement =ROTATION;
       orientationPiece = nouvelleOrientation;
     }
  }

  
}

// gestion de la logique du jeu
void logiqueJeu() {
  if (!(cycle%vitesse)) {
      if (piecePose()) { // alors on bloque la piece
          evenement = BLOQUE_PIECE;        
      } else {
          ligPiece++; 
      }
  }
  
  // est-ce que la pièce est bloquée ?
  if (evenement != BLOQUE_PIECE) {   // non: on ne fait rien
    return;
  }
  //evenement = RIEN;  

  // oui 
  // on l'ajoute dans les cases
  ajoutePiece();

  // on teste si on a fait des lignes
  for (byte l=0;l<16;l++) {
    if (cases[l][0]&cases[l][1]&cases[l][2]&cases[l][3]&cases[l][4]&cases[l][5]&cases[l][6]&cases[l][7]&cases[l][8]&cases[l][9]) {
      detruit[l] = 1;
      evenement = DETRUIT;
      tempoDetruit = 10;
    }
  }

  // on passe à la pièce suivante
  colPiece=3;
  ligPiece=0;
  orientationPiece= EST;
  typePiece=typePieceSuivante;
  typePieceSuivante = random(0,7);

   if (intersect(orientationPiece)) {
     evenement = FIN_PARTIE;
   }
}

void sonFinPartie() {
  frequenceBuzzer(2000,100);
  delai(100);
  frequenceBuzzer(2000,100);
  delai(100);
  frequenceBuzzer(1000,1000);
  delai(100);
}

void sonRecord() {
  frequenceBuzzer(1000,100);
  delai(100);
  frequenceBuzzer(1000,100);
  delai(100);
  frequenceBuzzer(2000,1000);  
  delai(100);
}

byte tempoSon;

void sonJeu() {
  switch (evenement) {
    case FIN_PARTIE:
      evenement = RIEN;
      modeJeu = ACCUEIL;
      niveau = 1;
      if (score>record) {
        record = score;
        EEPROM.write(0,record%255);
        EEPROM.write(1,record/255);
        sonRecord();
      } else {
        sonFinPartie();
      }
      while (!touche());
      while (touche());
      break;
  case  BLOQUE_PIECE:
    frequenceBuzzer(1000,20);
    evenement = RIEN;
    break;
  case ROTATION:
    frequenceBuzzer(2000,20);
    evenement = RIEN;
    break;
  case  DETRUIT:
    frequenceBuzzer(1000+20*(tempoSon++),20);
    break;
    default:
      tempoSon = 0;
      delai(DUREE_SON);
      break;
  }

  
}

void retireLignes() {
  byte nbLig=0;
  byte scoreLig[]={1,2,4,8};
  for (byte i =0;i<16;i++) {
    if (detruit[i]) {
      lignes++;
      nbLig++;
      niveau = min(9,nivDebut+(lignes/10));
      vitesse = tabVitesse[min(8,niveau-1)];
      detruit[i]=0;
      for (byte j=i;j>0;j--){
        cases[j][0] = cases[j-1][0];
        cases[j][1] = cases[j-1][1];
        cases[j][2] = cases[j-1][2];
        cases[j][3] = cases[j-1][3];
        cases[j][4] = cases[j-1][4];
        cases[j][5] = cases[j-1][5];
        cases[j][6] = cases[j-1][6];
        cases[j][7] = cases[j-1][7];
        cases[j][8] = cases[j-1][8];
        cases[j][9] = cases[j-1][9];
      }
        cases[0][0] = 0;
        cases[0][1] = 0;
        cases[0][2] = 0;
        cases[0][3] = 0;
        cases[0][4] = 0;
        cases[0][5] = 0;
        cases[0][6] = 0;
        cases[0][7] = 0;
        cases[0][8] = 0;
        cases[0][9] = 0;
    }
  }
  score += scoreLig[nbLig-1]*niveau;
}

byte sel=0;

void afficheAccueil() {
  byte CLIGNOTANT;
  
  if (((millis()/500)%2)==0) {
    CLIGNOTANT = BLANC;
  } else {
    CLIGNOTANT = NOIR;
  }
  
  effacerEcran(BLANC);
  ecrireEcran("Tetris",20,5,NOIR);

  ecrireEcran("Record: ",12,13,NOIR);
  
  ecrireLettre('0'+record%10,72,13,NOIR);
  if (record>=10) ecrireLettre('0'+(record/10)%10,67,13,NOIR);
  if (record>=100) ecrireLettre('0'+(record/100)%10,62,13,NOIR);
  if (record>=1000) ecrireLettre('0'+(record/1000)%10,57,13,NOIR);


  if (sel == 0) {
    ecrireEcran("Niveau:",12,21,CLIGNOTANT);
    ecrireLettre('0'+niveau,54,21,CLIGNOTANT);
  } else {
    ecrireEcran("Niveau:",12,21,NOIR);
    ecrireLettre('0'+niveau,54,21,NOIR);
  }

  if (sel == 1) {
      if (cfgMusic) ecrireEcran("Musique:oui",5,29,CLIGNOTANT); else
      ecrireEcran("Musique:non",5,29,CLIGNOTANT);
  } else {
      if (cfgMusic) ecrireEcran("Musique:oui",5,29,NOIR); else 
      ecrireEcran("Musique:non",5,29,NOIR);      
  }
  
  if (sel == 2) {
      if (eclair) ecrireEcran("Eclairage:oui",2,37,CLIGNOTANT); else
      ecrireEcran("Eclairage:non",2,37,CLIGNOTANT);
  } else {
      if (eclair) ecrireEcran("Eclairage:oui",2,37,NOIR); else
      ecrireEcran("Eclairage:non",2,37,NOIR);
  }
  
  rafraichirEcran();
}


void initPartie() {
  randomSeed(millis());
  score  = 0;
  lignes = 0;
  typePiece=random(0,7);
  typePieceSuivante=random(0,7);
  decalNote = 0;
  indexMusique = 0;
  vitesse = tabVitesse[niveau-1];
  for (byte l=0;l<16;l++)
    for (byte c=0;c<10;c++)
      cases[l][c]=0;
  nivDebut = niveau;
  modeJeu = PARTIE;
}

//byte relache = true;

void gestionTouchesAccueil() {
  if (!touche()) {
    relache = true;
  }
  if (toucheA()) {
    modeJeu = PARTIE;
    initPartie();
  }

  if (relache && toucheHaut() && sel >0) {
    sel--;
  }

  if (relache && toucheBas() && sel<2) {
    sel++;
  }

  if ( (sel == 0) && relache && toucheDroite() && niveau <9) {
    niveau++;
  }
  if ( (sel == 0) && relache && toucheGauche() && niveau >1) {
    niveau--;
  }

  if (sel ==1 && relache && (toucheGauche() || toucheDroite())) { 
      indexMusique = 0;
      decalNote = 0;
      cfgMusic = 1 - cfgMusic;
  }

  if (sel ==2 && relache && (toucheGauche() || toucheDroite())) { 
      eclair = 1- eclair;
      if (eclair) { lumiereEcran(255);     } else { lumiereEcran(0);  }
  }

  
  if (touche()) {
    relache = false;
  }
}

void loopAccueil() {
   afficheAccueil();
   gestionTouchesAccueil();
   joueMusique(); 
}

void loopPartie() {
  if (evenement != DETRUIT) {
     gestionTouches();
     logiqueJeu();
  } else {
    tempoDetruit--;
    if (tempoDetruit==0) {
      retireLignes();
      evenement = RIEN;
    }
  }
  afficheJeu();
  sonJeu();
  joueMusique();
  cycle = cycle +1;
}


// Loop est la boucle principale, va se lancer en boucle après Setup
void loop() {
  switch (modeJeu) {
    case ACCUEIL:
        loopAccueil();
        break;
    case PARTIE:
        loopPartie();
        break;
    default:
        break;    
  }
  }

