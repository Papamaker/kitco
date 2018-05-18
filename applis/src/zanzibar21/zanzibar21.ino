
/* VERSION 1.2.5
    creation ALLERA J-l le 08/02/2018
    Modification ALLERA J-L le 12/02/2018
    Amelioration du tirage au sort des dés
   touche A  relance les des non bloqués
   touche B  bloque le dé qui possede le focuss
   haut, droite, gauche, bas déplace le bloqueur de dé
   Modification ALLERA J-L le 04/04/2018
   Ajout des scores et affichage de chaque jets des 2 joueurs
   blocage en fin de partie
*/

#include "kitco.h"

/* valeur 0 = bloque pour chaque dé
    valeur du dé de 1 a 6 pour le lancé
    Position de la colonne du dé
*/
int GestionDE[3][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 8, 32, 56}};
int Position = 1;
boolean event = false;

// score defin de jets
// nbre jetons
// score convertit
// nbre de points
int score[3][3] = {{21, 0, 0}, {0, 0, 0},{0, 0, 0}};
// gestion des scores de chaques lances des des
int player[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

boolean passe;
 int flecheX = 72;
 int resultat = 0;
int retrait =0;

boolean jeu = true;
 
 /* **** fonctions utilisees dans le jeu ***** */

void JEU(){
  while (jeu) { 
    CYCLEJOUEUR(1);
    CYCLEJOUEUR(2);
    AFFICHESCOREDE();
    SUIVANT();
    TRI(1);
    TRI(2);
    effacerEcran(BLANC);
    COMPTAGEPOINTS();
    delay(500);
    AFFICHESCORE();
    SUIVANT();
  }
  AFFICHAGESCOREFINAL();
}

void AFFICHAGESCOREFINAL(){
    int X = 4;
    int Y = 10;
    effacerEcran(BLANC);
    creerRectangle(2, 4, 82, 46,0, NOIR);
    setStr("SCORE FINAL",9,1,NOIR);
    
    if (score[0][1] == 0){
      
      setStr("* Joueur 1 *",6,Y+5,NOIR);  
    }
    else{
      setStr("* Joueur 2 *",6,Y+5,NOIR);  
    }
    setStr("VAINQUEUR",14,Y+20,NOIR);
    rafraichirEcran();
  }


void CHARGE(){
 
    if(score[1][1] > score[2][1]){
        retrait = score[1][2];
        POT(); 
        score[0][2] = score[0][2] + retrait; 
     }
      
    if(score[1][1] < score[2][1]){
        retrait = score[2][2];
        POT();
        score[0][1] = score[0][1] + retrait;
     }
}

void POT(){
  if (score[0][0] > retrait){
      score[0][0] = score[0][0] - retrait;
  }
  else{
    retrait = score[0][0];
    score[0][0] = 0;
  }
}

void DECHARGE(){
    if(score[1][1] > score[2][1]){
    retrait = score[1][2];
    if(score[0][1] <= retrait){
      score[0][1] = 0;
      score[0][2] = 21;
      jeu = false;
    }
    else {
      score[0][1] = score[0][1] - retrait;
      score[0][2] = score[0][2] + retrait;
    }
    
  }
  else{
    retrait = score[2][2];
    if(score[0][2] <= retrait){
      score[0][2] = 0;
      score[0][1] = 21;
      jeu = false;
    }
    else {
      score[0][2] = score[0][2] - retrait;
      score[0][1] = score[0][1] + retrait;
    }  
  }
}

void COMPTAGEPOINTS(){
  if (score[0][0] > 0){
      CHARGE();     
  }
  else{
    DECHARGE();
  }
}

void AFFICHESCORE(){
    int X = 4;
    int Y = 10;
    effacerEcran(BLANC);
    creerRectangle(2, 4, 82, 46,0, NOIR);
    setStr("SCORE",30,1,NOIR);
    setStr("POT     : ",X,Y,NOIR);
    setStr("Joueur1 : ",X,Y+12,NOIR);
    setStr("Joueur2 : ",X,Y+24,NOIR);
    ecrireLettre( '0' + score[0][0] % 10, X+60, Y, NOIR);
    ecrireLettre( '0' + score[0][1] % 10, X+60, Y+12, NOIR);
    ecrireLettre( '0' + score[0][2] % 10, X+60, Y+24, NOIR);
    if (score[0][0]>=10) ecrireLettre('0'+(score[0][0]/10)%10,X+55,Y,NOIR);
    if (score[0][1]>=10) ecrireLettre('0'+(score[0][1]/10)%10,X+55,Y+12,NOIR);
    if (score[0][2]>=10) ecrireLettre('0'+(score[0][2]/10)%10,X+55,Y+24,NOIR);
    rafraichirEcran();

}

void TRI(int joueur) {
  int echange = 0;
  if (player[joueur][0] < player[joueur][1]) {
    echange = player[joueur][0];
    player[joueur][0] = player[joueur][1];
    player[joueur][1] = echange;
  }

  if (player[joueur][1] < player[joueur][2]) {
    echange = player[joueur][2];
    player[joueur][2] = player[joueur][1];
    player[joueur][1] = echange;
  }

  if (player[joueur][0] < player[joueur][1]) {
    echange = player[joueur][0];
    player[joueur][0] = player[joueur][1];
    player[joueur][1] = echange;
  }
resultat = (player[joueur][0] * 100) + (player[joueur][1] * 10) + player[joueur][2];

POINT(joueur,resultat);



}

void POINT(int joueur, int resultat) {

  switch (resultat) {
    case 421:
      score[joueur][1] = 999;
      score[joueur][2] = 10;
      break;
    case 111:
      score[joueur][1] = 907;
      score[joueur][2] = 7;
      break;
    case 666:
      score[joueur][1] = 906;
      score[joueur][2] = 6;
      break;
    case 555:
      score[joueur][1] = 905;
      score[joueur][2] = 5;
      break;
    case 444:
      score[joueur][1] = 904;
      score[joueur][2] = 4;
      break;
    case 333:
      score[joueur][1] = 903;
      score[joueur][2] = 3;
      break;
    case 222:
      score[joueur][1] = 902;
      score[joueur][2] = 2;
      break;
    case 611:
      score[joueur][1] = 806;
      score[joueur][2] = 6;
      break;
    case 511:
      score[joueur][1] = 805;
      score[joueur][2] = 5;
      break;
    case 411:
      score[joueur][1] = 804;
      score[joueur][2] = 4;
      break;
    case 311:
      score[joueur][1] = 803;
      score[joueur][2] = 3;
      break;
    case 211:
      score[joueur][1] = 802;
      score[joueur][2] = 2;
      break;
    case 654:
      score[joueur][1] = 704;
      score[joueur][2] = 2;
      break;
    case 543:
      score[joueur][1] = 703;
      score[joueur][2] = 2;
      break;
    case 432:
      score[joueur][1] = 702;
      score[joueur][2] = 2;
      break;
    case 321:
      score[joueur][1] = 701;
      score[joueur][2] = 2;
      break;
    case 221:
      score[joueur][1] = 221;
      score[joueur][2] = 2;
      break;
    default:
      score[joueur][1] = resultat;
      score[joueur][2] = 1;
      break;
  }
}

void AFFICHESCOREDE(){
  // a afficher apres la jet du deuxieme joueur
  int X = 4;
    int Y = 10;
    effacerEcran(BLANC);
    creerRectangle(2, 4, 82, 46,0, NOIR);
    setStr("SCORE",30,1,NOIR);
    setStr("des Jets",20,9,NOIR);
    setStr("Joueur1 : ",X,Y+12,NOIR);
    setStr("Joueur2 : ",X,Y+24,NOIR);
   
    ecrireLettre( '0' + player[1][0] , X+55, Y+12, NOIR);
    ecrireLettre( '0' + player[1][1] , X+60, Y+12, NOIR);
    ecrireLettre( '0' + player[1][2] , X+65, Y+12, NOIR);

    ecrireLettre( '0' + player[2][0] , X+55, Y+24, NOIR);
    ecrireLettre( '0' + player[2][1] , X+60, Y+24, NOIR);
    ecrireLettre( '0' + player[2][2] , X+65, Y+24, NOIR);
    
    rafraichirEcran();
    delay(500);
}

 void SUIVANT(){
  passe = false;
  
  // effacerEcran(0);
  ligneEcran(flecheX + 7,40,flecheX + 8,40,1);
  ligneEcran(flecheX + 8,41,flecheX + 9,41,1);
  ligneEcran(flecheX,42,flecheX + 10,42,1);
  ligneEcran(flecheX + 8,43,flecheX + 9,43,1);
  ligneEcran(flecheX + 7,44,flecheX + 8,43,1);
  rafraichirEcran();
  while (!passe) {
    if (toucheA()) {
      passe = true;
    }
  }
   effacerEcran(BLANC);
}


void ACCUEIL(){
  
  effacerEcran(BLANC);
  ligneEcran(2, 3, 44, 3, NOIR);
  ligneEcran(2, 5, 42, 5, NOIR);

  ligneEcran(44, 3, 0, 32, NOIR);
  //ligneEcran(40, 5, 0, 28, NOIR);
  
  ligneEcran(2, 30, 82, 30, NOIR);
  ligneEcran(2, 32, 82, 32, NOIR);
  
  rafraichirEcran();

    for (int y = 0; y<5; y++){
      setStr("ANZIBAR 21",20,17,1);
      rafraichirEcran();
      delay(300);
      setStr("ANZIBAR 21",20,17,0);
      rafraichirEcran();
      delay(300);
    }
}

void CARRE(int x, boolean plein, boolean color) {
  creerRectangle(x, 39, x + 6, 43, plein, color);
}
void  ENVELOPPEDE(int x) {
  creerRectangle(x, 17, x + 21, 34, 0, NOIR);
}

void DE1(int x, int y) {
  creerRectangle(x + 7, y + 7, x + 11, y + 9, 1, NOIR);
}

void DE2(int x, int y) {
  creerRectangle(x + 2, y + 3, x + 6, y + 5, 1, NOIR);
  creerRectangle(x + 12, y + 12, x + 16, y + 14, 1, NOIR);
}

void DE3(int x, int y) {
  DE2(x, y);
  DE1(x, y);
}

void DE4(int x, int y) {
  creerRectangle(x + 2, y + 3, x + 6, y + 5, 1, NOIR);
  creerRectangle(x + 2, y + 12, x + 6, y + 14, 1, NOIR);
  creerRectangle(x + 12, y + 3, x + 16, y + 5, 1, NOIR);
  creerRectangle(x + 12, y + 12, x + 16, y + 14, 1, NOIR);
}

void DE5(int x, int y) {
  DE4(x, y);
  DE1(x, y);
}

void DE6(int x, int y) {
  DE4(x, y);
  creerRectangle(x + 2, y + 7, x + 6, y + 9, 1, NOIR);
  creerRectangle(x + 12, y + 7, x + 16, y + 9, 1, NOIR);
}


void EFFACEDE() {
  creerRectangle(7, 18, 26, 33, 1, BLANC);
  creerRectangle(31, 18, 50, 33, 1, BLANC);
  creerRectangle(55, 18, 74, 33, 1, BLANC);
}

void DECOR()
{
  // Cadre
  effacerEcran(BLANC);
  creerRectangle(2, 13, 80, 45, 0, NOIR);

  //  dés
  ENVELOPPEDE(6);
  ENVELOPPEDE(30);
  ENVELOPPEDE(54);

  //carré de blocage
  CARRE(14, 0, NOIR);
  CARRE(38, 0, NOIR);
  CARRE(62, 0, NOIR);
}

void GESTIONTOUCHES() {


  event = false;

  while (!event) {
    delay(200);

    if ( toucheGauche() ) {
      switch (Position) {
        case 2:
          Position--;
          creerRectangle(37, 38, 45, 44, 0, BLANC);
          creerRectangle(13, 38, 21, 44, 0, NOIR);
          break;
        case 3:
          Position--;
          creerRectangle(61, 38, 69, 44, 0, BLANC);
          creerRectangle(37, 38, 45, 44, 0, NOIR);
          break;
        default:
          break;
      }
    }

    if ( toucheDroite() ) {
      switch (Position) {
        case 1:
          Position++;
          creerRectangle(13, 38, 21, 44, 0, BLANC);
          creerRectangle(37, 38, 45, 44, 0, NOIR);
          break;
        case 2:
          Position++;
          creerRectangle(37, 38, 45, 44, 0, BLANC);
          creerRectangle(61, 38, 69, 44, 0, NOIR);
          break;
        default:
          break;
      }
    }

    if (toucheB() ) {
      switch (Position) {
        case 1:
          if (GestionDE[0][1] == 0) {
            creerRectangle(15, 40, 20, 42, 1, NOIR);
            GestionDE[0][1] = 1;
          }
          else {
            creerRectangle(15, 40, 20, 42, 1, BLANC);
            GestionDE[0][1] = 0;
          }

          break;
        case 2:
          if (GestionDE[0][2] == 0) {
            creerRectangle(39, 40, 44, 42, 1, NOIR);
            GestionDE[0][2] = 1;
          }
          else {
            creerRectangle(39, 40, 44, 42, 1, BLANC);
            GestionDE[0][2] = 0;
          }
          break;
        case 3:
          if (GestionDE[0][3] == 0) {
            creerRectangle(63, 40, 68, 42, 1, NOIR);
            GestionDE[0][3] = 1;
          }
          else {
            creerRectangle(63, 40, 68, 42, 1, BLANC);
            GestionDE[0][3] = 0;
          }
          break;
        default:
          break;
      }
    }

    if (toucheA()) {
      event = true;
      // fin de la gestion des touches
     
    }
    rafraichirEcran();
  }
}

void LANCE() {
  for (int j = 1; j < 4; j++) {
    if (!GestionDE[0][j] ) {
      GestionDE[1][j] = random(1, 7);
    }
  }

}
void AFFICHEDE() {
  // generation des valeurs de des a afficher
  EFFACEDE();
  for (int k = 1; k < 4; k++) {

    switch (GestionDE[1][k]) {
      case 1:
        DE1(GestionDE[2][k], 17);
        break;
      case 2:
        DE2(GestionDE[2][k], 17);
        break;
      case 3:
        DE3(GestionDE[2][k], 17);
        break;
      case 4:
        DE4(GestionDE[2][k], 17);
        break;
      case 5:
        DE5(GestionDE[2][k], 17);
        break;
      case 6:
        DE6(GestionDE[2][k], 17);
        break;
      default:
        break;
    }
  }
}

void REINIT() {
  // remise a zero pour le blocage ete la valeur des des
  for (int b = 0 ; b < 4 ; b++) {
    GestionDE[0][b] = 0;
    GestionDE[1][b] = 0;
  }

  creerRectangle(15, 40, 20, 42, 1, BLANC);
  creerRectangle(39, 40, 44, 42, 1, BLANC);
  creerRectangle(63, 40, 68, 42, 1, BLANC);
}

void AFFICHEPHRASE(int numero) {
  if (numero == 3) {
    setStr("SUIVANT      ", 4, 3, 1);
  }
  else {
    setStr("             ", 4, 3, 1);
    setStr("Lance : ", 4, 3, 1);
    ecrireLettre( '0' + numero % 10, 49, 3, 1);
  }
  rafraichirEcran();
}


  
  void AFFICHAGESCOREJETONS(){
    int X = 4;
    int Y = 10;
    effacerEcran(BLANC);
    creerRectangle(2, 4, 82, 46,0, NOIR);
    setStr("SCORE",30,1,NOIR);
    setStr("JETONS",25,1,NOIR);
    setStr("POT     : ",X,Y,NOIR);
    setStr("Joueur1 : ",X,Y+12,NOIR);
    setStr("Joueur2 : ",X,Y+24,NOIR);
    ecrireLettre( '0' + score[0][0] % 10, X+60, Y, NOIR);
    ecrireLettre( '0' + score[0][1] % 10, X+60, Y+12, NOIR);
    ecrireLettre( '0' + score[0][2] % 10, X+60, Y+24, NOIR);
    if (score[0][0]>=10) ecrireLettre('0'+(score[0][0]/10)%10,X+55,Y,NOIR);
    if (score[0][1]>=10) ecrireLettre('0'+(score[0][1]/10)%10,X+55,Y+12,NOIR);
    if (score[0][2]>=10) ecrireLettre('0'+(score[0][2]/10)%10,X+55,Y+24,NOIR);
    rafraichirEcran();
  }

  
void CYCLEJOUEUR(int joueur){
  DECOR();
  rafraichirEcran();
  for (int T = 1 ; T < 4 ; T++) {
    LANCE();
    AFFICHEDE();
    AFFICHEPHRASE(T);
    GESTIONTOUCHES();
  }

  player[joueur][0] = GestionDE[1][1];
  player[joueur][1] = GestionDE[1][2];
  player[joueur][2] = GestionDE[1][3];
  
  // remise a zero pour le blocage et la valeur des des
  REINIT();
}

// ****************************
// cycles de base
//*****************************

void setup() {
  // put your setup code here, to run once:
  initialiserKitco(0);
  randomSeed(analogRead(5));
  ACCUEIL();
  SUIVANT();

}

void loop() {
  // put your main code here, to run repeatedly:

   JEU();
  
}
