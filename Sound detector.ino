#include <DFPlayer_Mini_Mp3.h>

#define pin1 2
#define dbugactive true

bool readData = digitalRead(pinData);

void setup() {
 mp3_set_volume(30);
 mp3_play(1);
 pinMode(pin1, input);
}

void loop() {
  //sensor de som
  if (readData == HIGH) {
   unsigned long int deltaTimeMin = 250;
   unsigned long int deltaTimeMax = 500;
   //Se sentir som
   if ((millis() - 0) >= deltaTimeMin && (millis() - 0) <= deltaTimeMax) {
     mp3_play (11);
      
     if (dbugActive == true) {
       Serial.print("Sound!");
     }
     
     deltaTimeMin = millis() + 250;
     deltaTimeMax = millis() + 500;
   }
 }//end song

