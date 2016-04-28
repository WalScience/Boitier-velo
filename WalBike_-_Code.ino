//WalScience
//2016
//WalOS 1.0

//Insert les librairies
#include <Wire.h>
#include <LiquidCrystal.h>
#include <DS1302RTC.h>
#include <Time.h>
#include <dht11.h>
#include <SD.h>
#include <SPI.h>

LiquidCrystal lcd(A0, A1, 5, 4, 3, 2); //Déclare les pins du lcd et nomme le lcd
 
dht11 DHT11; //Nomme le capteur de T°
#define DHT11PIN 6 //Définie le pin du capteur de T°

DS1302RTC RTC(9, 7, 8); //Déclare les pins de la rtc et nomme la rtc

#define CS_PIN 10
File monFichier;

int CapteurPin = A6;
int Capteur = 0;  

int affichageBatterie;
int ValBatterie;

float distanceparcourue = 0.61;
float distancetotalparcourue;


float vitessems;
int vitesse1;
float vitesse23;

int minutes;
int heures;
int seconde;
int anciensecondes;

int Droite;
int DroitePin;
int EtatDroit;
int ledDroite;

int Gauche;
int GauchePin;
int EtatGauche;
int ledGauche;


byte degre[8]={B01110, B01010, B01110, B00000, B00000, B00000, B00000, B00000}; 


void setup() 
{
  pinMode(CapteurPin, INPUT); //Definition de la pin 13 (celle du capteur) en entrée
  
  pinMode(ledGauche, OUTPUT);     
  pinMode(GauchePin, INPUT);
  
  pinMode(ledGauche, OUTPUT);     
  pinMode(GauchePin, INPUT);
  
  lcd.begin(20,4); //Définie la taille de l'afficheur lcd
  lcd.createChar(0, degre); //Crée le caractère spécial nommé "0"

  setSyncProvider(RTC.get); //Recuperation du temps du module RTC
  
  lcd.setCursor(4,0);
  lcd.print("WAL SCIENCE");
  delay(1500);
  
  lcd.setCursor(1,0);
  lcd.print("Initialisation...");
  delay(2000);
  
  if (SD.begin())
  {
    lcd.setCursor(1,0);
    lcd.print("SD valide");
    delay(1000);
  } 
  
  else
  {
    lcd.setCursor(1,0);
    lcd.print("ERREUR SD");
    delay(3000);
    return;
  }  
  
  monFichier = SD.open("Mesure.csv", FILE_WRITE);
  if (monFichier) 
  {   
    monFichier.println("Date,mois,heure:minute,Temperature,Vitesse,Distance,Chrono");
    monFichier.close();    
  } 

  lcd.clear();
  //delay(100); //P'tit delay OKLM
  
}


void loop() 
{
  //Appel de toute les fonctions
  calcul_distance();
  calcul_vitesse();
  mesure_temperature();
  heure();
  temps();
  lecture_batterie();
  Carte_SD();
  Clignotant_Gauche();
  Clignotant_Droit();

}

void calcul_vitesse()
{

  int seconde = millis()/1000; //Recuperation du temps en seconde pour le calcul de vitesse moyenne
  vitessems = distancetotalparcourue / seconde; //Calcul de la vitesse moyenne en m/s
  vitesse1 = vitessems * 3.6; //Vitesse m/s --> km/h

   //Affichage de la vitesse
  lcd.setCursor(0,1);
  lcd.print("VITESSE: ");
  lcd.print(vitesse1);
  lcd.setCursor(13,1);
  lcd.print("km/h");

}

void mesure_temperature()
{
  //Mesure la température 
  int chk = DHT11.read(DHT11PIN);

  //Affiche la température
  lcd.setCursor(0,0);
  lcd.print(DHT11.temperature-4); 
  lcd.setCursor(3,0);
  lcd.print("C");
  lcd.setCursor(2,0);
  lcd.write((byte)0);
}

void heure()
{
  static int sday = 0; 
  int minute2 = minute()- 15;
  lcd.setCursor(7, 0);
  lcd.print(hour()+10);
  lcd.print(":");
  if(minute2<10) {
  lcd.print("0");
  }
  lcd.print(minute()-15);
   
 
}


void lecture_batterie() {
  ValBatterie = analogRead(A0);
  
  affichageBatterie = map(ValBatterie, 970, 1023, 0, 100);
  
  if (affichageBatterie > 99) {
    lcd.setCursor(16,0);
  }
  else  {
    lcd.setCursor(17,0);
  }

  lcd.print(ValBatterie);
  lcd.setCursor(19,0);
  lcd.print("%");
  delay(100);
}

void temps()
{
  heures=millis()/3600000;
  lcd.setCursor(6, 4);
  minutes=millis()/60000;
  seconde=millis()/1000;
 
  
  seconde = (millis() / 1000) - anciensecondes;
  if(seconde>= 60) {seconde = 0; anciensecondes = millis() / 1000; minutes++;}
  if(minutes >= 60) {minutes = 0; heures++;}
 
  lcd.print(heures);
  lcd.print(":");
  if(minutes < 10) {lcd.print("0");}
  lcd.print(minutes);
  lcd.print(":");
  if(seconde < 10) {lcd.print("0");}
  lcd.print(seconde);
}

void calcul_distance() {

Capteur = digitalRead(CapteurPin);

 if (Capteur == LOW) {

distancetotalparcourue = distancetotalparcourue+1.9;
delay(300);
 
  }


  lcd.setCursor(0,2);
  lcd.print("Distance: "); 
  lcd.print(distancetotalparcourue);
  lcd.setCursor(19,2);
  lcd.print("m");
  

}

void Carte_SD() {

  String info = String(day()+","+month()) + "," + String(hour()+":"+minute()) + ":" + DHT11.temperature + "," + String(vitesse1) + "," + String(distancetotalparcourue) + "," + String(heures+":"+minutes);

 
  monFichier = SD.open("donnees.csv", FILE_WRITE); 
  if (monFichier) 
  {   
    monFichier.println(info);
    monFichier.close();    
  } 
  else 
  {
    lcd.setCursor(3,0);
    lcd.print("ERREUR FICHIER");
  }  
  delay(30000);
}

void Clignotant_Droit() {
  Droite = digitalRead(DroitePin);
 

  if (Droite == HIGH) {    

    if (EtatDroit == 1) {
      digitalWrite(ledGauche, LOW);
      
      digitalWrite(ledDroite, HIGH);
      delay(1000); 
      digitalWrite(ledDroite, LOW);
      delay(1000);
      
      EtatDroit=0;

    }  
         
     
  }
  else {

    EtatDroit=1;
    digitalWrite(ledDroite, LOW); 
  }
}
  
  
void Clignotant_Gauche() {
  Gauche = digitalRead(GauchePin);
 

  if (Gauche == HIGH) {    

    if (EtatGauche == 1) {
  
      digitalWrite(ledDroite, LOW);
      
      digitalWrite(ledGauche, HIGH);
      delay(1000); 
      digitalWrite(ledGauche, LOW);
      delay(1000);
      
      EtatGauche=0;

    }  
         
     
  }
  else {

    EtatGauche=1;
    digitalWrite(ledGauche, LOW);
      
  }
}
