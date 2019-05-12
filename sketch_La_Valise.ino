//////////////////////////////////////////////////////////////////////////////////////
/*
   La Valise (version 2019.05.11b - 23h36)
   Copyright 2013, 2014, 2015, 2016, 2019 - Eric Sérandour

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
//////////////////////////////////////////////////////////////////////////////////////
/*
  Compilation réussie avec Arduino 1.6.13
  Compilation réussie avec Arduino 1.8.5
  Type de carte : "maniacbug" Mighty 1284p 16 Mhz using Optiboot
  Pour installer cette carte => https://github.com/JChristensen/mighty-1284p/tree/v1.6.3
/*
//////////////////////////////////////////////////////////////////////////////////////

/*
  L'électronique (non exhaustif) :
  
  * Microcontroleur ATMega 1284P PU : http://serandour.com/atmega1284p-et-arduino.htm
  * Quartz 16 Mhz
  * MicroSD Card Adapter de chez Catalex
  * Afficheur LCD 4x20 caractères.
  * Clavier 12 touches matricielles.
  * Circuit intégré PCF8574P (interface parallèle 8 bits I2C)
  * Horloge RTC DS1307
  * Module BMP180 de chez Adafruit (capteur de pression + température)
  * Capteur de température LM35CZ
  * Capteur de lumière GA1A12S202 de chez Adafruit

  Le circuit :
  
    Les entrées analogiques :
  =============================
  * EA0 sur Arduino (broche 40 de l'ATMega 1284P)
  * EA1 sur Arduino (broche 39 de l'ATMega 1284P)
  * EA2 sur Arduino (broche 38 de l'ATMega 1284P)
  * EA3 sur Arduino (broche 37 de l'ATMega 1284P)
  * EA4 sur Arduino (broche 36 de l'ATMega 1284P)
  * EA5 sur Arduino (broche 35 de l'ATMega 1284P)
  * EA6 sur Arduino (broche 34 de l'ATMega 1284P)
  * EA7 sur Arduino (broche 33 de l'ATMega 1284P)
 
    Les entrées / sorties numériques :
  ======================================
  * Ecran LCD sur D23, D22, D0, D1, D2, D3 sur Arduino,
    ce qui correspond aux broches 29, 28, 1, 2, 3, 4 de l'ATMega 1284P.
    Brochage de l'afficheur compatible HD44780 :
    1  : Vss : GND (fil noir)
    2  : Vdd : Power Supply +5V (fil rouge)
    3  : V0 : Contrast Adjust. Point milieu du potentiomètre (fil gris)
    4  : RS : Register Select Signal. => D23 sur Arduino (fil bleu)
    5  : R/W : Data Read/Write. Relié à GND (fil noir)
    6  : E : Enable Signal. => D22 sur Arduino (fil vert)
    7  : non connecté
    8  : non connecté
    9  : non connecté
    10 : non connecté
    11 : DB4 : Data Bus Line. => D0 sur Arduino (fil jaune)
    12 : DB5 : Data Bus Line. => D1 sur Arduino (fil orange)
    13 : DB6 : Data Bus Line. => D2 sur Arduino (fil rouge)
    14 : DB7 : Data Bus Line. => D3 sur Arduino (fil marron)
    15 : LED+ : Power supply for BKL(+). Relié à +5V (fil rouge)
    16 : LED- : Power supply for BKL(-). Relié à GND (fil noir)
 
  * Buzzer sur D15 sur Arduino (broche 21 de l'ATMega 1284P).
    D15 -> R = 1 kohms -> Buzzer -> GND.

  * Clavier 12 touches matricielles.
  * Circuit intégré PCF8574P (interface parallèle 8 bits I2C).  
    Le circuit PCF8574P :
    A0  -> GND
    A1  -> GND
    A2  -> GND
    P0  -> Ligne 1 du clavier
    P1  -> Ligne 2 du clavier
    P2  -> Ligne 3 du clavier
    P3  -> Ligne 4 du clavier
    P4  -> Colonne 1 du clavier
    P5  -> Colonne 2 du clavier
    P6  -> Colonne 3 du clavier
    P7  -> Non connecté
    Vss -> GND
    Vdd -> 5V
    SDA -> D17 sur Arduino (broche 23 de l'ATMega 1284P)
    et SDA -> R de 4,7 kilohms -> 5 V
    SCL -> D16 sur Arduino (broche 22 de l'ATMega 1284P) 
    et SCL -> R de 4,7 kilohms -> 5 V
    INT -> Non connecté
 
  * La carte SD est reliée au bus SPI (bus série normalisé) de la manière suivante :
  ** SS - D10 du shield Snootlab (voir la constante CHIP_SELECT)
     => D4 sur Arduino (broche 5 de l'ATMega 1284P)
  ** MOSI - D11 du shield Snootlab (Master Out Slave In : Sortie de données séries)
     => D5 sur Arduino (broche 6 de l'ATMega 1284P)
  ** MISO - D12 du shield Snootlab (Master In Slave Out : Entrée de données séries)
     => D6 sur Arduino (broche 7 de l'ATMega 1284P)
  ** SCK - D13 du shield Snootlab (Serial ClocK : Pour synchroniser les échanges de données)
     => D7 sur Arduino (broche 8 de l'ATMega 1284P)


    Les ports de communication :
  ======================================
  L'horloge RTC du shield mémoire est reliée à :
  * Bus I2C : SDA (D17 sur Arduino, ce qui correspond à la broche 23 de l'ATMega 1284P)
  * Bus I2C : SCL (D16 sur Arduino, ce qui correspond à la broche 22 de l'ATMega 1284P)
  
  
    Bilan :
  ===========
    Sont utilisées  : A0, A1, A2, A3, A4, A5, A6, A7,   (Entrées analogiques)
                      D23, D22, D0, D1, D2, D3,         (Ecran LCD)
                      D15,                              (Buzzer)
                      D8, D9,                           (Port série)
                      D17, D16,                         (I2C : SDA/SCL)
                      D4, D5, D6, D7,                   (SPI pour carte SD)
                      D21, D20, D19, D18                (Entrées numériques)
   Sont disponibles : D10, D11,
                      D12, D13, D14
*/

#include <SPI.h> // Nécessaire avec Arduino 1.6.12

// *** Afficheur LCD
#include <LiquidCrystal.h>        // On importe la bibliothèque
// Initialisation de la bibliothèque avec les numéros de broches utilisées
LiquidCrystal lcd(23,22,0,1,2,3); // Correspond sur l'afficheur à RS,Enable,DB4,DB5,DB6,DB7
const byte NB_LIGNES_LCD = 4;     // Nombre de lignes de l'écran
const byte NB_COLONNES_LCD = 20;  // Nombre de colonnes de l'écran
byte ligneLCD = 0;                // Numéro de la ligne
byte colonneLCD = 0;              // Numéro de la colonne
char ligne[NB_COLONNES_LCD]; // Pour le formatage des nombres avec la fonction dtostrf()


// *** Buzzer
const byte BUZZER = 15;


