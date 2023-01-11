/* code by Foxy Ayano/Sidney Enzo R. Silva And choco/phellip

   Project Name glasseres  bliblioteca baixadas da internet:       DFPlayer_Mini_Mp3.h
                           
   version 1.3.7           TinyGPS.h            EEPROM.h           dht.h

   date 12/21/2022         minhas blibliotecas: dbug.h a dbug.h ela funciona a base de      
                           um time e um liga e desliga e if para definir um dbug

   script C and C++    

 obijetive criar um óculos diferenciado com arduino com multi funções incluindo sensor de som, leitor de umidade do ar e temperatura, uma mini IA localização com satélites, dbug e otimizado.                                                                   */

/*bibliotecas
####################################################################*/

 //voz
 #include <DFPlayer_Mini_Mp3.h>
 #include <SoftwareSerial.h>
 SoftwareSerial serial8 (8,9);

 #include <TinyGPS.h>
 TinyGPS gps;

 //display
 #include <LiquidCrystal.h>
 LiquidCrystal display (2,4,10,11,12,13);

 //memoria
 #include <EEPROM.h>

 //umidade
 #include <dht.h>
 dht myDht;

/*variaveis globais
####################################################################*/

 String txt = " ";
 int desprezivel = 0;
 
 //pinos
 #define pinData 3
 #define dhtPin 5
 #define pinLed A0

 //definições 
 #define volume 30
 #define dbugActive false
 void dbugTime (int Time);

 //deltatime
 unsigned long int deltaMillis = 0;

 //memoria
 byte up, down;
 int memory;
 int x1 = 1;
 int x2 = 2;
 int x3 = 3;
 int x4 = 4;
 int x5 = 5;

 //ler dados
 char digitar = ' ';
 bool recebido = false;
 bool readData = digitalRead (pinData);
 float readDht = myDht.read11 (dhtPin);

/*configurações 
####################################################################*/

void setup() {

 //conf básica
 if ( dbugActive == true ) {
   Serial.begin (9600);
 }

 serial8.begin (9600);

 pinMode (pinData, INPUT);
 pinMode (pinLed, OUTPUT);

 mp3_set_volume (volume);
 
 //display
 display.begin(16,2);
 display.blink();

 //voz do arduino
 mp3_play (x1);
 mp3_play (x2);
 mp3_play (x3);

 delay (5000);

}

/*ler serial
####################################################################*/

