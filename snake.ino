#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "board.h" //tout ce qui dépend de la configuration matérielle ;-)

#include <stdio.h>
#include <stdlib.h>


/* Structures ( dans l'idéal à placer dans un .h commun à toutes les applis ) */

enum Direction { HAUT, BAS , GAUCHE, DROITE, AUCUNE };

/* Definition d'un point */
typedef struct Point Point;
struct Point

{
    int x;
    int y;
};

typedef struct Element Element;
struct Element

{
    struct  Point point;
    struct Element* nxt;
};

typedef Element* Eliste;


typedef struct Snake Snake;
struct Snake

{
    Element* tete;
    Eliste corps;
    int taille;
};


typedef struct Nourriture Nourriture;
struct Nourriture

{
    struct  Point point;
    //int valeur pour éventuellement donner plus de points;
};



/* Définition des variables globales */
Snake snake;
Nourriture nourriture;


/* Fonctions d'affichage */

/* Affichage du snake */
void afficheSnake(){

  Eliste tmp = snake.corps;
  
  while (tmp!=NULL){
    display.drawCircle(tmp->point.x, tmp->point.y, 1, BLACK);
    
    Serial.print(tmp->point.x);
    Serial.print(" ");
    Serial.print(tmp->point.y);
    tmp = tmp->nxt;
    
    Serial.print("\n");
  }
    
  
    Serial.print("\n");
    
    Serial.print("\n");
}

/* Affichage de la nourriture snake */
void afficheNourriture(){
  display.drawCircle(nourriture.point.x, nourriture.point.y, 1, BLACK);
}

/* Affichage du niveau */

void afficheNiveau(int niveau=1){
  
  //Affichage du cadre
  display.drawRect(0, 0, display.width(), display.height(), BLACK);

  //Ajouter selon le niveau des éléments causant la mort du snake (un rocher par exemple)
  
}

void genereNourriture(){

  randomSeed(analogRead(0));
  int r = random(30);
  if(nourriture.point.x == -1 && r == 5){
    nourriture.point.x = 2 + random(82);
    nourriture.point.y = 2 + random(46);
  }
  
}
void initSnake(){

  int x = display.width()/3;
  int y = display.height()/2;

  nourriture.point.x = display.width()/2;
  nourriture.point.y = display.height()/2;

  Point p;
  p.y = y;
  p.x = x;
  
  Eliste el = (Eliste) malloc(sizeof(Element));
  el -> point = p;
  el -> nxt = NULL; 
  snake.corps = el;
  
  
  for(int i = 1; i < 15; i++ )
  {
    Point pt;
    pt.y = y;
    pt.x = x - i;
    
    Eliste elmt = (Eliste) malloc(sizeof(Element));
    elmt -> point = pt;
    elmt -> nxt = snake.corps;
    snake.corps = elmt;
  }

  snake.taille = 15;
}


void bougeSnake(Direction dir ){

    Eliste tmp1 = snake.corps;
    Eliste tmp2 = snake.corps;

    Point queue;
    queue=snake.corps->point;
    
    tmp1 = tmp1->nxt;
    int i=0;
    while (tmp1->nxt != NULL){
      tmp2->point = tmp1->point;
      tmp1 = tmp1->nxt;
      tmp2 = tmp2->nxt;
    }
  
    int vectx = tmp1->point.x - tmp2->point.x;
    int vecty = tmp1->point.y - tmp2->point.y;

    int mange=0;
    Point pt;

    
    pt.x = tmp1-> point.x + vectx ;
    pt.y = tmp1-> point.y + vecty;
    
    if(dir==BAS  && vectx != 0){
      pt.x = tmp1-> point.x ;
      pt.y = tmp1-> point.y - 1;
    }

    if(dir==HAUT  && vectx != 0){
      pt.x = tmp1-> point.x ;
      pt.y = tmp1-> point.y + 1;
    }

    if(dir==GAUCHE  && vecty != 0 ){
      pt.x = tmp1-> point.x - 1;
      pt.y = tmp1-> point.y;
    }

    if(dir==DROITE  && vecty != 0){
      pt.x = tmp1-> point.x + 1;
      pt.y = tmp1-> point.y;
    }


    tmp2->point = tmp1->point;
    tmp1->point = pt;
    
    // Tester si nourriture avalée
    if(nourriture.point.x == pt.x && nourriture.point.y == pt.y){
      nourriture.point.x = -1;
      nourriture.point.y = -1;
      
      Eliste elmt = (Eliste) malloc(sizeof(Element));
      elmt -> point = queue;
      elmt -> nxt = snake.corps;
      snake.corps = elmt;

      snake.taille ++;
    }

    // Tester si contact avec le cadre
    if(pt.x - 1 == 0 || pt.y - 1 == 0 || pt.x + 1 == display.width() || pt.y + 1 == display.height() ){
        perdu();
    }

    // Tester si contact avec le snake
    
    Eliste tmp = snake.corps;
    tmp = tmp->nxt;
    while (tmp->nxt != NULL){
      if(snake.corps->point.x == tmp->point.x && snake.corps->point.y == tmp->point.y){
        perdu();
      }
      tmp = tmp->nxt;
    }
    
  
}

void perdu(){
      delay(1000);
      display.clearDisplay();
      display.setCursor(3,display.height()/2-7);
      display.println(" Perdu !");
      display.display();
      delay(5000);
      setup();
}


Direction toucheDirection(){
  if(toucheHaut()){
    return HAUT;
  }else if(toucheBas()){
    return BAS;
  }else if(toucheGauche()){
    return GAUCHE;
  }else if(toucheDroite()){
    return DROITE;
  }else{
    return AUCUNE;
  }
}
void setup()
{
    Serial.begin(9600);
    display.begin();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(3,display.height()/2-7);
    display.println(" Papamaker.fr");
    display.display();
    
    delay(1000);
    display.clearDisplay();
    initSnake();
    afficheSnake();
    afficheNourriture();
        
    display.display();
}

/* Boucle */
void loop()
{
    delay(500 - min(snake.taille,400));

    bougeSnake(toucheDirection());

    display.clearDisplay();
    afficheNiveau();
    afficheSnake();
    afficheNourriture();
    genereNourriture();
    display.display();
}



