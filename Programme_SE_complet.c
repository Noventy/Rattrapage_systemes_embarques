/////////////////////////////////////////////////////////////////
/////////////////////// Bibliothèque ////////////////////////////

#include <ChainableLED.h>
#include "DHT.h"
#include <Wire.h>
#include "RTClib.h"
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

/////////////////////////////////////////////////////////////////
/////////////////////// Define //////////////////////////////////
#define NUM_LEDS 1
#define DHTPIN A0
#define DHTTYPE DHT11

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

String DATA_STRING;
String GPS_DATA;
bool *ETAT_SD;

/////////////////////////////////////////////////////////////////
/////////////////////// Prototype ///////////////////////////////

void red_interruption();
void green_interruption();

void red_button();
void green_button();

void start_syst();
void stand_mode();
void config_mode();
void maint_mode();
void eco_mode();

void date();
void gps();
void lum_capt();
void hygr_capt();
void lum_capt();

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

volatile int key = 0;

long prev_millis = 0;
int led_delay =  500;
int capt_delay = 3000;

byte pos = 0;
int q1 = false;
ChainableLED leds(7, 8, NUM_LEDS);
const int pinLight = A1;
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

SoftwareSerial SoftSerial(4, 5);
String data;

unsigned long start_rpress; // pour mesurer le temps d'appui
unsigned long end_rpress; // pour mesurer le temps d'appui
bool press_rbutton = false;

unsigned long start_gpress; // pour mesurer le temps d'appui
unsigned long end_gpress; // pour mesurer le temps d'appui
bool press_gbutton = false;

/////////////////////////////////////////////////////////////////
/////////////////////// Setup ///////////////////////////////////

void setup()
{
  Serial.begin(9600);
  leds.init();
  dht.begin();

  Serial.print(" Initialisation de la carte SD :");

  if (!SD.begin(4))
  {
    Serial.println(" Erreur");
    sd_error();
    return;
  }
  Serial.println(" Réussie");

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), red_interruption, CHANGE);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), green_interruption, CHANGE);


  #ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  SoftSerial.begin(9600); 
  
}

/////////////////////////////////////////////////////////////////
/////////////////////// Boucle principale ///////////////////////

