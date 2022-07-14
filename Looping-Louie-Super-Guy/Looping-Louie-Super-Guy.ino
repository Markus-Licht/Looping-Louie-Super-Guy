#include <PCM.h>
#include "music.h"
#include <PCD8544.h>
PCD8544 lcd;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 9
#define LED_COUNT 72


int Motor = 6;                              //Eingänge
int Modusschalter = 8;
int Wippe = 2;
int As6 = 12;
int As7 = 13;

int MerkerModus = 0;                        //Merker
int Modus = 0;
int StatusvariableModus = 1;
int StatusvariableWippe = 0;
int Geschwindigkeit = 255;
int MerkerDurchlauf = 0;
int prozent = 0;
int reset = 50;


int LED_pW = 9;                             //LED_proWippe

int zufall;

int Merker[8] = {1, 1, 1, 1, 1, 1, 1, 1};    //Spielercoins
int Players[8] = {3, 3, 3, 3, 3, 3, 3, 3};
int Playersm[8] = {1, 1, 1, 1, 1, 1, 1, 1};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void resetter() {
  if (reset > 100) {
    MerkerModus--;
    StartColor();
    for (int i = 0; i < 8; i++) {
      Players[i] = {3};
      Playersm[i] = {1};
      reset = 0;

    }
  }
  if (digitalRead(Modusschalter) == 1) {
    reset++;
  }
  if (digitalRead(Modusschalter) == 0) {
    reset = 0;
  }
}

void coinser(int Eingang, int Player) {                             //Spielercoins werden abgezogen und Sound abgespielt

  if (Merker[Player] == 1 && Eingang == 1 && Players[Player] > 0) {
    Merker[Player] = 0;
    Players[Player]--;
    startPlayback(coin, sizeof(coin));
    delay(100);
  }
  if (Eingang == 0 && Merker[Player] == 0) {
    Merker[Player] = 1;
    delay(100);
  }
}

void Geschwindigkeitseinstellung() {                              //Geschwindigkeit im Modus3 wir eingestellt

  if (MerkerDurchlauf == 1000) {
    MerkerDurchlauf = 0;
    Geschwindigkeit = random(75, 300);
    if (Geschwindigkeit > 255) {
      Geschwindigkeit = 0;
    }
  }
  if (Geschwindigkeit == 0 && MerkerDurchlauf == 1) {
    MerkerDurchlauf = 600;
  }
  prozent = map(Geschwindigkeit, 0, 255, 0, 100);
}

void colorWipe(uint32_t color, int wait, int i, int ende) {         //Farbverlauf

  for (; i <= ende; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

void RGB() {

  for (int i = 0; i < 9; i++) {                                   //RBG färben
    if ((Players[i] == 0) && (Playersm[i] == 1)) {
      colorWipe(strip.Color(0, 0, 0), 50, (i * LED_pW), (i * LED_pW + LED_pW - 1));
      strip.show();
      Playersm[i] = 0;
    }
  }
}

void StartColor() {
  colorWipe(strip.Color(255, 215, 0), 5, 0, 8);
  colorWipe(strip.Color(139, 0, 139), 5, 9, 17);
  colorWipe(strip.Color(255, 0, 0), 5, 18, 26);
  colorWipe(strip.Color(255, 215, 0), 5, 27, 35);
  colorWipe(strip.Color(102, 205, 0), 5, 36, 44);
  colorWipe(strip.Color(139, 0, 139), 5, 45, 53);
  colorWipe(strip.Color(255, 0, 0), 5, 54, 62);
  colorWipe(strip.Color(102, 205, 0), 5, 63, 71);

}



void Modusumstellen() {                                                   //Entprellen des Modustasters

  if (digitalRead(Modusschalter) == 1 && StatusvariableModus == 1) {
    StatusvariableModus = 0;
    delay(50);
  }

  if (digitalRead(Modusschalter) == 0 && StatusvariableModus == 0 && reset < 100) {
    StatusvariableModus = 1;
    MerkerModus++;
    delay(50);
  }

}

void Wipptaster() {                                                   //Entprellen des Wipptasters und abspielen des Sounds

  if (digitalRead(Wippe) == 1 && StatusvariableWippe == 1) {
    startPlayback(sprung , sizeof(sprung));
    StatusvariableWippe = 0;
    delay(5);
  }
  if (digitalRead(Wippe) == 0 && StatusvariableWippe == 0) {
    StatusvariableWippe = 1;
    delay(5);
  }
}


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(Motor, OUTPUT);
  pinMode(Modusschalter, INPUT);
  pinMode(Wippe, INPUT);
  pinMode(As6, INPUT);
  pinMode(As7, INPUT);


  lcd.begin(48, 84);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)





}



void Modi() {                                                     //Modi werden eingestellt, LCD beschrieben und Motorgeschwindigkeit eingestellt

  switch (MerkerModus) {
    case 0:
      if (Modus == 0) {

        lcd.setCursor(0, 0);
        lcd.print(" Looping Louie");
        lcd.setCursor(0, 1);
        lcd.print("   Superstar");
        lcd.setCursor(0, 3);
        lcd.print("  Modus: AUS");
        lcd.setCursor(0, 5);
        lcd.print(" Geschw.: 0%  ");
        Modus = 1;
        analogWrite(Motor, 0);
        delay(200);





      }

      break;

    case 1:
      if (Modus == 1) {

        startPlayback(go, sizeof(go));
        lcd.setCursor(0, 3);
        lcd.print("   Modus: 1  ");
        lcd.setCursor(0, 5);
        lcd.print(" Geschw.: 50% ");
        Modus = 2;
        analogWrite(Motor, 100);
      }

      break;


    case 2:
      if (Modus == 2) {
        startPlayback(go, sizeof(go));
        lcd.setCursor(0, 3);
        lcd.print("   Modus: 2  ");
        lcd.setCursor(0, 5);
        lcd.print(" Geschw.: 100% ");
        Modus = 3;
        analogWrite(Motor, 255);
      }

      break;


    case 3:
      if (Modus == 3 && MerkerDurchlauf == 0) {
        startPlayback(go, sizeof(go));
        lcd.setCursor(0, 3);
        lcd.print("   Modus: 3  ");
        lcd.setCursor(0, 5);
        lcd.print(" Geschw.: ");
        lcd.print(prozent);
        lcd.print("% ");
      }
      analogWrite(Motor, Geschwindigkeit);
      MerkerDurchlauf++;
      break;

    default:
      startPlayback(go, sizeof(go));
      MerkerModus = 0;
      MerkerDurchlauf = 0;
      Modus = 0;



  }
}

void loop()
{
  Serial.println(reset);
  Serial.println(StatusvariableModus);
  int WP1 = digitalRead(A0);
  int WP2 = digitalRead(A1);
  int WP3 = digitalRead(A2);
  int WP4 = digitalRead(A3);
  int WP5 = digitalRead(A4);
  int WP6 = digitalRead(A5);
  int WP7 = digitalRead(As6);
  int WP8 = digitalRead(As7);


  resetter();
  Geschwindigkeitseinstellung();
  Modusumstellen();
  RGB();
  Modi();
  Wipptaster();
  coinser(WP1, 0);
  coinser(WP2, 1);
  coinser(WP3, 2);
  coinser(WP4, 3);
  coinser(WP5, 4);
  coinser(WP6, 5);
  coinser(WP7, 6);
  coinser(WP8, 7);

}