// *** Clavier
#include <Wire.h>    // On importe la bibliothèque Wire pour l'I2C
#define ADRESSE_CLAVIER 0x20  // 32 en décimal (sur le PCF8574P, A0 A1 A2 sont mis tous à 0)
const byte ROWS = 4; // Nombre de rangées du clavier
const byte COLS = 3; // Nombre de colonnes du clavier
const char KEYS[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
const char NO_KEY = '\0'; // null ou code 0 en ASCII
// Supérieurs à 99 pour permettre de rentrer au clavier des nombres à 2 chiffres,
const byte TOUCHE_DIESE = 100;  // car le code ASCII de # est 35 
const byte TOUCHE_ETOILE = 101; // et celui de * est 42.


// *** Horloge RTC
// On importe les bibliothèques
#include <Wire.h>    // On importe la bibliothèque Wire pour l'I2C
#include <RTClib.h>  // https://github.com/adafruit/RTClib
RTC_DS1307 RTC;
DateTime DateHeure;
String Date;
String Heure;


// *** Carte SD
// On importe la bibliothèque après avoir modifié le fichier
// /usr/share/arduino/libraries/SD/utility/Sd2PinMap.h
// qui n'est pas adapté à l'ATMega 1284P et qui en plus contient une erreur :
// Voir http://serandour.com/atmega1284p-et-arduino.htm
#include <SD.h>
// On the Ethernet Shield, CHIP_SELECT (SS) is pin 4. Note that even if it's not
// used as the SS pin, the hardware SS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const byte CHIP_SELECT = 4;   // Chip Select de la carte SD.
File dataFile;
String enteteFichier;
const String SEPARATEUR =";"; // Séparateur de données pour le tableur
                              // Ne pas choisir l'espace à cause de Date Heure

// *** Ecrans
const byte TAG_ECRAN_ACCUEIL = 0;
const byte TAG_MENU_PRINCIPAL = 1;
const byte TAG_REGLAGE_HORLOGE = 2;
const byte TAG_MENU_ENREGISTREUR = 3;
const byte TAG_MENU_CAPTEURS = 4;
const byte TAG_MENU_CADENCE = 5;
const byte TAG_MENU_BARO_ALTIMETRE = 6;
const byte TAG_MENU_THERMOMETRE = 7;
const byte TAG_MENU_LUXMETRE = 8;
const byte TAG_ENREGISTRER_FICHIER = 9;
const byte TAG_TRANSFERER_FICHIER = 10;
const byte TAG_MENU_ENTREES_ANALOGIQUES = 11;
const byte TAG_MENU_ENTREES_NUMERIQUES = 12;
byte ecran = TAG_ECRAN_ACCUEIL;
int defilement = 0;


// *** Gestion du temps
const byte MODE_MANUEL = 0;
const unsigned long CADENCE[9] = {
MODE_MANUEL, // MODE MANUEL
100,         // 100 MS
1000,        // 1 S
5000,        // 5 S
15000,       // 15 S
60000,       // 1 MIN
300000,      // 5 MIN
900000,      // 15 MIN
3600000      // 1 H
};
unsigned long cadenceDefaut = CADENCE[2]; // Valeur par défaut.
unsigned long deltaMesures = cadenceDefaut; // Intervalle entre 2 mesures (en ms).
unsigned long time = 0;
unsigned long timeOffset = 0;


// *** Autres
unsigned long numeroMesure = 0;
boolean recording = false;


//////////////////////////////////////////////////////////////////////////////////////

// *** Les capteurs (début)

//////////////////////////////////////////////////////////////////////////////////////

#define AUCUN_CAPTEUR -1

// *** Entrées analogiques
const byte EA0 = A0; // 24
const byte EA1 = A1; // 25
const byte EA2 = A2; // 26
const byte EA3 = A3; // 27
const byte EA4 = A4; // 28
const byte EA5 = A5; // 29
const byte EA6 = A6; // 30
const byte EA7 = A7; // 31

// *** Entrées numériques
const byte EN0 = 21;
const byte EN1 = 20;
const byte EN2 = 19;
const byte EN3 = 18;
const byte DECLENCHEUR = EN3; // Pour déclencher un enregistrement en mode manuel


// *** Le baromètre BMP180
#define ADRESSE_BMP180 0x77 // 119 en décimal

// Capteurs sur entrées analogiques (Luxmètre & Thermomètre)
const byte TRANSLATION = 8; // Pour éviter que 2 entrées aient la meme adresse
// *** Le luxmètre
#define ADRESSE_LUXMETRE A7 + TRANSLATION
// *** Le thermomètre LM35
#define ADRESSE_THERMOMETRE A6 + TRANSLATION

// *** Entrées disponibles
// Modifier en conséquence la procédure lectureCapteurs() quelques lignes en dessous
const byte NB_ENTREES_MAX = 15;
const byte ENTREE[NB_ENTREES_MAX] = {
  EA0,                  //  Entrée analogique
  EA1,                  //  Entrée analogique
  EA2,                  //  Entrée analogique
  EA3,                  //  Entrée analogique
  EA4,                  //  Entrée analogique
  EA5,                  //  Entrée analogique
  EA6,                  //  Entrée analogique
  EA7,                  //  Entrée analogique  
  EN0,                  //  Entrée numérique
  EN1,                  //  Entrée numérique
  EN2,                  //  Entrée numérique
  EN3,                  //  Entrée numérique
  ADRESSE_BMP180,       //  Adresse du baromètre BMP180 sur le port I2C
  ADRESSE_THERMOMETRE,  //  Adresse du thermomètre
  ADRESSE_LUXMETRE      //  Adresse du luxmètre
};
const String NOM_ENTREE[NB_ENTREES_MAX] = {
  "BROCHE 40 (A)",      // EA0
  "BROCHE 39 (A)",      // EA1
  "BROCHE 38 (A)",      // EA2
  "BROCHE 37 (A)",      // EA3
  "BROCHE 36 (A)",      // EA4
  "BROCHE 35 (A)",      // EA5
  "BROCHE 34 (A)",      // EA6
  "BROCHE 33 (A)",      // EA7
  "BROCHE 27 (N)",      // EN0
  "BROCHE 26 (N)",      // EN1
  "BROCHE 25 (N)",      // EN2
  "BROCHE 24 (N)",      // EN3
  "BARO-ALTIMETRE",     // ADRESSE_BMP180
  "THERMOMETRE",        // ADRESSE_THERMOMETRE
  "LUXMETRE"            // ADRESSE_LUXMETRE
};
const byte POIDS_ENTREE[NB_ENTREES_MAX] = {
  1, // EA0 mesure 1 seule grandeur
  1, // EA1 mesure 1 seule grandeur
  1, // EA2 mesure 1 seule grandeur
  1, // EA3 mesure 1 seule grandeur
  1, // EA4 mesure 1 seule grandeur
  1, // EA5 mesure 1 seule grandeur
  1, // EA6 mesure 1 seule grandeur
  1, // EA7 mesure 1 seule grandeur
  1, // EN0 mesure 1 seule grandeur
  1, // EN1 mesure 1 seule grandeur
  1, // EN2 mesure 1 seule grandeur
  1, // EN3 mesure 1 seule grandeur
  4, // Le baromètre renvoie 4 grandeurs : La pression absolue, la pression relative, l'altitude, la température
  1, // Le thermomètre renvoie 1 seule grandeur : La température
  1  // Le luxmètre renvoie 1 seule grandeur : L'éclairement lumineux
};
const byte NB_MESURES_MAX = 18; // La somme du poids des entrées (1+1+1+1+1+1+1+1+1+1+1+1+4+1+1)
const String NOM_MESURE[NB_MESURES_MAX] = {
  "BROCHE 40 (A)",
  "BROCHE 39 (A)",
  "BROCHE 38 (A)",
  "BROCHE 37 (A)",
  "BROCHE 36 (A)",
  "BROCHE 35 (A)",
  "BROCHE 34 (A)",
  "BROCHE 33 (A)",  
  "BROCHE 27 (N)",
  "BROCHE 26 (N)",
  "BROCHE 25 (N)",
  "BROCHE 24 (N)",
  "PRESSION ABSOLUE", "PRESSION RELATIVE", "ALTITUDE", "TEMPERATURE",
  "TEMPERATURE",
  "ECLAIREMENT"
};

// *** Variables
boolean selectionCapteur[NB_ENTREES_MAX];
int nbCapteurs;
int adresseCapteur[NB_ENTREES_MAX];
int nbMesures;
long mesureBrute[NB_MESURES_MAX]; // initialement int mais modifié à cause du luxmètre

// *** Baro-altimètre BMP180
float pressionAbsolue = 0;
float pressionRelative = 0;
float altitude = 0;
float temperatureBMP180 = 0; // Température
float altitudeReference = 0; // Altitude connue rentrée au clavier
float pressionReference = 0; // Pression à l'altitude de référence

// Thermomètre

// Luxmètre


//////////////////////////////////////////////////////////////////////////////////////

void initCapteurs()
{
  // Initialisation des entrées
  nbCapteurs = 0;
  for (int i=0; i<NB_ENTREES_MAX; i++) {
    adresseCapteur[i] = AUCUN_CAPTEUR;
  }
  // Initialisation de l'entete du fichier
  enteteFichier = "MESURE" + SEPARATEUR + "DATE HEURE" + SEPARATEUR + "MILLISECONDES";    
}

//////////////////////////////////////////////////////////////////////////////////////

void lectureCapteurs()
{
  int j=0;
  for (int i=0; i<nbCapteurs ; i++) {
    if (adresseCapteur[i] == ENTREE[0]) {       // EA0
      mesureBrute[j] = analogRead(ENTREE[0]);
      j += POIDS_ENTREE[0];
    }
    else if (adresseCapteur[i] == ENTREE[1]) {  // EA1
      mesureBrute[j] = analogRead(ENTREE[1]);
      j += POIDS_ENTREE[1];
    }
    else if (adresseCapteur[i] == ENTREE[2]) {  // EA2
      mesureBrute[j] = analogRead(ENTREE[2]);
      j += POIDS_ENTREE[2];
    }
    else if (adresseCapteur[i] == ENTREE[3]) {  // EA3
      mesureBrute[j] = analogRead(ENTREE[3]);
      j += POIDS_ENTREE[3];
    }
    else if (adresseCapteur[i] == ENTREE[4]) {  // EA4
      mesureBrute[j] = analogRead(ENTREE[4]);
      j += POIDS_ENTREE[4];
    }
    else if (adresseCapteur[i] == ENTREE[5]) {  // EA5
      mesureBrute[j] = analogRead(ENTREE[5]);
      j += POIDS_ENTREE[5];
    }
    else if (adresseCapteur[i] == ENTREE[6]) {  // EA6
      mesureBrute[j] = analogRead(ENTREE[6]);
      j += POIDS_ENTREE[6];
    }
    else if (adresseCapteur[i] == ENTREE[7]) {  // EA7
      mesureBrute[j] = analogRead(ENTREE[7]);
      j += POIDS_ENTREE[7];
    }    
    else if (adresseCapteur[i] == ENTREE[8]) {  // EN0
      mesureBrute[j] = digitalRead(ENTREE[8]);
      j += POIDS_ENTREE[8];
    }
    else if (adresseCapteur[i] == ENTREE[9]) {  // EN1
      mesureBrute[j] = digitalRead(ENTREE[9]);
      j += POIDS_ENTREE[9];
    }
    else if (adresseCapteur[i] == ENTREE[10]) {  // EN2
      mesureBrute[j] = digitalRead(ENTREE[10]);
      j += POIDS_ENTREE[10];
    }
    else if (adresseCapteur[i] == ENTREE[11]) {  // EN3
      mesureBrute[j] = digitalRead(ENTREE[11]);
      j += POIDS_ENTREE[11];
    }       
    else if (adresseCapteur[i] == ENTREE[12]) {  // Baromètre BMP180
      barometreRead(ENTREE[12]);
      mesureBrute[j] = pressionAbsolue * 10;    // x 10 : Pour récupérer la première décimale
      mesureBrute[j+1] = pressionRelative * 10; // x 10 : Pour récupérer la première décimale
      mesureBrute[j+2] = altitude * 10;         // x 10 : Pour récupérer la première décimale
      mesureBrute[j+3] = temperatureBMP180 * 10;// x 10 : Pour récupérer la première décimale
      j += POIDS_ENTREE[12];
    }
    else if (adresseCapteur[i] == ENTREE[13]) {  // Thermomètre
      mesureBrute[j] = thermometreRead(ENTREE[13]-TRANSLATION) * 10; // x 10 : Pour récupérer la première décimale
                                                                     // -TRANSLATION car le thermomètre est
                                                                     // branché sur une entrée analogique.
      j += POIDS_ENTREE[13];
    }           
    else if (adresseCapteur[i] == ENTREE[14]) {  // Luxmètre
      mesureBrute[j] = luxmetreRead(ENTREE[14]-TRANSLATION); // -TRANSLATION car le luxmètre est
                                                             // branché sur une entrée analogique.
      j += POIDS_ENTREE[14];
    }
  }
  nbMesures=j;
}

//////////////////////////////////////////////////////////////////////////////////////

// *** Les capteurs (fin)

//////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  // Capteurs par défaut
  initCapteurs();
  
  Serial.begin(9600);  // Setup serial. Transfert des données à 9600 bauds.
  
  // Afficheur LCD
  lcd.begin(20,4);
  lcd.clear();
  
  // Buzzer
  pinMode(BUZZER, OUTPUT);
  
  // Initialisation de l'interface I2C.
  Wire.begin();   
  // Horloge RTC DS 1307 (I2C)
  initHorloge();
 
  // Carte SD
  // Make sure that the default chip select pin is set to output, even if you don't use it.
  pinMode(CHIP_SELECT, OUTPUT); // Chip Select en sortie.
  // See if the card is present and can be initialized. 
  if (!SD.begin(CHIP_SELECT)) {
    bipErreur();
    lcd.setCursor(0,1);
    lcd.print("PROBLEME DE CARTE SD");    
    delay(3000);
    lcd.clear();
  } else {
    bipOK();
  }
  
  // Bornes numériques
  pinMode(EN0, INPUT);
}