void loop()
{ 
  if(millis()<10000)
    {
      start_syst();
      if(key == 1)
      {
        config_mode();
        delay(10000);
        key = 0;
      }
  }
  if(millis()>10000)
  {
    switch(key)
    {
     case 0:
      stand_mode();
     break;

     case 1:
      eco_mode();
     break;

     case 2:
      maint_mode(ETAT_SD);
     break;
    }
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Interruption pour le bouton rouge ///////

void red_interruption()
{
  if(millis()<10000) //temps < 10 secondes
  {
  red_button(); //alors on declanche la foncntion 
  }

  if(millis()>10000) //temps > 10 secondes
  {
   byte rbutton_position =  digitalRead(3); //on lie la valeur du bouton rouge
   if((rbutton_position == LOW) && (press_rbutton==false)) //si on appuie
   {
    press_rbutton = true;
    start_rpress = millis(); //on commence à compter
   } 
   else if (( rbutton_position == HIGH) && (press_rbutton) && (millis()-start_rpress<=5000)) //si on arrete d'appuyer avant les 5 secondes 
   {
    end_rpress = millis(); // on arrete de compter et rien ne se passe
    press_rbutton = false;
   }
   else if (( rbutton_position == HIGH) && (press_rbutton) && (millis()-start_rpress>=5000)) //si on maintient l'appuie pendant 5 seconde ou plus
   {
    red_button(); // on effectue la fonction du bouton rouge
    end_rpress = millis(); // et on arrete de compter
    press_rbutton = false;
   }
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Interruption pour le bouton vert ////////

void green_interruption()
{
  byte gbutton_position =  digitalRead(2); //on lie la valeur du bouton vert
  if((gbutton_position == LOW) && (press_gbutton==false)) // si il est appuyé
  {
    press_gbutton = true;
    start_gpress = millis(); //on commence à compter
  } 
  else if (( gbutton_position == HIGH) && (press_gbutton) && (millis()-start_gpress<=5000)) //si le relache avant les 5 secondes
  {
    end_gpress = millis();
    press_gbutton = false; //on arrete de compter et il ne se passe rien
  }
  else if (( gbutton_position == HIGH) && (press_gbutton) && (millis()-start_gpress>=5000)) //si on ne relache pas avant les 5 secondes
  {
    green_button(); //on effectue la fonction lié au bouton vert
    end_gpress = millis(); //et on arrete de compter
    press_gbutton = false;
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Bouton rouge ////////////////////////////


void red_button()
{
  if(millis() <10000 )
  {
   if(key == 0)
    {
     key = 1;
    }
  }
  if(millis() >= 10000)
  {
    if(key == 2 || key == 1)//si maintenance ou economique alors standard
    {
      key = 0;
    }
    else if (key == 0) //si standard alors maintenance
    {
      key = 2;
    }
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Bouton vert /////////////////////////////

void green_button()
{
  if(millis()>10000)
  {
    if(key == 0)//si standard alors economique
    {
      key=1;
    }
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Démarrage système ///////////////////////


void start_syst()// blanc qui clignote
{
  if(millis() - prev_millis > led_delay)
  {
   leds.setColorRGB(pos, 100,100, 100);
   delay(300);
   prev_millis = millis();
   leds.setColorRGB(pos, 0,0, 0);
   prev_millis = millis();
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Mode Standard ///////////////////////////

void stand_mode() //vert
{
  leds.setColorRGB(pos, 0,128, 0);
  q1 = false;
  date();
  capt_delay = millis();
  gps_capt();
  capt_delay = millis();
  lum_capt();
  capt_delay = millis();
  hygr_capt();
  capt_delay = millis();
  temp_capt();
  capt_delay = millis();
}

/////////////////////////////////////////////////////////////////
/////////////////////// Mode Economique /////////////////////////

void eco_mode() // bleu
{
  leds.setColorRGB(pos, 0,0, 255);
  if(q1 == false)
  {
    q1 = true;
    Serial.println(F("Vous êtes dans le mode economique, les capteurs sont désactivés afin d'éconnomiser la batterie"));
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Mode Configuration //////////////////////

void config_mode() //jaune
{
  leds.setColorRGB(pos, 255,255, 0);
  Serial.println(F("Vous êtes dans le mode economique, les capteurs sont désactivés afin d'éconnomiser la batterie"));
}

/////////////////////////////////////////////////////////////////
/////////////////////// Mode Maintenance ////////////////////////

void maint_mode(bool *ETAT_SD)//orange
{
  leds.setColorRGB(pos, 255,127, 0);
  int a = '0';
  delay(300);
  if(q1 == false)
  {
    q1 = true;
    Serial.println(F("Vous etes dans le mode maintenance, les données des capteurs ne sont plus enregistrees"));
    Serial.println(F("Vous pouvez retirez la carte SD, n'oubliez pas de la remettre avant de repasser en mode standard"));
  }
  delay(500);
  Serial.println(F("Voulez-vous voir les données prisent par les capteurs ?"));
  Serial.println(F("1-oui"));
  Serial.println(F("2-non"));
  while(Serial.available() == 0){}
  a = Serial.read();
  if(a == '1')
  {
    date();
    capt_delay = millis();
    gps_capt();
    capt_delay = millis();
    lum_capt();
    capt_delay = millis();
    hygr_capt();
    capt_delay = millis();
    temp_capt();
    capt_delay = millis();
    a = '0';
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Luminosité //////////////////////////////


void lum_capt()
{
  int LUMIN_LOW = 200;
  int LUMIN_HIGH = 700;
  int sensorValue = analogRead(pinLight);
  DATA_STRING += String(sensorValue);
  DATA_STRING += "; ";

  if (sensorValue < LUMIN_LOW)
  {
    Serial.print("La luminosite est faible, ");
  }
  else if (sensorValue > LUMIN_HIGH){
        Serial.print("La luminosite est eleve, ");
  }
  else
  {
        Serial.print("La luminosite est normale, ");
  }
  Serial.print("Sensor = ");
  Serial.println(sensorValue);
}


/////////////////////////////////////////////////////////////////
/////////////////////// HUMIDITY ////////////////////////////////

void hygr_capt()
{
  bool HYGR = true;
  int HYGR_MINT = 0;
  int HYGR_MAXT = 100;

  if(HYGR == true)
  {
    int h = dht.readHumidity();
    if(isnan(h))
    {
      Serial.println(F("Failed to read from DHT sensor !"));
      return;
    }

    else if(h < HYGR_MINT || h > HYGR_MAXT)
    {
      inconsistent_data_error();
    }

    else
    {
      Serial.print(F("Humidity : "));
      Serial.print(h);
      Serial.print(F("%"));
      DATA_STRING += String(h);
      DATA_STRING += "%; ";

    }
  }
   
}

/////////////////////////////////////////////////////////////////
/////////////////////// TEMPERATURE /////////////////////////////

void temp_capt()
{
  bool TEMP_AIR = true;
  int MIN_TEMP_AIR = -10;
  int MAX_TEMP_AIR = 60;

  int t = dht.readTemperature();
  

  if(isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor !"));
    return;
  }

  else if(t < MIN_TEMP_AIR || t > MAX_TEMP_AIR)
  {  
    inconsistent_data_error();
  }

  else
  {
    Serial.print(F("\n Temperature :"));
    Serial.print(t);
    Serial.print(F("°C \n"));
    DATA_STRING += String(t);
    DATA_STRING += "°C";
    DATA_STRING += "";

  }  
}

/////////////////////////////////////////////////////////////////
/////////////////////// DATE ////////////////////////////////////

void date()
{
  DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

/////////////////////////////////////////////////////////////////
/////////////////////// GPS /////////////////////////////////////

void gps_capt()
{
  
  String GPS_DATA = "";
  data = "";
  GPS_DATA = SoftSerial.readStringUntil('\n');
  if (GPS_DATA.startsWith("$GPGGA",0))
  {
    data = GPS_DATA;
    Serial.println(data);
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////// Carte SD ////////////////////////////////
void SD_CARD()
{
  File dataFile = SD.open("datalog.txt", FILE_WRITE); // On ouvre le fichier
  if (dataFile)
  {
    dataFile.println(DATA_STRING);
    dataFile.println(GPS_DATA);
    dataFile.close();
    Serial.println(F("Ecriture ..."));
    
  }
  else 
  {
    Serial.println(F("error opening datalog.txt")); // Message d'erreur
  }
 }

/////////////////////////////////////////////////////////////////
/////////////////////// Erreur d'horloge ////////////////////////

void clock_error()// rouge et bleu
{
  leds.setColorRGB(pos, 255, 0, 0); 
  delay(500);
  leds.setColorRGB(pos, 0, 0, 255); 
  delay(500);  
}

/////////////////////////////////////////////////////////////////
/////////////////////// Erreur de GPS ///////////////////////////

void gps_error()//rouge et jaune
{
  leds.setColorRGB(pos, 255, 0, 0); 
  delay(500);
  leds.setColorRGB(pos, 255, 255, 0); 
  delay(500);
}

/////////////////////////////////////////////////////////////////
/////////////////////// Erreur de capteur ///////////////////////

void capt_error()//rouge et vert
{
  leds.setColorRGB(pos, 255, 0, 0);
  delay(500);
  leds.setColorRGB(pos, 0, 255, 0); 
  delay(500);
}

/////////////////////////////////////////////////////////////////
/////////////////////// Erreur de donnée capteur ////////////////

void inconsistent_data_error()//rouge et vert x2
{
  leds.setColorRGB(pos, 255, 0, 0); 
  delay(250);
  leds.setColorRGB(pos, 0, 255, 0); 
  delay(750);
}

/////////////////////////////////////////////////////////////////
/////////////////////// SD pleine ///////////////////////////////

void sd_full()// rouge et blanc
{
  leds.setColorRGB(pos, 255, 0, 0); 
  delay(500);
  leds.setColorRGB(pos, 255, 255, 255); 
  delay(500);
}

/////////////////////////////////////////////////////////////////
/////////////////////// Erreur de carte SD //////////////////////

void sd_error()// Rouge et blanc x2
{
  leds.setColorRGB(pos, 255, 0, 0); 
  delay(250);
  leds.setColorRGB(pos, 255, 255, 255); 
  delay(750);
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////