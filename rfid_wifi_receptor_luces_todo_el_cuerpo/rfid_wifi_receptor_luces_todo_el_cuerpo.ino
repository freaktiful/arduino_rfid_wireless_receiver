/*
---- Receiver Code ----
Mert Arduino Tutorial & Projects (YouTube)
Please Subscribe for Support
*/

#include <Adafruit_NeoPixel.h>

#include <SPI.h>      //the communication interface with the modem
#include "RF24.h"     //the library which helps us to control the radio modem
#include <nRF24L01.h>

RF24 radio(5,10);     /*This o--bject represents a modem connected to the Arduino. 
                      Arguments 5 and 10 are a digital pin numbers to which signals 
                      CE and CSN are connected.*/

const uint64_t pipe = 0xE8E8F0F0E1LL; //the address of the modem,that will receive data from the Arduino.

byte RFID_Data[4]; 

int boton = 3;
long tiempo_anterior_boton;
long diferencia_boton;

long tiempo_anterior =0;
long diferencia;

#define PIN_PECHO 6
#define NUMPIXELS_PECHO 23

#define PIN_BRAZO1 7
#define NUMPIXELS_BRAZO1 1

#define PIN_BRAZO2 8
#define NUMPIXELS_BRAZO2 1

#define PIN_TORSOABDOMEN 9
#define NUMPIXELS_TORSOABDOMEN 18

#define PIN_PIERNAS 4
#define NUMPIXELS_PIERNAS 2

