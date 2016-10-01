#ifndef _UTILS_H
#define _UTILS_H

// Les caractères accentués pour les chaines de caractère
#define EAIGU  ((char)0x82)
#define AGRAVE ((char)0x85)

// fonction d'attente synchrone d'appui sur une touche
void attendTouche() {
    while (!toucheHaut() &&
           !toucheBas() &&
           !toucheGauche() &&
           !toucheDroite() &&
           !toucheA() &&
           !toucheB()) {
        delay(10);
    }
}

// fonction d'attente synchrone d'un relaché des touches
void attendRelache() {
    while (toucheHaut() ||
           toucheBas()||
           toucheGauche() ||
           toucheDroite() ||
           toucheA() ||
           toucheB()) {
        delay(10);
    }
}

//connaitre RAM dispo
int freeRam () {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#endif
