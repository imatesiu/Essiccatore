#include <SoftwareSerial.h>
#include <Stepper.h>
#include "note.h"

// Messaggi Monitor Nextion
// l = Led ON c = led cambio stato
// o = Led OFF
// m = Motor ON t = Motor OFF
// b = Buzz
// &ssid:password$ = wifi config

//*****CONFIG****

int rx = 10;     //pin Monitor Nextion
int tx = 11;

int LED = 13;

int buzzPin = 2;      //Pin collegato al buzzer
int tempo = 1000;
/*
 * IN1 -> 8
 * IN2 -> 9
 * IN3 -> 10
 * IN4 -> 11
 */

// definizione dei PIN
const int stepPin = 3; 
const int dirPin = 4; 
const int stepForRevolution = 200;


//*****CONFIG_end****

SoftwareSerial monitorSerial(rx, tx); // RX, TX
String Data = "";

String ssid = "";
char pcharacter;
bool bandiera = false;
bool acceso = false;
bool motor = false;
bool buzz = false;

void setup() {
   Serial.begin(57600);
   //Serial3.begin(115200);
   monitorSerial.begin(115200);
   pinMode(LED, OUTPUT);
   pinMode(buzzPin,OUTPUT); 

  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
   //Motore in direzione oraria
  digitalWrite(dirPin,HIGH);

}

void loop() {
  monitorSerial.listen();
  if (monitorSerial.available()) {
    char character = monitorSerial.read();
    //Serial.println(character);
    if(pcharacter=='&' or bandiera){
      bandiera = true;
       Data.concat(character);
       if(character=='$'){
          ssid = Data.substring(0, Data.length()-1);
          Serial.println(ssid); 
          Data = "";
          bandiera = false;
       }
    }
    if(pcharacter=='#'){
      //Serial.println(character);
      switch(character){
       case 'l' : { Serial.println("Led On"); digitalWrite(LED, HIGH); acceso=true; break;}
       case 'o' : { Serial.println("Led Off"); digitalWrite(LED, LOW);  acceso=false; break;}
       case 'c' : { Serial.println("Lec C"); 
           if(acceso){
            digitalWrite(LED, LOW); acceso =false;
           }else{
            digitalWrite(LED, HIGH); acceso = true;
           }
          break;
       }
       case 'm' : { Serial.println("Motor On"); motor = true; break;}
       case 't' : { Serial.println("Motor Off"); motor = false; break;}
       case 'b' : { Serial.println("Buzz"); buzz = true; break;}
       //default: {Serial.println("Comando Sconosciuto"); break;}
       
      }
      Data = "";
    }
    pcharacter = character;
  }
  if(motor){
    ruota();
  }
  if(buzz){
    suona(NOTE_C4,4);
    suona(NOTE_D4,4);
    suona(NOTE_E4,4);
    suona(NOTE_F4,4);
    suona(NOTE_G4,4);
    suona(NOTE_A4,4);
    suona(NOTE_B4,4);
    suona(NOTE_C5,4);
  }
}

void suona(unsigned int nota, long durata){
  tone(buzzPin, nota, tempo / durata);    
  delay(tempo / durata);
  noTone(buzzPin);    
//A fine riproduzione del suono, questo viene manualmente interrotto,
//necessario solo nel caso in cui si utilizzino più pin per controllare il buzzer
}

void ruota(){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(2000); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(2000); 
}