Adafruit_NeoPixel pixelsPecho = Adafruit_NeoPixel(NUMPIXELS_PECHO, PIN_PECHO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsBrazo1 = Adafruit_NeoPixel(NUMPIXELS_BRAZO1, PIN_BRAZO1, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel pixelsBrazo2 = Adafruit_NeoPixel(NUMPIXELS_BRAZO2, PIN_BRAZO2, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel pixelsTorsoAbdomen = Adafruit_NeoPixel(NUMPIXELS_TORSOABDOMEN, PIN_TORSOABDOMEN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel pixelsPiernas = Adafruit_NeoPixel(NUMPIXELS_PIERNAS, PIN_PIERNAS, NEO_RGBW + NEO_KHZ800);

bool encendido = false;
int cont = 0;

void setup(){
  Serial.begin(9600);

  pixelsPecho.begin(); // This initializes the NeoPixel library.
  pixelsPecho.show(); // initialize all pixels to "off"

  pixelsBrazo1.begin();
  pixelsBrazo1.show();

  pixelsBrazo2.begin();
  pixelsBrazo2.show();

  pixelsTorsoAbdomen.begin();
  pixelsTorsoAbdomen.show();

  pixelsPiernas.begin();
  pixelsPiernas.show();

  pinMode(boton, INPUT);
  attachInterrupt(digitalPinToInterrupt(boton), pulsacion, FALLING);
  
  radio.begin();                    //it activates the modem.
  radio.openReadingPipe(1, pipe);   //determines the address of our modem which receive data.
  radio.startListening();           //enable receiving data via modem
  
}

void loop(){
  if(radio.available()){            //checks whether any data have arrived at the address of the modem
    
    radio.read(&RFID_Data, sizeof(RFID_Data));
    char scannedRFID[10];
    sprintf(scannedRFID, "%02X%02X%02X%02X", RFID_Data[0], RFID_Data[1], RFID_Data[2], RFID_Data[3]);
    Serial.println(scannedRFID);
    cont++;
    if(cont>5){
    if(scannedRFID[0] == '8' && scannedRFID[1] == 'B' && scannedRFID[2] == 'B' &&
         scannedRFID[3] =='1' && scannedRFID[4] =='B' && scannedRFID[5] =='3' &&
         scannedRFID[6] =='1' && scannedRFID[7] =='B'){

  /*    if(scannedRFID[0] == 'D' && scannedRFID[1] == '4' && scannedRFID[2] == '9' &&
         scannedRFID[3] =='2' && scannedRFID[4] =='9' && scannedRFID[5] =='9' &&
         scannedRFID[6] =='1' && scannedRFID[7] =='3'){*/
    
      diferencia = millis() - tiempo_anterior;
      tiempo_anterior = millis();
      // Serial.println(diferencia);
    
      if(diferencia > 7000){
        cambiaEstadoLED();
        cont = 0;
      }
    }  
    } 
  } 
 }

 void pulsacion(){
  diferencia_boton = millis() - tiempo_anterior_boton;
  tiempo_anterior_boton = millis();
  //Serial.println(diferencia);

  if(diferencia_boton > 500){
    cambiaEstadoLED();
  }
}

  void cambiaEstadoLED(){
    if(!encendido){
      encender();
      Serial.println("encendido");
    }
    else{
      apagar();
      Serial.println("apagado");
    } 
    encendido = !encendido;
  }

void encender(){
  uint16_t i, j;

  for (j = 0; j < 150 ; j++) {
    for(int i=0;i<NUMPIXELS_PECHO;i++){
      pixelsPecho.setPixelColor(i, pixelsPecho.Color(23,161,165));
    }
    pixelsPecho.setBrightness(j);
    pixelsPecho.show();

    for(int i=0;i<NUMPIXELS_BRAZO1;i++){
      pixelsBrazo1.setPixelColor(i, pixelsBrazo1.Color(161,23,190, 50));
    }
    pixelsBrazo1.setBrightness(j);
    pixelsBrazo1.show();

    for(int i=0;i<NUMPIXELS_BRAZO2;i++){
      pixelsBrazo2.setPixelColor(i, pixelsBrazo2.Color(161,23,190, 50));
    }
    pixelsBrazo2.setBrightness(j);
    pixelsBrazo2.show();

    for(int i=0;i<NUMPIXELS_TORSOABDOMEN;i++){
      pixelsTorsoAbdomen.setPixelColor(i, pixelsTorsoAbdomen.Color(161,23,190, 50));
    }
    pixelsTorsoAbdomen.setBrightness(j);
    pixelsTorsoAbdomen.show();

    for(int i=0;i<NUMPIXELS_PIERNAS;i++){
      pixelsPiernas.setPixelColor(i, pixelsPiernas.Color(161,23,190, 50));
    }
    pixelsPiernas.setBrightness(j);
    pixelsPiernas.show();
    
    delay(20);
  }
}

void apagar(){
  uint16_t i, j;

  for (j = 150; j > 0; j--) {
    for(int i=0;i<NUMPIXELS_PECHO;i++){
      pixelsPecho.setPixelColor(i, pixelsPecho.Color(23,161,165));
    }
    pixelsPecho.setBrightness(j);
    pixelsPecho.show();

    for(int i=0;i<NUMPIXELS_BRAZO1;i++){
      pixelsBrazo1.setPixelColor(i, pixelsBrazo1.Color(161,23,190, 50));
    }
    pixelsBrazo1.setBrightness(j);
    pixelsBrazo1.show();

    for(int i=0;i<NUMPIXELS_BRAZO2;i++){
      pixelsBrazo2.setPixelColor(i, pixelsBrazo2.Color(161,23,190, 50));
    }
    pixelsBrazo2.setBrightness(j);
    pixelsBrazo2.show();

    for(int i=0;i<NUMPIXELS_TORSOABDOMEN;i++){
      pixelsTorsoAbdomen.setPixelColor(i, pixelsTorsoAbdomen.Color(161,23,190, 50));
    }
    pixelsTorsoAbdomen.setBrightness(j);
    pixelsTorsoAbdomen.show();

    for(int i=0;i<NUMPIXELS_PIERNAS;i++){
      pixelsPiernas.setPixelColor(i, pixelsPiernas.Color(161,23,190, 50));
    }
    pixelsPiernas.setBrightness(j);
    pixelsPiernas.show();
    
    delay(15);
  }

}
