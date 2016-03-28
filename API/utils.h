#ifndef _UTILS_H
#define _UTILS_H

// Les caractères accentués pour les chaines de caractère
#define EAIGU  ((char)0x82)
#define AGRAVE ((char)0x85)

// fonction d'attente synchrone d'appui sur une touche
void attendTouche() {
    litBoutons();
    while (etatBoutons==0) {
        litBoutons();
        delay(50);
    }
}

// fonction d'attente synchrone d'un relaché des touches
void attendRelache() {
    litBoutons();
    while (etatBoutons!=0) {
        litBoutons();
        delay(50);
    }
}


#endif