//////////////////////////////////////////////////////////////////////////////////////

void loop()
{ 
  switch(ecran) {
    case TAG_ECRAN_ACCUEIL: afficherEcranAccueil(); break;
    case TAG_MENU_PRINCIPAL: afficherMenuPrincipal(); break;
    case TAG_REGLAGE_HORLOGE: afficherReglageHorloge(); break;
    case TAG_MENU_ENREGISTREUR: afficherMenuEnregistreur(); break;
    case TAG_MENU_CADENCE: afficherMenuCadence(); break;
    case TAG_MENU_CAPTEURS: afficherMenuCapteurs(); break;
    case TAG_MENU_BARO_ALTIMETRE: afficherBaroAltimetre(ADRESSE_BMP180); break;
    case TAG_MENU_LUXMETRE : afficherLuxmetre(ADRESSE_LUXMETRE); break;
    case TAG_MENU_THERMOMETRE : afficherThermometre(ADRESSE_THERMOMETRE); break;
    case TAG_ENREGISTRER_FICHIER: enregistrerFichier(); break;
    case TAG_TRANSFERER_FICHIER: transfererFichier(); break;
    case TAG_MENU_ENTREES_ANALOGIQUES: afficherEntreesAnalogiques(); break;
    case TAG_MENU_ENTREES_NUMERIQUES: afficherEntreesNumeriques(); break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherEcranAccueil()
{
  // Affichage
  afficherHorloge();
  lcd.setCursor(0,0);
  lcd.print("* MENU");
  lcd.setCursor(11,0);
  lcd.print("# HORLOGE");
  
  // Gestion du clavier
  switch(getKey()) {      
    case '*': selectMenu(TAG_MENU_PRINCIPAL); break;
    case '#': selectMenu(TAG_REGLAGE_HORLOGE); break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherMenuPrincipal()
{
  // Affichage
  const byte NB_LIGNES_MENU = 7;
  String menu[] = {
    "0: ACCUEIL",
    "1: ENREGISTREUR",
    "2: 40/../33 (A)",
    "3: 27/../24 (N)",
    "4: BARO-ALTIMETRE",
    "5: THERMOMETRE", 
    "6: LUXMETRE"
  };  
  afficheMenu(menu, NB_LIGNES_MENU);

  // Gestion du clavier
  switch(choixMenu(1)) { // On entre un nombre à 1 chiffre
    case 0: selectMenu(TAG_ECRAN_ACCUEIL); break;
    case 1: selectMenu(TAG_MENU_ENREGISTREUR); break;
    case 2: selectMenu(TAG_MENU_ENTREES_ANALOGIQUES); break;
    case 3: selectMenu(TAG_MENU_ENTREES_NUMERIQUES); break;
    case 4: selectMenu(TAG_MENU_BARO_ALTIMETRE); break;
    case 5: selectMenu(TAG_MENU_THERMOMETRE); break;
    case 6: selectMenu(TAG_MENU_LUXMETRE); break;
    case TOUCHE_DIESE: defileMenu(NB_LIGNES_MENU); break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherMenuEnregistreur()
{
  // Affichage
  const byte NB_LIGNES_MENU = 4;
  String menu[] = {
    "0: ACCUEIL",
    "1: PARAMETRES",
    "2: ENREGISTRER",
    "3: TRANSFERT -> USB"
  };  
  afficheMenu(menu, NB_LIGNES_MENU);
  
  // Gestion du clavier
  switch(choixMenu(1)) { // On entre un nombre à 1 chiffre
    case 0: selectMenu(TAG_ECRAN_ACCUEIL); break;
    case 1: selectMenu(TAG_MENU_CADENCE); break;
    case 2: selectMenu(TAG_ENREGISTRER_FICHIER); break;
    case 3: selectMenu(TAG_TRANSFERER_FICHIER); break;
    case TOUCHE_DIESE: defileMenu(NB_LIGNES_MENU); break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherMenuCadence()
{
  // Affichage
  const byte NB_LIGNES_MENU = 9;
  String menu[] = {
    "1:     MANUEL",
    "2: 100 MILLISECONDES",
    "3:   1 SECONDE",
    "4:   5 SECONDES",
    "5:  15 SECONDES",
    "6:   1 MINUTE",
    "7:   5 MINUTES",
    "8:  15 MINUTES",
    "9:   1 HEURE" 
  };  
  afficheMenu(menu, NB_LIGNES_MENU);
  
  // Gestion du clavier
  byte choix = choixMenu(1); // On entre un nombre à 1 chiffre
  switch(choix) {
    case TOUCHE_ETOILE: break;
    case 0: selectMenu(TAG_ECRAN_ACCUEIL); break;
    case TOUCHE_DIESE: defileMenu(NB_LIGNES_MENU); break;
    default :
      if (choix <= NB_LIGNES_MENU) {
        deltaMesures = CADENCE[choix-1];
        int correction = 0;
        if (defilement > NB_LIGNES_MENU - NB_LIGNES_LCD) {
          if (choix <= defilement) {
          correction = NB_LIGNES_MENU;
          }
        }
        if ((choix+correction >= defilement+1) 
        && (choix+correction <= defilement+NB_LIGNES_LCD)) {
          lcd.clear();
          lcd.setCursor(0,(choix+NB_LIGNES_MENU-1-defilement)%NB_LIGNES_MENU);
          lcd.print(menu[choix-1]);
          delay(1000);
          selectMenu(TAG_MENU_CAPTEURS);
          // Initialisation des capteurs sélectionnés
          nbCapteurs = 0;
          for (int i=0; i<NB_ENTREES_MAX; i++) {
            adresseCapteur[i] = AUCUN_CAPTEUR;
            selectionCapteur[i] = false;
          }
          enteteFichier="MESURE";
        }  
      }
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherMenuCapteurs()
{ 
  // Affichage
  const byte NB_LIGNES_MENU = NB_ENTREES_MAX;
  String menu[NB_LIGNES_MENU];
  for (int i=0; i<NB_LIGNES_MENU; i++) {
    int nbEspaces = NB_COLONNES_LCD - NOM_ENTREE[i].length() - 4;
    if (i<9) {
      menu[i] = "0";
    }
    menu[i] += String(i+1,DEC) + ": " + NOM_ENTREE[i];
    for (int j=1; j<=nbEspaces; j++) {
      menu[i] += " ";
    }
    if (selectionCapteur[i]) {
      menu[i]=menu[i].substring(0,NB_COLONNES_LCD-1)+'*';
    }
  }
  afficheMenu(menu, NB_LIGNES_MENU);
  
  // Gestion du clavier
  byte choix = choixMenu(2); // On entre un nombre à 2 chiffres
  switch(choix) {
    case TOUCHE_ETOILE: // Validation
      enteteFichier += SEPARATEUR + "DATE HEURE" + SEPARATEUR + "MILLISECONDES";               
      // REGLAGE DE L'ALTIMETRE
      for (int i=0; i<nbCapteurs; i++) {
        if (adresseCapteur[i] == ADRESSE_BMP180) {
          initBaroAltimetre(adresseCapteur[i]);
          reglageBaroAltimetre(adresseCapteur[i]);
        }
      }
      selectMenu(TAG_MENU_ENREGISTREUR);
      break;   
    case 0: selectMenu(TAG_ECRAN_ACCUEIL); break;
    case TOUCHE_DIESE: defileMenu(NB_LIGNES_MENU); break;
    default :
      if (choix <= NB_LIGNES_MENU) {
        int correction = 0;
        if (defilement > NB_LIGNES_MENU - NB_LIGNES_LCD) {
          if (choix <= defilement) {
            correction = NB_LIGNES_MENU;
          }  
        }
        if ((choix+correction >= defilement+1) 
        && (choix+correction <= defilement+NB_LIGNES_LCD)) {
          if (selectionCapteur[choix-1] == true) {
            bipErreur();
          }
          else {
            selectionCapteur[choix-1] = true;
            adresseCapteur[nbCapteurs] = ENTREE[choix-1];       
            int index = 0;
            for (int i=0; i<choix-1; i++) {
              index += POIDS_ENTREE[i];
            }
            for (int i=0; i<POIDS_ENTREE[choix-1]; i++) {
              enteteFichier += SEPARATEUR + NOM_MESURE[index+i];
            }       
            nbCapteurs++;
            while (getKey() != NO_KEY); // On boucle tant que la touche pressée n'est pas relachée
          }
        }        
      }
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void afficheMenu(String menu[], byte nbLignes)
{
  for (int i=0; i<NB_LIGNES_LCD ; i++) {
    lcd.setCursor(0,i);
    int index = i + defilement;
    if (index >= nbLignes) {
      index -= nbLignes;
    }
    lcd.print(menu[index]);
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void defileMenu(byte nbLignes)
{
  if (nbLignes > NB_LIGNES_LCD) {
    defilement++;
    defilement %= nbLignes;
    while (getKey() != NO_KEY); // On boucle tant que la touche pressée n'est pas relachée
    lcd.clear(); 
  }  
}

//////////////////////////////////////////////////////////////////////////////////////

void defileMesures(byte nbLignes, byte nbLignesLCD)
{
  if (nbLignes > nbLignesLCD) {
    defilement++;
    defilement %= nbLignes;
    while (getKey() != NO_KEY); // On boucle tant que la touche pressée n'est pas relachée
  }  
}

//////////////////////////////////////////////////////////////////////////////////////

void selectMenu(byte tag)
{
  ecran = tag;
  defilement = 0;
  while (getKey() != NO_KEY); // On boucle tant que la touche pressée n'est pas relachée 
  lcd.clear();
}

//////////////////////////////////////////////////////////////////////////////////////

// BUZZER

//////////////////////////////////////////////////////////////////////////////////////

void bipErreur()
{
  // 650 Hz;
  for (int i = 1; i <= 3 ; i++) {
    playFrequence(650,100);
    delay(50);
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void bipOK()
{
  // 2600 Hz
  for (int i = 1; i <= 2 ; i++) {
    playFrequence(2600,100);
    delay(50);
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void playFrequence(int frequence, int duration)
{
  long demiPeriode = 1000000L / (long)frequence >> 2; // Demi-periode en microsecondes
  for (long i = 0; i < duration * 1000L; i += demiPeriode * 2) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(demiPeriode);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(demiPeriode);
  }
}

//////////////////////////////////////////////////////////////////////////////////////

// CLAVIER I2C

//////////////////////////////////////////////////////////////////////////////////////

char getKey() {
  char key;
  // Les 8 bits des valeurs binaires qui suivent correspondent à
  // P7 P6 P5 P4 P3 P2 P1 P0 au niveau du PCF8574
  //    C3 C2 C1 L4 L3 L2 L1 au niveau du clavier (pas de C4 : clavier à 3 colonnes)
  writePCF8574(B11110000);
  byte valeur1 = readPCF8574();     // On récupère la colonne : Si une touche est pressée,
                                    // un des 4 bits de poids fort passe de 1 à 0.
  writePCF8574(B00001111);
  byte valeur2 = readPCF8574();     // On récupère la ligne : Si une touche est pressée,
                                    // un des 4 bits de poids faible passe de 1 à 0.
  byte codeKey = valeur1 | valeur2; // OU bit à bit pour fusionner les résultats ci-dessus

  switch (codeKey) {
    case B11101110 : key = KEYS[0][0]; break; // 238
    case B11011110 : key = KEYS[0][1]; break; // 222
    case B10111110 : key = KEYS[0][2]; break; // 190
    case B11101101 : key = KEYS[1][0]; break; // 237
    case B11011101 : key = KEYS[1][1]; break; // 221
    case B10111101 : key = KEYS[1][2]; break; // 189
    case B11101011 : key = KEYS[2][0]; break; // 235
    case B11011011 : key = KEYS[2][1]; break; // 219
    case B10111011 : key = KEYS[2][2]; break; // 187
    case B11100111 : key = KEYS[3][0]; break; // 231
    case B11010111 : key = KEYS[3][1]; break; // 215
    case B10110111 : key = KEYS[3][2]; break; // 183
    default        : key = NO_KEY;            // null en ASCII
  }
  return key;
}

//////////////////////////////////////////////////////////////////////////////////////

void writePCF8574(byte data) {
  Wire.beginTransmission(ADRESSE_CLAVIER);
  Wire.write(data);
  Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////

byte readPCF8574() {
  Wire.requestFrom(ADRESSE_CLAVIER, 1);     // Request 1 byte from device
  while (Wire.available() < 1);             // Wait until byte are ready
  byte value = Wire.read();  
  return value;
}

//////////////////////////////////////////////////////////////////////////////////////

int valeurTouche(char key)
{
  int valeur = byte(key)-byte('1')+1;
  return valeur;
}

//////////////////////////////////////////////////////////////////////////////////////

byte choixMenu(byte nbChiffres)
{
  byte choix = 0;
  boolean quitter = false;
  do {
    char touche = getKey();
    if (touche != NO_KEY) {
      switch (touche) {
        case '#' :
          choix = TOUCHE_DIESE;
          quitter = true;
          break;
        case '*' :
          choix = TOUCHE_ETOILE;
          quitter = true;  
          break;
        default :
          choix = 10*choix + valeurTouche(touche);
          nbChiffres--;
          if (nbChiffres < 1) {
            quitter = true;
          }
          while (getKey() != NO_KEY); // On attend que la touche pressée soit relachée
        }
    }  
  } while (quitter == false);
  return choix;
}

//////////////////////////////////////////////////////////////////////////////////////

// HORLOGE

//////////////////////////////////////////////////////////////////////////////////////

void initHorloge()
{
  RTC.begin();  // Initialisation de l'horloge.
  // Si la RTC n'est pas configurée, le faire avec les valeurs de l'ordinateur
  // au moment de la compilation.
  if (! RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__)); 
  }
  // Pas nécessairement utile mais on prend toutes les précautions
  if (!testHorloge()) {
    RTC.adjust(DateTime(2000,1,1,0,0,0));
  }
}

//////////////////////////////////////////////////////////////////////////////////////

boolean testHorloge()
{
  DateHeure = RTC.now();
  int annees = DateHeure.year();
  int mois = DateHeure.month();
  int jours = DateHeure.day();
  int heures = DateHeure.hour();
  int minutes = DateHeure.minute();
  int secondes = DateHeure.second();
  boolean etat = true;
  if (annees < 2000 || annees > 2099 ||
      mois < 1 || mois > 12 ||
      jours < 1 || jours > 31 ||
      heures < 0 || heures > 23 ||
      minutes < 0 || minutes > 59 ||
      secondes < 0 || secondes > 59) {
    etat = false;
    }
  return etat;  
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherHorloge()
{
  DateHeure = RTC.now();
  formaterDateHeure();
  lcd.setCursor(5,2);
  lcd.print(Date);
  lcd.setCursor(6,3);
  lcd.print(Heure); 
}

//////////////////////////////////////////////////////////////////////////////////////

void formaterDateHeure()
{
  String ddmmyy = "";  
  if (DateHeure.day() < 10) {
    ddmmyy = "0";
  }
  ddmmyy += String(DateHeure.day(),DEC) + '/'; 
  if (DateHeure.month() < 10) {
    ddmmyy += "0";
  }
  Date = ddmmyy + String(DateHeure.month(),DEC) + '/' + String(DateHeure.year(),DEC);
  
  String hhmmss = ""; 
  if (DateHeure.hour() < 10) {
    hhmmss = "0";
  }  
  hhmmss += String(DateHeure.hour(),DEC) + ':';  
  if (DateHeure.minute() < 10) {
    hhmmss += "0";
  }   
  hhmmss += String(DateHeure.minute(),DEC) + ':';  
  if (DateHeure.second() < 10) {
    hhmmss += "0";
  }   
  Heure = hhmmss + String(DateHeure.second(),DEC);  
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherReglageHorloge()
{
  colonneLCD = 4;
  String chaine = "";
  // Affichage
  lcd.setCursor(4,2);
  lcd.print("JJMMAAAAhhmm"); 
  // Gestion du clavier
  do {
    char touche = getKey();
    if ((touche != NO_KEY) && (touche!='*') && (touche!='#')) { // Si une touche est pressée
      lcd.setCursor(colonneLCD, 1);
      lcd.print(touche);
      chaine += touche;
      colonneLCD++;
      while (getKey() != NO_KEY); // On boucle tant que la touche pressée n'est pas relachée
    }
  } while (colonneLCD < 16);  
  decodeChaineHorloge(chaine);
  selectMenu(TAG_ECRAN_ACCUEIL);
}
//////////////////////////////////////////////////////////////////////////////////////

void decodeChaineHorloge(String chaine)
{
  int jours = chaine.substring(0,2).toInt();
  int mois = chaine.substring(2,4).toInt();
  int annees = chaine.substring(4,8).toInt();
  int heures = chaine.substring(8,10).toInt();
  int minutes = chaine.substring(10,12).toInt();
  if (annees >= 2000 && annees <= 2099 &&
      mois >= 1 && mois <= 12 &&
      jours >= 1 && jours <= 31 &&
      heures >= 0 && heures <= 23 &&
      minutes >= 0 && minutes <= 59) {
    RTC.adjust(DateTime(annees, mois, jours, heures, minutes, 0));
  }
 else {
   bipErreur();
 } 
}

//////////////////////////////////////////////////////////////////////////////////////

void enregistrerFichier()
{
  do {
    if (!recording) {
      nouveauFichier();
      recording = true;
      numeroMesure = 0;
    }
    else {
      // MODE MANUEL
      if (deltaMesures == MODE_MANUEL) {
        DateHeure = RTC.now();
        lectureCapteurs();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ATTENTE");      
        lcd.setCursor(0,1);
        lcd.print("NB MESURES: ");     
        lcd.print(numeroMesure+1);
        lcd.setCursor(0,2);
        lcd.print("#: ENREGISTRER");
        lcd.setCursor(0,3);
        lcd.print("*: STOP");        
        formaterDateHeure();
        
        // Vers la carte SD
        // Open the file. Note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open("data.txt", FILE_WRITE);
        // If the file is available, write to it.
        if (dataFile) {
          dataFile.print(numeroMesure);
          dataFile.print(SEPARATEUR);
          for (int i=0; i<(nbMesures); i++) {
            dataFile.print(mesureBrute[i]);
            dataFile.print(SEPARATEUR); 
          }
          dataFile.print(Date);
          dataFile.print(" ");
          dataFile.print(Heure);
          dataFile.println("");
          dataFile.close();
        }
        // If the file isn't open, pop up an error.
        else {
          Serial.println("Error opening datalog.txt");  
          bipErreur();
        }
        
        // Vers le port série
        Serial.print(numeroMesure);
        Serial.print(SEPARATEUR);
        for (int i=0; i<(nbMesures); i++) {
          Serial.print(mesureBrute[i]);
          Serial.print(SEPARATEUR);
        }
        Serial.print(Date);
        Serial.print(" ");
        Serial.print(Heure);
        Serial.println("");        
        
        boolean attente = true;
        do {
          // Lecture du clavier
          char key = getKey();
          switch (key) {
            case '*' :
              // Stoppe l'enregistrement
              recording = false;
              selectMenu(TAG_MENU_ENREGISTREUR);
              break;
            case '#' :
              attente = false;
              while (getKey() != NO_KEY); // On boucle tant que la touche pressée n'est pas relachée
              break;
          }
          // Lecture du déclencheur à distance
          if (digitalRead(DECLENCHEUR) == HIGH) {
            delay(50); // Delai supérieur à celui des rebondissements d'une éventuelle touche
            attente = false;
            while (digitalRead(DECLENCHEUR) == HIGH); // On boucle en attendant que le déclencheur passe à LOW          
          }
        } while (attente && recording);
        numeroMesure++;
      }
      // MODE AUTOMATIQUE
      else {
        time = millis();
        DateHeure = RTC.now();
        if (numeroMesure == 0) {
          timeOffset = time;
        }
        unsigned long duree = time - timeOffset;
        lectureCapteurs();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ECRITURE");      
        lcd.setCursor(0,1);
        lcd.print("NB MESURES: ");     
        lcd.print(numeroMesure+1);
        lcd.setCursor(0,2);
        lcd.print("DUREE: ");
        String dureeFormatee = "";
        int nbHeures = long(numeroMesure*deltaMesures/3600000);
        int nbMinutes = long(numeroMesure*deltaMesures/60000-nbHeures*60);
        int nbSecondes = long(numeroMesure*deltaMesures/1000-nbHeures*3600-nbMinutes*60);
        if (nbHeures < 10) {
          dureeFormatee = "0";
        }
        dureeFormatee += String(nbHeures, DEC);
        if (deltaMesures < 3600000) {
          dureeFormatee += ":";  
          if (nbMinutes < 10) {
            dureeFormatee += "0";
          }
          dureeFormatee += String(nbMinutes, DEC);
        }
        if (deltaMesures < 60000) {
          dureeFormatee += ":";      
          if (nbSecondes < 10) {
            dureeFormatee += "0";
          }      
          dureeFormatee += String(nbSecondes, DEC);
        }
        lcd.print(dureeFormatee);
        lcd.setCursor(0,3);
        lcd.print("*: STOP");        
        formaterDateHeure();
        
        // Vers la carte SD
        // Open the file. Note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open("data.txt", FILE_WRITE);
        // If the file is available, write to it.
        if (dataFile) {
          dataFile.print(numeroMesure);
          dataFile.print(SEPARATEUR);
          for (int i=0; i<(nbMesures); i++) {
            dataFile.print(mesureBrute[i]);
            dataFile.print(SEPARATEUR);
          }      
          dataFile.print(Date);
          dataFile.print(" ");
          dataFile.print(Heure);
          dataFile.print(SEPARATEUR);
          dataFile.print(duree);
          dataFile.println("");
          dataFile.close(); 
        }
        // If the file isn't open, pop up an error.
        else {
          Serial.println("Error opening datalog.txt");  
          bipErreur();
        }
        
        // Vers le port série
        Serial.print(numeroMesure);
        Serial.print(SEPARATEUR);
        for (int i=0; i<(nbMesures); i++) {
          Serial.print(mesureBrute[i]);
          Serial.print(SEPARATEUR);
        }
        Serial.print(Date);
        Serial.print(" ");
        Serial.print(Heure);
        Serial.print(SEPARATEUR);
        Serial.print(duree);
        Serial.println("");
        
        // Mécanisme de régulation.
        // On regarde où on en est au niveau temps parce qu'une boucle dure ici environ 33 ms
        time = millis();
        duree = time - timeOffset;
        long correction = duree-(numeroMesure*deltaMesures);
        long tempsPause = deltaMesures-correction; // Pour éviter de rares bugs lors de
        if (tempsPause < 0) {                      // la régulation : il peut arriver que
          tempsPause = 0;                          // correction dépasse 100 ms.
        }
        unsigned long tempsEcoule;
        do {
          char key = getKey(); // Lecture de la touche actionnée 
          if (key == '*') {
            // Stoppe l'enregistrement
            recording = false;
            selectMenu(TAG_MENU_ENREGISTREUR);
            break;  // On sort de la boucle do...while  
          }
          tempsEcoule = millis() - time;     
        } while (tempsEcoule < tempsPause);
        numeroMesure++;
      }
    }  
  } while (recording == true);
}

//////////////////////////////////////////////////////////////////////////////////////

void nouveauFichier()
{
  String entete = enteteFichier;
  if (deltaMesures == MODE_MANUEL) {
    // On enlève MILLISECONDES à la fin de l'entete du fichier d'où le -14
    entete = enteteFichier.substring(0,enteteFichier.length()-14);
  }
  
  // Sortie sur carte SD
  // Effacer le fichier data.txt précédent
  if (SD.exists("data.txt")) {
    SD.remove("data.txt");
  }
  // Créer le fichier data.txt et sa première ligne
  dataFile = SD.open("data.txt", FILE_WRITE);  
  // If the file is available, write to it.
  if (dataFile) {
    dataFile.println(entete);
    dataFile.close(); 
  }
  // If the file isn't open, pop up an error.
  else {
    Serial.println("Error opening data.txt");
    bipErreur();
  }

  // Sortie sur port série
  Serial.println(entete);
}

//////////////////////////////////////////////////////////////////////////////////////

void transfererFichier()
{
  lcd.setCursor(0,0);
  lcd.print("TRANSFERT DU FICHIER");
  // Lecture du fichier data.txt sur le port série  
  dataFile = SD.open("data.txt");
  if (dataFile) {
    unsigned long tailleFichier = dataFile.size();
    // Read from the file until there's nothing else in it
    while (dataFile.available()) {
      Serial.write(dataFile.read());
      // Affichage du pourcentage de transfert
      lcd.setCursor(15,3);
      byte pourcentage = 100 * dataFile.position() / tailleFichier;
      dtostrf(pourcentage, 3, 0, ligne);
      lcd.print(ligne);
      lcd.print(" %");    
      // Pour abandonner le transfert
      char key = getKey(); // Lecture de la touche actionnée 
      if (key == '0') {
        break;  // On sort de la boucle while  
      }
    }
    // Close the file
    dataFile.close();
    delay(3000); // Le temps que le programme de récupération se ferme
    bipOK();
    selectMenu(TAG_MENU_ENREGISTREUR);
  }
  // If the file didn't open, print an error
  else { 
    Serial.println("Error opening data.txt");
    bipErreur();
  }  
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherEntreesAnalogiques()
{
  const byte NB_ENTREES = 8;
  String menu[NB_ENTREES];
  boolean quitter = false;
  do {        
    for (int i=0; i<NB_ENTREES; i++) {
      dtostrf(analogRead(ENTREE[i]), 4, 0, ligne);
      menu[i] = "BROCHE " + String(40-i) + ": " + ligne;
    }    
    lcd.setCursor(0,0);
    lcd.print("ENTREES ANALOGIQUES");
    for (int i=0; i<NB_LIGNES_LCD-1; i++) {
      lcd.setCursor(0,i+1);
      int index = i + defilement;
      if (index >= NB_ENTREES) {
        index -= NB_ENTREES;
      }
    lcd.print(menu[index]);
    }  
    for (int i=0; i<100; i++) {
      delay(1);
      char key = getKey();
      if (key == '0') {
        quitter = true;
        break;  
      } 
      else if (key == '#') {
        defileMesures(NB_ENTREES, NB_LIGNES_LCD-1);    
      }   
    }             
  } while (quitter == false);
  selectMenu(TAG_ECRAN_ACCUEIL);
}

//////////////////////////////////////////////////////////////////////////////////////

void afficherEntreesNumeriques()
{
  const byte NB_ENTREES = 4;
  String menu[NB_ENTREES];
  boolean quitter = false;
  do {        
    for (int i=0; i<NB_ENTREES; i++) {
      dtostrf(digitalRead(ENTREE[8+i]), 1, 0, ligne);
      menu[i] = "BROCHE " + String(27-i) + ": " + ligne;
    }    
    lcd.setCursor(0,0);
    lcd.print("ENTREES NUMERIQUES");
    for (int i=0; i<NB_LIGNES_LCD-1; i++) {
      lcd.setCursor(0,i+1);
      int index = i + defilement;
      if (index >= NB_ENTREES) {
        index -= NB_ENTREES;
      }
    lcd.print(menu[index]);
    }  
    for (int i=0; i<100; i++) {
      delay(1);
      char key = getKey();
      if (key == '0') {
        quitter = true;
        break;  
      } 
      else if (key == '#') {
        defileMesures(NB_ENTREES, NB_LIGNES_LCD-1);    
      }   
    }             
  } while (quitter == false);
  selectMenu(TAG_ECRAN_ACCUEIL);
}

//////////////////////////////////////////////////////////////////////////////////////
/*
    LES CAPTEURS INTERNES
*/
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
/*
    BARO-ALTIMETRE BMP180
*/
//////////////////////////////////////////////////////////////////////////////////////

void afficherBaroAltimetre(int adresseI2C)
{
  // Initialisation du baro-altimètre
  initBaroAltimetre(adresseI2C);
  
  // Réglage du baro-altimètre
  reglageBaroAltimetre(adresseI2C);
  
  // Affichage des mesures
  byte degre[8] = {   // Déclaration d’un tableau de 8 octets pour le caractère °.
    B00111,           // Définition de chaque octet au format binaire :
    B00101,           // 1 pour un pixel affiché – 0 pour un pixel éteint.
    B00111,           // Les 3 bits de poids forts sont ici inutiles.
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };
  const byte DEGRE = 1;
  lcd.createChar(DEGRE, degre); // Création du caractère personnalisé degré 
  boolean quitter = false;
  lcd.clear();
  do {
    byte codeErreur = barometreRead(adresseI2C);
    if (codeErreur != 0) {
      lcd.setCursor(0,0);
      lcd.print("BARO-ALTIMETRE");
      lcd.setCursor(0,1);
      lcd.print("ERREUR (CODE ");
      lcd.print(codeErreur);
      lcd.print(")");
      bipErreur();
      delay(1000);
      quitter = true;
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("BARO-ALTIMETRE");
      /*dtostrf(temperatureBMP180, 5, 1, ligne);
      lcd.print(ligne);
      lcd.write(DEGRE);
      lcd.print("C");*/      
      lcd.setCursor(0,1);
      lcd.print("Pa : ");
      dtostrf(pressionAbsolue, 6, 1, ligne);
      lcd.print(ligne);
      lcd.print(" hPa");
      lcd.setCursor(0,2);
      lcd.print("Pr : ");
      dtostrf(pressionRelative, 6, 1, ligne);
      lcd.print(ligne);
      lcd.print(" hPa");
      lcd.setCursor(0,3);
      lcd.print("H  : ");
      dtostrf(altitude, 6, 1, ligne);
      lcd.print(ligne);
      lcd.print(" m");
      for (int i=0; i<100; i++) {
        delay(1);
        char key = getKey();
        if (key == '0') {
          quitter = true;
          break;  
        }        
      }         
    }
  } while (quitter == false);
  selectMenu(TAG_ECRAN_ACCUEIL);
}

//////////////////////////////////////////////////////////////////////////////////////

void reglageBaroAltimetre(int adresseI2C)
{
  boolean altitudeOK = false;
  altitudeReference = 0;
  int colonne = 15; // Position du point d'interrogation (voir pus bas)
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("REGLAGE");
  lcd.setCursor(0,1);
  lcd.print("ALTITUDE (m) : ?");
  lcd.setCursor(0,2);
  lcd.print("# : EFFACER");   
  lcd.setCursor(0,3);
  lcd.print("* : VALIDER");  
  boolean quitter = false;
  do {
    char touche = getKey();
    if (touche != NO_KEY) { // Si une touche est actionnée
      switch (touche) {
        case '#': // Effacer
          altitudeOK = false;
          lcd.setCursor(0,1);
          lcd.print("ALTITUDE (m) : ?    ");
          colonne = 15;
          altitudeReference = 0;
          break;
        case '*': // Valider
          if (altitudeOK) {
            barometreRead(adresseI2C);
            pressionReference = pressionAbsolue;
            quitter = true;
          }
          break;
        default :
          if (colonne < NB_COLONNES_LCD-1) {
            altitudeOK = true;
            lcd.setCursor(colonne,1);
            lcd.print(valeurTouche(touche));
            altitudeReference = 10 * altitudeReference + valeurTouche(touche);
            colonne++;
            while (getKey() != NO_KEY); // On boucle tant que la touche pressée n'est pas relaché
          }
      }
    }
  } while (quitter == false);  
}

//////////////////////////////////////////////////////////////////////////////////////

int16_t  ac1, ac2, ac3, b1, b2, mb, mc, md; // Calibration coefficients
uint16_t ac4, ac5, ac6;                     // Calibration coefficients
// Ultra low power       : oss = 0, osd =  5 ms
// Standard              : oss = 1, osd =  8 ms
// High resolution       : oss = 2, osd = 14 ms
// Ultra high resolution : oss = 3, osd = 26 ms
const uint8_t OSS = 3;     // Set oversampling setting
const uint8_t OSD = 26;    // with corresponding oversampling delay

//////////////////////////////////////////////////////////////////////////////////////

void initBaroAltimetre(int adresseI2C) // Voir le Data sheet du BMP180, à la page 15.
{
  // Read calibration data from the EEPROM of the BMP180
  ac1 = readRegister16(adresseI2C, 0xAA);
  ac2 = readRegister16(adresseI2C, 0xAC);
  ac3 = readRegister16(adresseI2C, 0xAE);
  ac4 = readRegister16(adresseI2C, 0xB0);
  ac5 = readRegister16(adresseI2C, 0xB2);
  ac6 = readRegister16(adresseI2C, 0xB4);
  b1  = readRegister16(adresseI2C, 0xB6);
  b2  = readRegister16(adresseI2C, 0xB8);
  mb  = readRegister16(adresseI2C, 0xBA);
  mc  = readRegister16(adresseI2C, 0xBC);
  md  = readRegister16(adresseI2C, 0xBE);
/*  
  Serial.println("Sensor calibration data :");
  Serial.print("AC1 = "); Serial.println(ac1);
  Serial.print("AC2 = "); Serial.println(ac2);
  Serial.print("AC3 = "); Serial.println(ac3);
  Serial.print("AC4 = "); Serial.println(ac4);
  Serial.print("AC5 = "); Serial.println(ac5);
  Serial.print("AC6 = "); Serial.println(ac6);
  Serial.print("B1 = ");  Serial.println(b1);
  Serial.print("B2 = ");  Serial.println(b2);
  Serial.print("MB = ");  Serial.println(mb);
  Serial.print("MC = ");  Serial.println(mc);
  Serial.print("MD = ");  Serial.println(md);
*/
}

//////////////////////////////////////////////////////////////////////////////////////

uint16_t readRegister16(int adresseI2C, uint8_t code)
{
  uint16_t value = 0;
  Wire.beginTransmission(adresseI2C);         // Start transmission to device 
  Wire.write(code);                           // Sends register address to read from
  byte error = Wire.endTransmission();        // End transmission
  if (error == 0) {
    Wire.requestFrom(adresseI2C, 2);          // Request 2 bytes from device
    while (Wire.available() < 2);             // Wait until bytes are ready
    value = (Wire.read() << 8) + Wire.read();
  }
  return value;
}

//////////////////////////////////////////////////////////////////////////////////////

byte barometreRead(int adresseI2C) // Voir le Data sheet du BMP180, à la page 15.
{
  int32_t x1, x2, x3, b3, b5, b6, ut, up, t, p;
  uint32_t b4, b7;
  int16_t msb, lsb, xlsb;
  byte error = 0;
  
  // Read uncompensated temperature value (ut)
  Wire.beginTransmission(adresseI2C);          // Start transmission to device
  Wire.write(0xf4);                            // Sends register address
  Wire.write(0x2e);                            // Write data
  error = Wire.endTransmission();              // End transmission 
  if (error == 0) { // On continue
    delay(5);                                  // Data sheet suggests 4.5 ms 
    
    Wire.beginTransmission(adresseI2C);        // Start transmission to device
    Wire.write(0xf6);                          // Sends register address to read from
    error = Wire.endTransmission();            // End transmission
    if (error == 0) { // On continue
      Wire.requestFrom(adresseI2C, 2);         // Request 2 bytes (0xf6, 0xf7)
      while (Wire.available() < 2);            // Wait until bytes are ready
      msb = Wire.read();
      lsb = Wire.read();
  
      ut = ((int32_t)msb << 8) + (int32_t)lsb;
  
      // Read uncompensated pressure value (up)
      Wire.beginTransmission(adresseI2C);      // Start transmission to device
      Wire.write(0xf4);                        // Sends register address
      Wire.write(0x34 + (OSS << 6));           // Write data
      error = Wire.endTransmission();          // End transmission
      if (error == 0) { // On continue
        delay(OSD);                            // Oversampling setting delay
  
        Wire.beginTransmission(adresseI2C);    // Start transmission to device
        Wire.write(0xf6);                      // Sends register address to read from
        error = Wire.endTransmission();        // End transmission
        if (error == 0) { // On continue
          Wire.requestFrom(adresseI2C, 3);     // Request 3 bytes (0xf6, 0xf7, 0xf8)
          while (Wire.available() < 3);        // Wait until bytes are ready
          msb = Wire.read();
          lsb = Wire.read();
          xlsb = Wire.read();
  
          up = (((int32_t)msb << 16) + ((int32_t)lsb << 8) + ((int32_t)xlsb)) >> (8 - OSS);
  
          // Calculate true temperature
          x1 = (ut - (int32_t)ac6) * (int32_t)ac5 >> 15;
          x2 = ((int32_t)mc << 11) / (x1 + (int32_t)md);
          b5 = x1 + x2;
          t = (b5 + 8) >> 4;
          temperatureBMP180 = t / 10.0f;  // temperature in celsius                         
  
          // Calculate true pressure
          // On étend la taille de certaines variables pour éviter des dépassements.
          // Par exemple, dans la 2ème ligne, x1 est int32_t, b2 est int16_t
          // et b6 est int_32t d'où le (int32_t)b2.
          // Pour gagner en vitesse de calcul, on utilise << ou >> :
          // << : un décalage de 1 bit vers la gauche revient à multiplier par 2
          // >> : un décalage de 1 bit vers la droite revient à diviser par 2
          b6 = b5 - 4000;          
          x1 = ((int32_t)b2 * (b6 * b6 >> 12)) >> 11;          
          x2 = (int32_t)ac2 * b6 >> 11;          
          x3 = x1 + x2;          
          b3 = ((((int32_t)ac1 * 4 + x3) << OSS) + 2) >> 2;          
          x1 = (int32_t)ac3 * b6 >> 13;          
          x2 = ((int32_t)b1 * (b6 * b6 >> 12)) >> 16;
          x3 = ((x1 + x2) + 2) >> 2;
          b4 = ((uint32_t)ac4 * (uint32_t)(x3 + 32768)) >> 15;
          b7 = ((uint32_t)up - (uint32_t)b3) * (uint32_t)(50000 >> OSS);
          if (b7 < 0x80000000) { p = (b7 << 1) / b4; }
          else { p = (b7 / b4) << 1; }
          x1 = (p >> 8) * (p >> 8);
          x1 = (x1 * 3038) >> 16;
          x2 = (-7357 * p) >> 16;
          p = p + ((x1 + x2 + 3791) >> 4);
          pressionAbsolue = p / 100.0f;  // pression in hPa
  
          // Calculate pressure at sea level
          pressionRelative = pressionAbsolue / pow((1.0f - (altitudeReference / 44330.0f)), 5.255f);
          
          // Calculate absolute altitude
          float pression0 = pressionReference / pow((1.0f - (altitudeReference / 44330.0f)), 5.255f);
          altitude = 44330.0f * (1 - pow(pressionAbsolue / pression0, (1 / 5.255f)));
        }
      }
    }
  }
  return error;
}

//////////////////////////////////////////////////////////////////////////////////////
/*
    LUXMETRE
*/
//////////////////////////////////////////////////////////////////////////////////////

void afficherLuxmetre(int adresseEA)
{
  boolean quitter = false;
  do {
    long eclairement = luxmetreRead(adresseEA);
    if ((eclairement < 3) || (eclairement > 55000)) {
      bipErreur();
    } 
    lcd.setCursor(0,0);
    lcd.print("LUXMETRE");    
    lcd.setCursor(0,1);
    lcd.print("E : ");
    dtostrf(eclairement, 5, 0, ligne);
    lcd.print(ligne);
    lcd.print(" lx");
    
    for (int i=0; i<100; i++) {
      delay(1);
      char key = getKey();
      if (key == '0') {
        quitter = true;
        break;  
      }        
    }
  } while (quitter == false);
  selectMenu(TAG_ECRAN_ACCUEIL);
}

//////////////////////////////////////////////////////////////////////////////////////

long luxmetreRead(int adresseEA)
{
  #define NBREADINGS 5
  const long R = 68000; // Résistance placée par Adafruit sur le module
                        // (entre Iout et GND)
  long total = 0;
  for(int i=0; i<NBREADINGS; i++) {
    total = total + analogRead(adresseEA);
    delay(20); // On laisse un peu de temps au capteur
  }
  float moyenne = float(total) / NBREADINGS; // On calcule la moyenne
  
  float volts = moyenne * 5 / 1023.0;
  float amperes = volts / R;
  float microAmperes = amperes * 1000000;
  long lux = long(pow(10, microAmperes/10)); // Voir datasheet : Io=10.log(Ev)
  return lux;
}

//////////////////////////////////////////////////////////////////////////////////////
/*
    THERMOMETRE A BASE DE LM35CZ / BMP180
*/
//////////////////////////////////////////////////////////////////////////////////////

void afficherThermometre(int adresseEA)
{
  boolean quitter = false;
  do {
    // Création du caractère °
    byte degre[8] = {   // Déclaration d’un tableau de 8 octets pour le caractère °.
      B00111,           // Définition de chaque octet au format binaire :
      B00101,           // 1 pour un pixel affiché – 0 pour un pixel éteint.
      B00111,           // Les 3 bits de poids forts sont ici inutiles.
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    };
    const byte DEGRE = 1;
    lcd.createChar(DEGRE, degre); // Création du caractère personnalisé degré

    // LM35CZ
    float temperatureLM35CZ = thermometreRead(adresseEA);
    lcd.setCursor(0,0);
    lcd.print("THERMOMETRE");    
    lcd.setCursor(0,1);
    lcd.print("T LM35CZ  : ");
    dtostrf(temperatureLM35CZ, 5, 1, ligne);
    lcd.print(ligne);
    lcd.write(DEGRE);
    lcd.print("C");

    // BMP180
    // Initialisation du baro-altimètre
    initBaroAltimetre(ADRESSE_BMP180);
    // Lecture de la température notamment
    byte codeErreur = barometreRead(ADRESSE_BMP180);
    if (codeErreur != 0) {
      lcd.setCursor(0,2);
      lcd.print("T BMP180  :  ERREUR");
      /*lcd.print("ERREUR (CODE ");
      lcd.print(codeErreur);
      lcd.print(")");*/
      bipErreur();
      delay(1000);
      quitter = true;
    }
    else {
      lcd.setCursor(0,2);
      lcd.print("T BMP180  : ");
      dtostrf(temperatureBMP180, 5, 1, ligne);
      lcd.print(ligne);
      lcd.write(DEGRE);
      lcd.print("C");
    }

    // Moyenne du LM35CZ et BMP180
    float temperature = (temperatureLM35CZ + temperatureBMP180) / 2;
    lcd.setCursor(0,3);
    lcd.print("T MOYENNE : ");
    dtostrf(temperature, 5, 1, ligne);
    lcd.print(ligne);
    lcd.write(DEGRE);
    lcd.print("C");    
    
    for (int i=0; i<100; i++) {
      delay(1);
      char key = getKey();
      if (key == '0') {
        quitter = true;
        break;  
      }        
    }
  } while (quitter == false);
  selectMenu(TAG_ECRAN_ACCUEIL);
}

//////////////////////////////////////////////////////////////////////////////////////

float thermometreRead(int adresseEA)  // Seulement le LM35CZ
{
  #define NBREADINGS 5
  long total = 0;
  for(int i=0; i<NBREADINGS; i++) {
    total = total + analogRead(adresseEA);
    delay(20); // On laisse un peu de temps au capteur
  }
  float moyenne = float(total) / NBREADINGS; // On calcule la moyenne

  // Convertit moyenne en une tension en volts (un simple produit en croix). 
  float tension = 5.0 * moyenne / 1023.0;
  
  // L'amplification avec l'ampli op non inverseur est de Vs/Ve = (1 + R3 / R2)
  // R3 théorique = 39 kilohms
  // R2 théorique = 10 kilohms
  // Amplification théorique = (1 + (39 / 10)) = 4.9
  // Sur mon montage, quand en sortie de capteur on a 0,21 V,
  // au niveau de l'entrée analogique de la carte Arduino on a 1.04 V.
  // L'amplification mesurée est donc de 1.04 / 0,21 = 4.95.
  float tension_avant_amplification = tension / 4.9;
  
  // Le capteur de température LM35CZ fournit une 
  // réponse linéaire de 10 mV (0,01 V) par °C 
  // dans l'intervalle [+ 2 °C ; + 110 °C].  
  float temperature = tension_avant_amplification / 0.01;
  
  return temperature;
}

//////////////////////////////////////////////////////////////////////////////////////
