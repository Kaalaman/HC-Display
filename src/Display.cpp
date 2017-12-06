//Version 1.0.0a1
//---Alle benoetigten Biliotheken einbinden--------------------------------------
#include <Arduino.h>
#include <TFT_HX8357.h>
#include <Wire.h>
//
//---Angeschlossene Obekte definieren----------------------------------------------
TFT_HX8357 tft = TFT_HX8357();

//
//---Globale-Variablen deklarieren -------------------------------------------------
//
const byte MY_ADRESS = 21;
byte bModi = 0;
byte bPosX = 0;
byte bPosY = 0;
byte bSwit = 0;
byte bPoti = 0;
byte bBatU = 0;

float fBatU = 0;

unsigned long timerBatterie;

bool bModusFlugSetup = false;
bool bModusLeer = false ;
bool bModusConfig = false;

// Funtionen deklarieren
void receiveEvent (int);
void vModusFlugLoop ();
void vModusFlugOnce ();

//
//--- Setup, wird einmal durchlaufen----------------------------------------------
//
void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(115200);


  tft.init();
  tft.setRotation(0);

  Wire.begin (MY_ADRESS);
  Wire.onReceive (receiveEvent);

}//Ende Setup


//
//---Schleife, Loop. Wird andauernt durchlaufen. Immer im Kries-------------------------
//
void loop()
{
  Serial.print(bModi);
  Serial.println();

  switch (bModi) {
    case 0 : // Leer-Modus , also ohne Sinn, nur zum start

      bModusFlugSetup = false;
      bModusConfig = false ;

      if (bModusLeer == false) {
        tft.fillScreen(TFT_BLACK);
        } // ende if, nur einmal Bildschirm füllen

      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setTextSize(8);
      tft.setCursor(50, 240);
      tft.print("Start");
      bModusLeer = true;

      break;

    case 1 :

      bModusLeer = false;
      bModusConfig = false;

      if (bModusFlugSetup == false ) {
        vModusFlugOnce();
        } //ende if, nur einmal Setup

      vModusFlugLoop();
      break;

    case 2 :

      bModusLeer = false;
      bModusFlugSetup = false;

      if (bModusConfig == false) {
        tft.fillScreen(TFT_BLACK);
        } // ende if, nur einmal Bildschirm füllen

      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setTextSize(8);
      tft.setCursor(50, 170);
      tft.print("Modus");
      tft.setCursor (30, 250);
      tft.print ("Config");
      bModusConfig = true;

      //hier dann Code zum senden an Fernbedienung
      //um Eingaben zu prüfen

      //Wire.write ();

      // tu es
      break;

    case 3 :
      // tu es
      break;
  } //ende switch Case

}//Ende Loop




void receiveEvent (int howMany)
{
  bModi = Wire.read ();
  bPosX = Wire.read ();
  bPosY = Wire.read ();
  bSwit = Wire.read ();
  bPoti = Wire.read ();
  bBatU = Wire.read ();

}
void vModusFlugOnce () {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  tft.drawRect(5, 5, 230, 230, TFT_WHITE);
  tft.drawRect(240, 5, 75, 230, TFT_WHITE);

  tft.setCursor(5, 250);
  tft.print("X= ");

  tft.setCursor(95, 250);
  tft.print("Y= ");


  tft.setCursor(5, 330);
  tft.setTextSize(2);
  tft.print("Batterie");
  tft.drawRect(5, 360, 30, 105, TFT_WHITE);
  tft.drawRect(37, 360, 30, 105, TFT_WHITE);
  tft.drawRect(69, 360, 30, 105, TFT_WHITE);

  tft.setTextSize(1);
  tft.setCursor(8, 468);
  tft.print("3,3V");
  tft.setCursor(41, 468);
  tft.print("3,3V");
  tft.setCursor(74, 468);
  tft.print("3,3V");

  tft.setTextSize (2);

}
void vModusFlugLoop () {

  unsigned long time = micros();
  tft.setTextSize(2);

  tft.setCursor(30, 250);
  tft.print(bPosX);
  tft.print("  ");


  tft.setCursor(120, 250);
  tft.print(bPosY);
  tft.print("  ");

  tft.setCursor(195, 250);
  tft.print("S= ");
  tft.print(bPoti);
  tft.print("  ");

  tft.fillRect(241, 6, 73, 228, TFT_BLACK); //Ausgabe aktueller Schub / max Schub
  tft.drawLine(241, 233-bPoti, 313, 233-bPoti, TFT_YELLOW);
  tft.drawLine(241, 233-(0.3333*233), 313, 233-(0.3333*233), TFT_RED);


  if (millis() - timerBatterie > 500) {

    tft.setTextSize(1);
    fBatU = (bBatU / 254.00) + 3.3; //Berechnung 1 Spannung von 3,3->4,3

    tft.setCursor(8, 350); //Ausgabe aktuelle Spannung Zelle 01
    tft.print(fBatU);
    tft.fillRect(6, 361, 28, 103, TFT_BLACK);
    tft.drawLine(6, 474-(bBatU/2), 33, 474-(bBatU/2), TFT_YELLOW);

    timerBatterie = millis();
  }// ende if, nur alle 0,5s einmal Baterie

  tft.setCursor(5, 280);
  tft.print(micros()-time);
  tft.print("   ");

  bModusFlugSetup = true;
}//Ende FlugmousLoop
