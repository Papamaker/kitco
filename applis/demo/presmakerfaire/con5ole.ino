#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "board.h" //tout ce qui dépend de la configuration matérielle ;-)


// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

// séquence de démarrage pour tester LEDs et écran et buzzer
void animDebut() {
  
  setLEDBleue1(HIGH);
  tone(BUZZER,1000,500);
  delay(500);
  setLEDBleue1(LOW);
  setLEDJaune2(HIGH);
  tone(BUZZER,1500,500);
  delay(500);
  setLEDJaune2(LOW);
  setLEDVerte3(HIGH);
  tone(BUZZER,2000,500);
  delay(500);
  setLEDVerte3(LOW);
  setLEDRouge4(HIGH);
  tone(BUZZER,2500,500);
  delay(1000);
  setLEDRouge4(LOW);
  noTone(BUZZER);
  delay(500);

  setLEDBleue1(HIGH);
  setLEDJaune2(HIGH);
  setLEDVerte3(HIGH);
  setLEDRouge4(HIGH);
  tone(BUZZER,4000,500);
  delay(1000);
  noTone(BUZZER);
  setLEDBleue1(LOW);
  setLEDJaune2(LOW);
  setLEDVerte3(LOW);
  setLEDRouge4(LOW);

  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Papamaker.fr");
  display.display();
  for (char i=0;i<101;i++) {
    setLcdLight(i);
  }

  delay(1000);
  
  display.clearDisplay();
  display.display();  
  display.println("Papamaker.fr");
  display.display(); 

  
}

void setup()   {

  // initialisations liées au matériel
  initBoard();
  
  display.begin();

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.clearDisplay();   // clears the screen and buffer
  display.display(); // show splashscreen

  animDebut();
}


// mode de règlage de l'intensité lumineuse de l'écran
void reglageIntensite() {
  litBoutons();
  while (!toucheSelect()) {
    litBoutons();
    if (toucheHaut() && (retroLcd<100)) {
      setLcdLight(retroLcd + 1);
    }
    if (toucheBas() && (retroLcd >0)) {
      setLcdLight(retroLcd -1);
    }
    delay(50);
  }
} 



void loop() {

  litBoutons();
  if (toucheStart()) {
    display.clearDisplay();
    display.display();  
    display.println("reglage Lumiere");
    display.display(); 
    reglageIntensite();
    display.clearDisplay();
    display.display();  
    display.println("Papamaker.fr");
    display.display(); 
  }
  delay(200);
}