void loop() {

 //ler serial
   while (serial8.available()) {
     char L = serial8.read();
     recebido = gps.encode(L);
   }
   
   while (Serial.available()) {
     digitar = Serial.read();
   }

/*ajuda
####################################################################*/

   if (desprezivel == 0) {
     Serial.print("to help, write H"); 
     Serial.print("para ajuda, escreva H.");
     desprezivel = 1;
   }
   
   if (digitar == 'H') {
     Serial.print("select linguage:");
     Serial.print("E = english, sorry we don't have englad english.");
     Serial.print("P = portuguese-br, sorry we don't have portuguese of portugal.");
     digitar = ' ';
    }//end help

/*tradução
####################################################################*/

   //ingles
   if (digitar == 'E') {
     x1 = 1;
     x2 = 2;
     x3 = 3;
     x4 = 4;
     x5 = 9;
     memory = Serial.parseInt();
     up = memory / 256;
     down = memory;
     EEPROM.write(0, up);
     EEPROM.write(1, down);
   } 
     // português br
     else if (digitar == 'P') {
       x1 = 5;
       x2 = 6;
       x3 = 7;
       x4 = 8;
       x5 = 10;
       memory = Serial.parseInt();
       up = memory / 256;
       down = memory;
       EEPROM.write(0, up);
       EEPROM.write(1, down);
     }//end traduct

/*sensores
####################################################################*/

   //sensor de som
   if (readData == HIGH) {
     unsigned long int deltaTimeMin = 250;
     unsigned long int deltaTimeMax = 500;
     //Se sentir som
     if ((millis() - 0) >= deltaTimeMin && (millis() - 0) <= deltaTimeMax) {
       digitalWrite (pinLed, HIGH);
       
       if (dbugActive == true) {
         Serial.print("Sound!");
       }
        
       deltaTimeMin = millis() + 250;
       deltaTimeMax = millis() + 500;
       
       dbugTime(10000);
     }
   }//end song

 //aviso de alta temperatura
 if (millis() - deltaMillis >= 5000) {
   deltaMillis = millis();
   
   //calculos
   float temperatureK = 0;
   int umidade = 0;
   int temperature = 0;
   
   //calculo
   temperatureK = myDht.temperature + 273.15;
   temperature = map (temperatureK, 0, 309.15, 0, 100);
   umidade = myDht.humidity;
   
   if (temperatureK >= 307.15) {
     mp3_play(x4);
     
     if (dbugActive == true) {
       Serial.print (temperatureK);
       Serial.println ("°K");
     }
     
     txt.concat("temperature: " + temperature + '%');
     display.print (txt);
      
     dbugTime(10000);
     
     if (millis() - deltaMillis >= 6990) {
       deltaMillis = millis();
       display.clear();
     }
   }//end temperature

  if (umidade >= 65) {
   mp3_play(x5);
   
    if (dbugActive == true) {
     Serial.print (umidade);
     Serial.println ('%');
    }
   
   txt.concat("humidity: " + umidade + '%');
   display.print (txt);
   
   dbugTime(10000);
   
    if (millis() - deltaMillis >= 1990) {
     deltaMillis = millis();
     display.clear();
    }
  }//end humidity and temperature
 }//end sensor dht
  
/*Localização 
####################################################################*/

 if (millis() - deltaMillis >= 7000) {
   
   deltaMillis = millis();
   
   if (recebido) {
     long longitude, latitude;
     float speed = 0;
     unsigned long int infoTime = 0;
     
     gps.get_position (&latitude, &longitude, &infoTime);
     
     if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
       txt.concat("latitude: " + latitude);
      if (dbugActive == true) {
        Serial.print ("latitude: ");
        Serial.print (latitude);
      }
     
     display.print(txt);
     dbugTime(10000);
    }
    
   if (millis() - deltaMillis >= 5000) {
     deltaMillis = millis();
     display.clear();
   }
   
   if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
     txt.concat("longitude: " + longitude);
     if (dbugActive == true) {
       Serial.print ("longitude: ");
       Serial.print (longitude);
     }
     
     display.print(txt);
     dbugTime(10000);
     
     if (millis() - deltaMillis >= 5000) {
       deltaMillis = millis();
       display.clear();
     }
   }
   
   if (infoTime != TinyGPS::GPS_INVALID_F_ANGLE) {
    txt.concat ("info time: " + infoTime);
      if (dbugActive == true) {
       Serial.print ("info time: ");
       Serial.print (infoTime);
      }
    
    display.print (txt);
    dbugTime(10000);
   
     if (millis() - deltaMillis >= 5000) {
       deltaMillis = millis();
       display.clear();
     }
   }
   
   speed = gps.f_speed_kmph ();
    if (dbugActive == true) {
      Serial.print (speed, 2);
      Serial.print ("km/h");
    }
   
   display.print (speed, 2);
   dbugTime(10000);
   
    if (millis() - deltaMillis >= 5000) {
      deltaMillis = millis();
      display.clear();
    }
  }//end localization
 }//end millis

/*Dbug
####################################################################*/

 #ifdef dbugActive == true
  if (millis() - 0 >= 1000) {
   Serial.print (' ');
   Serial.print ("Ateção!!! função dbug iniciada!");
   Serial.print ("ajustando sistema para modo soneca de 30m...");
   Serial.print ("resultados finais das variaveis");
   Serial.print (' ')
   //strings
   Serial.print ("String:");
   Serial.print ("txt = ");
   Serial.println (txt);
   Serial.print (' ');
   //inteiras
   Serial.print ("inteiras:");
   Serial.print ("desprezivel = ");
   Serial.println (desprezivel);
   Serial.print ("memory: = ");
   Serial.println (memory);
   Serial.print ("x1 = ");
   Serial.println (x1);
   Serial.print ("x2 = ");
   Serial.println (x2);
   Serial.print ("x3 = ");
   Serial.println (x3);
   Serial.print ("x4 = ");
   Serial.println (x4);
   Serial.print ("x5 = ");
   Serial.println (x5);
   Serial.print (' ');
   //unsigned
   Serial.print ("unsigned:");
   Serial.print (' ');
   //long
   Serial.print ("long");
   Serial.print (' ');
   //float
   Serial.print ("float:");
   Serial.print ("readDht = ");
   Serial.println (readDht);
   Serial.print (' ');
   //byte
   Serial.print ("byte:");
   Serial.print ("up = ");
   Serial.println (up);
   Serial.print ("down = ");
   Serial.println (down);
   Serial.print (' ');
   //char
   Serial.print ("char:");
   Serial.print ("digitar = ");
   Serial.println (digitar);
   Serial.print (' ');
   //boolean
   Serial.print ("boolean:");
   Serial.print ("recebido = ");
   Serial.println (recebido);
   Serial.print ("readData = ");
   Serial.println (readData);
   //type
   Serial.print ("type: 1 for true 0 for false");
  }
  dbugTime (60000*30);
 #endif
}//end void

/*funções 
####################################################################*/

void dbugTime (int Time) {
  if (dbugActive == true) {
    delay (Time);
    Serial.print(millis());
 }
  else {
    delay(0);
  }
}

//#1 so queria que alguem dissesse para eu apenas tentar