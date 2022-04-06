/*************************************************************************************************************

    Projet FlatSat 4A  -  Armand Guzzonato / Leo Spery

    Communication LoRa par la carte Heltech ESP32 / Emission

 *************************************************************************************************************/

#include <Arduino.h>
#include <ESP32Ping.h>

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <math.h>


//**************************************************************************************************************


//define the pins used by the LORA transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define LORA_LED 2  // led temoins on/off transmission lora 
#define BAND 868E6 //868E6 for Europe

int i = 0;

//Lora
// variable used for LORA Tx
int readingID = 0;
int counter = 0;
String LoRaMessage[11] = "";
//autre
int loraOk = 0;
char inChar ;
String Temp_String = "";         // String pour stocker les donnees recus du microcontrolleur principal
int aff_lora = 1;

int LoraStatus = 0; // Commande soft de la Liaison LORA, (0 transmission LORA off au demarrage, 1 On au demarrage)

// Delai

double delay_loop = 5000;   //ms

//**************************************************************************************************************


//Fonctions

//Initialize LoRa module
void startLoRA() {
  pinMode(SS, OUTPUT);
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  digitalWrite(SS, LOW); //<<<<<<<<<<<< selectionner SPI LORA
  LoRa.setPins(SS, RST, DIO0);
  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(500);
  }
  if (counter == 10) {
    Serial.println("Starting LoRa failed!");
  }
  else {
    LoRa.setSyncWord(0x5F);

    //valeurs par defaut :
    LoRa.setTxPower(17);  // 17dBm
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5); // 4/5
    LoRa.setPreambleLength(8);
    LoRa.disableCrc();  // LoRa.enableCrc();

    Serial.println("LoRa Initialisation OK ...");
    Serial.println("(TxPower : 17dBm, SpreadingFactor : 7, SignalBandwidth : 125E3, CodingRate : 4/5, PreambleLength : 8, disableCrc) ");
    loraOk = 1;
    LoraStatus = 1;
  }
}


//**************************************************************************************************************


void setup() {
  Serial.begin(115200);

  pinMode(LORA_LED , OUTPUT);  // LED LIAISON LORA
  digitalWrite(LORA_LED, LOW); // LED CONFIGURATION LORA

  startLoRA();
  delay(200);
  
  if (loraOk == 1)
  {
    digitalWrite(LORA_LED, HIGH);  // LED LIAISON LORA
  }

  Serial.print("LoRa Spreading Factor : "); Serial.println(LoRa.getSpreadingFactor());
  Serial.print("LoRa Signal Bandwidth (Hz): "); Serial.println(LoRa.getSignalBandwidth());
  Serial.print("LoRa Signal Frequency (MHz): "); Serial.println(LoRa.getFrequency());  //
  Serial.print("LoRa Signal Power : (dBm) "); Serial.println(LoRa.getTxPower());
  Serial.print("LoRa Coding Rate (4/x): "); Serial.println(LoRa.getCodingRate4());
  Serial.print("LoRa Preamble Length : "); Serial.println(LoRa.getPreambleLength());

  pinMode(25,OUTPUT);

  delay(1000);
}

//**************************************************************************************************************


void loop() {
  
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW

  delay(delay_loop);
}
