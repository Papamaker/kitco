// Programme clignotant LED verte
// Fait clignoter la LED verte

// pour l'environnement kitco
#include "kitco.h"



void setup() {
  initialiserKitco(0);
}

void loop() {
  LEDVerte(ALLUME);
  delai(500);
  LEDVerte(ETEINTE);
  delai(500);
}
