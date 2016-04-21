#include <Wire.h>
#include <LiquidCrystal.h>
#include <DS1302RTC.h>
#include <Time.h>
#include <dht11.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
dht11 DHT11; 

#define DHT11PIN 6 

DS1302RTC RTC(9, 7, 8);


const int CapteurPin = 13;
int interrupteur = LOW;       
int etat = 0;             
int Capteur = 0;  
int tempsa;
int tempsb;
int compteur;
float vitesse1=0;
float tempssec = 0;
int affichageBatterie;
float distanceparcourue = 0.61;
int ValBatterie;
int distancetotalparcourue;
int BouttonStop=A2;
float vitessems;
int minutes;
int heures;
int seconde;

 byte degre[8]={
 B01110,
 B01010,
 B01110,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000}; 

int anciensecondes;


void setup() 
{
lcd.begin(20,4);
  lcd.createChar(0, degre); 


  setSyncProvider(RTC.get); 

  delay (200);
  lcd.clear();
}


void loop() 
{

  calcul_vitesse_et_distance();
  mesure_temperature();
  heure();
temps();
  lecture_batterie();
    boutton_stop();
}

void calcul_vitesse_et_distance()
{
   Capteur = digitalRead(CapteurPin);

  if (Capteur == HIGH) {    

     distancetotalparcourue = distancetotalparcourue+0.61;
   

     
  }
  
 

   vitessems=distancetotalparcourue/seconde;
   vitesse1=vitessems*3.6;
  if(vitessems > 1000) {
   vitessems/1000;
  }
  lcd.setCursor(0,1);
  lcd.print("VITESSE: ");
  lcd.print(vitesse1);
  lcd.setCursor(13,1);
  lcd.print("km/h");

   lcd.setCursor(0,2);
  lcd.print("Distance: "); 
  lcd.print(distancetotalparcourue);
  lcd.setCursor(19,2);
  lcd.print("m");
  


    }

void mesure_temperature()
{
    int chk = DHT11.read(DHT11PIN); 
  lcd.setCursor(0,0);
  lcd.print(DHT11.temperature); 
  lcd.setCursor(3,0);
  lcd.print("C");
  lcd.setCursor(2,0);
  lcd.write((byte)0);
}

void heure()
{
static int sday = 0; 

 lcd.setCursor(7, 0);
  print2digits(hour()+2);
  lcd.print(":");
  print2digits(minute()-14);
   
   switch (hour()) 
   {
  case 24:
    lcd.print("00");
    break;
  case 25:
    lcd.print("01");
    break;

}

void print2digits(int number)
{
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);

}

void lecture_batterie() {
  ValBatterie = analogRead(A0);
  
  affichageBatterie = map(ValBatterie, 970, 1023, 0, 100);
  
if (affichageBatterie > 99) 
  {
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
 void temps() {

 
heures=millis()/3600000;
  lcd.setCursor(6, 4);



minutes=millis()/60000;




  seconde=millis()/1000;
 

 
 seconde = (millis() / 1000) - anciensecondes;
  if(seconde>= 60) {seconde = 0; anciensecondes = millis() / 1000; minutes++;}
  if(minutes >= 60) {minutes = 0; heures++;}
  if(heures < 10) {lcd.print("0");}
  lcd.print(heures);
  lcd.print(":");
  if(minutes < 10) {lcd.print("0");}
  lcd.print(minutes);
  lcd.print(":");
  if(seconde < 10) {lcd.print("0");}
  lcd.print(seconde);
  
   }
   
 


void boutton_stop() {
  BouttonStop=digitalRead(BouttonStop);
  if(BouttonStop == HIGH) {
  minutes=0;
  heures=0;
  seconde=0;
  distancetotalparcourue=0;
}
  
}
