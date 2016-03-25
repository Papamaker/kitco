#ifndef _UI_H
#define _UI_H

// toute l'initialisation dépendant du matériel
void initBoard();

// Pour ajuster l'éclairage à pourcent(age) 
void setLcdLight(unsigned char pourcent);

// Lecture globale des boutons stocké en cache
void litBoutons();

// Est-ce que les touches sont appuyées 1: oui 0: non
char toucheHaut()   ;
char toucheBas()    ;
char toucheGauche() ;
char toucheDroite() ;
char toucheStart()  ;
char toucheSelect() ; 
char toucheA()      ; 
char toucheB()      ; 

// passe les LEDs à HIGH ou LOW
void setLEDBleue1(byte state);
void setLEDJaune2(byte state);
void setLEDVerte3(byte state);
void setLEDRouge4(byte state);

#endif
