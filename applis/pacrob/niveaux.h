#ifndef _NIVEAUX_H
#define _NIVEAUX_H


//
// Les éléments de description du labyrinthe
//
// MG = mur gauche 
#define MG   1
// MD = mur droite
#define MD   2
// MH = mur haut
#define MH   4
// MB = mur bas
#define MB   8
// FA = fantome
#define FA  16
// VI = Vitamine
#define VI  32



// Niv 1: AV av
byte niv1[] = {3,1,0,2,
  MG|MH|MB,
  MH|MB,
  MD|MH|MB};
  
// Niv 2: Av Av Ga Av
byte niv2[] = {3,2,3,2,
  0,
  0,
  MG|MH|MD,
  MG|MH|MB,
  MH|MB,
  MD|MB};

// Niv 3: Av Av Ga Av Ga Av Av
byte niv3[] = {3,2,3,0,
  MG|MH|MB,
  MH|MB,
  MH|MD,
  MG|MH|MB,
  MH|MB,
  MD|MB};

// Niv 4: avec un fantôme 
byte niv4[] = {7,1,3,0,
  MG|MH|MB,
  MH|MB,
  MH|MB,
  MH|MB,
  MH|MB,
  MH|MB,
  MD|MB|MH};


// Le niveau en cours
byte niv9[25]={7,3,14,0,
  MG|MH|MD,
  MG|MH,
  MB|MH,
  MB|MH,
  MH|MD,
  MG|MH,
  MH|MD,
  
  MB|MG,
  MB|MD,
  MG|MH,
  MH|MD,
  MG|MB,
  MB|MD,
  MG|MD,
  
  MG|MH|MB,
  MH|MB,
  MB|MD,
  MB|MG,
  MH|MB,
  MH|MB,
  MB|MD
  };

// Les différents niceaux 

byte* lesNivs[] = {niv1,niv2,niv3,niv4};


#endif
