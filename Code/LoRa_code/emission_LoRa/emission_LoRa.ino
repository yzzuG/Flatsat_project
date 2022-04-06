//*----------------------------------------------------------------------------
//*  Projet FlatSat 4A
//*----------------------------------------------------------------------------
//* File Name : emission_LoRa.ino
//* Object :  Communication LoRa par la carte Heltech ESP32 / Emission en C++
//* Creation : Armand Guzzonato/Leo Spery  15/01/2022
//* Modification : Armand Guzzonato 02/04/2022
//*----------------------------------------------------------------------------
// Include Standard files
#include <Arduino.h>
#include <ESP32Ping.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <math.h>
/* Global constants */
//define the pins used by the LORA transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define LORA_LED 2  // led temoins on/off transmission lora
#define LORA_LED_SEND 25  // led temoins when data is transmitted
#define BAND 868E6 //868E6 Hz for Europe
/* Global variables */
//define variable for measurment
String var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15, var16, var17, var18, var19, var20, var21, var22, var23, var24, var25;
String var26, var27, var28, var29, var30, var31, var32, var33, var34, var35, var36, var37, var38, var39, var40, var41;
float var21B, var22B, var23B, var24B, var25B, var26B, var27B, var28B, var29B, var30B, var31B, var32B;
int i = 0;
//Lora
// variable used for LORA Tx
int readingID = 0;
int counter = 1;
String LoRaMessage[11] = "";  //Data storage
//GNSS
int GNSS_Status = 0;
//autre
int loraOk = 0;
char inChar ;
String Temp_String = "";         // String pour stocker les donnees recus du microcontrolleur principal
int aff_lora = 1;
int LoraStatus = 0; // Commande soft de la Liaison LORA, (0 transmission LORA off au demarrage, 1 On au demarrage)
// Delai
double delay_Lora = 200;  //ms
double delay_sent = 40;   //ms
double delay_sent_read = 200;   //ms
double delay_loop = 20000;   //ms

/*************************************************************************************************************
 
     Format des donnees recu en LoRa : 
      LoRaMessage[0] = @numData_1/ String(var1) + "#" + String(var4) + "#" + String(var6) + "#" + String(var7);   //EPS
      LoRaMessage[1] = @numData_2/TEMP#ALTITUDE#PRESSION
      LoRaMessage[2] = @numData_3/TANGAGE#LACET#ROULIS    //EULER
      LoRaMessage[3] = @numData_4/QUAR1#QUAR2#QUAR3#QUAR4    //QUATERION
      LoRaMessage[4] = @numData_5/Vx#Vy#Vz    //V ANGULAIRE
      LoRaMessage[5] = @numData_6/Ax#Ay#Az    //ACCELERATION
      LoRaMessage[6] = @numData_7/Bx#By#Bz    //CHAMP MAGNETIQUE
      LoRaMessage[7] = @numData_8/Gx#Gy#Gz    //VECTEUR GRAVITE
      LoRaMessage[8] = @numData_9/+X#-X#+Y#-Y#+Z    //LUMINANCE
      LoRaMessage[9] = @numData_10/HORODOTAGE#LONGITUDE#LATITUDE#NBRSAT    //GNSS


 *************************************************************************************************************/


//**************************************************************************************************************

//*--------------------------------------------------------------------------------
//* Function Name : setup()
//* Object : Initialisation du programme
//*--------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(LORA_LED , OUTPUT);  // LED LIAISON LORA
  digitalWrite(LORA_LED, LOW); // LED CONFIGURATION LORA
  pinMode(LORA_LED_SEND,OUTPUT);  // LED SEND DATA LORA
  
  startLoRA();
  delay(200);
  
  if (loraOk == 1)
  {
    digitalWrite(LORA_LED, HIGH);  // LED LIAISON LORA
  }

  //LoRa parameters
  Serial.print("LoRa Spreading Factor : "); Serial.println(LoRa.getSpreadingFactor());
  Serial.print("LoRa Signal Bandwidth (Hz): "); Serial.println(LoRa.getSignalBandwidth());
  Serial.print("LoRa Signal Frequency (MHz): "); Serial.println(LoRa.getFrequency());  //
  Serial.print("LoRa Signal Power : (dBm) "); Serial.println(LoRa.getTxPower());
  Serial.print("LoRa Coding Rate (4/x): "); Serial.println(LoRa.getCodingRate4());
  Serial.print("LoRa Preamble Length : "); Serial.println(LoRa.getPreambleLength());

  delay(1000);
}

//*--------------------------------------------------------------------------------
//* Function Name : loop()
//* Object : Boucle principale du programme
//*--------------------------------------------------------------------------------
void loop() {

  for (i = 0; i < 13; i++)
  {
    if (aff_lora == 1)
    {
      serialSend();
    }
    
    delay(delay_sent_read);

    if (Serial1.available())
    {
      serialRead();
    }
  }

  LoRaMessage[0] = "@"+String(counter)+"_"+String(1) + "/" + String(var1) + "#" + String(var4) + "#" + String(var6) + "#" + String(var7);   //EPS
  LoRaMessage[1] = "@"+String(counter)+"_"+String(2) + "/" + String(var8) + "#" + String(var9) + "#" + String(var10) ;    //TEMP + ALTITUDE + PRESSION
  LoRaMessage[2] = "@"+String(counter)+"_"+String(3) + "/" + String(var11) + "#" + String(var12) + "#" + String(var13) ;    //EULER
  //LoRaMessage[3] = "@"+String(counter)+"_"+String(4) + "/" + String(var14) + "#" + String(var15) + "#" + String(var16) + "#" + String(var17)  ;    //QUATERION
  LoRaMessage[4] = "@"+String(counter)+"_"+String(5) + "/" + String(var18) + "#" + String(var19) + "#" + String(var20)  ;    //V ANGULAIRE
  LoRaMessage[5] = "@"+String(counter)+"_"+String(6) + "/" + String(var21) + "#" + String(var22) + "#" + String(var23)  ;    //ACCELERATION
  LoRaMessage[6] = "@"+String(counter)+"_"+String(7) + "/" + String(var24) + "#" + String(var25) + "#" + String(var26)  ;    //CHAMP MAGNETIQUE
  LoRaMessage[7] = "@"+String(counter)+"_"+String(8) + "/" + String(var30) + "#" + String(var31) + "#" + String(var32)  ;    //VECTEUR GRAVITE
  //LoRaMessage[8] = "@"+String(counter)+"_"+String(9) + "/" + String(var33) + "#" + String(var34) + "#" + String(var35) + "#" + String(var36) + "#" + String(var37)  ;    //LUMINANCE
  LoRaMessage[9] = "@"+String(counter)+"_"+String(10) + "/" + String(var38) + "#" + String(var39) + "#" + String(var40) + "#" + String(var41)  ;    //GNSS

  sendReadings();
 
  aff_lora = 1;
  counter ++;
  Serial.print("\n\n ########################################## \n\n");
  delay(delay_loop);
}


//Fonctions

//*--------------------------------------------------------------------------------
//* Function Name : startLoRa
//* Object : Initialise le module LoRa
//* Input Parameters : /
//* Output Parameters : /
//*--------------------------------------------------------------------------------
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

//*--------------------------------------------------------------------------------
//* Function Name : sendReadings
//* Object : Envoie les donnees en LoRa
//* Input Parameters : /
//* Output Parameters : /
//*--------------------------------------------------------------------------------
void sendReadings() {

  digitalWrite(SS, LOW); //<<<<<<<<<<<< selectionner SPI LORA
  digitalWrite(LORA_LED_SEND,HIGH);

  //DATA Number
  Serial.print("\n Envoi de la trame LORA N°: ");
  Serial.println(counter);

  //Send LoRa packet to receiver    1
  // EPS
  LoRa.beginPacket();
  LoRa.print(LoRaMessage[readingID]);
  LoRa.endPacket();
  //Affichage sur le moniteur Arduino
  Serial.print("Envoi partie des donnees N°: ");
  Serial.println(readingID + 1);
  Serial.println(LoRaMessage[readingID]);
  readingID++;
  delay(delay_Lora);

  //Send LoRa packet to receiver    2
  // TEMP/PRESSURE/ALTITUDE
  LoRa.beginPacket();
  LoRa.print(LoRaMessage[readingID]);
  LoRa.endPacket();
  Serial.print("Envoi partie des donnees N°: ");
  Serial.println(readingID + 1);
  Serial.println(LoRaMessage[readingID]);
  readingID++;
  delay(delay_Lora);

  //Send LoRa packet to receiver    3
  // EULER
  LoRa.beginPacket();
  LoRa.print(LoRaMessage[readingID]);
  LoRa.endPacket();
  Serial.print("Envoi partie des donnees N°: ");
  Serial.println(readingID + 1);
  Serial.println(LoRaMessage[readingID]);
  readingID++;
  delay(delay_Lora);

//  //Send LoRa packet to receiver    4
//  // QUATERION
//  LoRa.beginPacket();
//  LoRa.print(LoRaMessage[readingID]);
//  LoRa.endPacket();
//  Serial.print("Envoi partie des donnees N°: ");
//  Serial.println(readingID + 1);
//  Serial.println(LoRaMessage[readingID]);
  readingID++;
//  delay(delay_Lora);

  //Send LoRa packet to receiver    5
  // V ANGULAIRE
  LoRa.beginPacket();
  LoRa.print(LoRaMessage[readingID]);
  LoRa.endPacket();
  Serial.print("Envoi partie des donnees N°: ");
  Serial.println(readingID + 1);
  Serial.println(LoRaMessage[readingID]);
  readingID++;
  delay(delay_Lora);

  //Send LoRa packet to receiver    6
  // ACCELERATION
  LoRa.beginPacket();
  LoRa.print(LoRaMessage[readingID]);
  LoRa.endPacket();
  Serial.print("Envoi partie des donnees N°: ");
  Serial.println(readingID + 1);
  Serial.println(LoRaMessage[readingID]);
  readingID++;
  delay(delay_Lora);

  //Send LoRa packet to receiver    7
  // CHAMP MAGNETIQUE
  LoRa.beginPacket();
  LoRa.print(LoRaMessage[readingID]);
  LoRa.endPacket();
  Serial.print("Envoi partie des donnees N°: ");
  Serial.println(readingID + 1);
  Serial.println(LoRaMessage[readingID]);
  readingID++;
  delay(delay_Lora);

  //Send LoRa packet to receiver    8
  // GRAVITY
  LoRa.beginPacket();
  LoRa.print(LoRaMessage[readingID]);
  LoRa.endPacket();
  Serial.print("Envoi partie des donnees N°: ");
  Serial.println(readingID + 1);
  Serial.println(LoRaMessage[readingID]);
  readingID++;
  delay(delay_Lora);

//  //Send LoRa packet to receiver    9
//  // LUMINANCE
//  LoRa.beginPacket();
//  LoRa.print(LoRaMessage[readingID]);
//  LoRa.endPacket();
//  Serial.print("Envoi partie des donnees N°: ");
//  Serial.println(readingID + 1);
//  Serial.println(LoRaMessage[readingID]);
  readingID++;
//  delay(delay_Lora);

  //Send LoRa packet to receiver    10
  // GNSS
  if(GNSS_Status == 1){
    LoRa.beginPacket();
    LoRa.print(LoRaMessage[readingID]);
    LoRa.endPacket();
    Serial.print("Envoi partie des donnees N°: ");
    Serial.println(readingID + 1);
    Serial.println(LoRaMessage[readingID]);
    delay(delay_Lora);
  }

  
  digitalWrite(LORA_LED_SEND,LOW);
  
  readingID = 0;
}

//*--------------------------------------------------------------------------------
//* Function Name : serialSend
//* Object : envoie des lettres vers la carte OBC pour definir les donnees attendues sur le bus UART 1
//* Input Parameters : /
//* Output Parameters : /
//*--------------------------------------------------------------------------------
void serialSend() {

  if (LoraStatus == 1)
  {
    // EPS
    Serial1.print('B');   // envoi sur le bus UART 1
    Serial.print('B');    // affichage sur le moniteur Arduino
    delay(delay_sent);

    // TEMPERATURE
    Serial1.print('C');
    Serial.print('C');
    delay(delay_sent);
    
    // ALTITUDE
    Serial1.print('D');
    Serial.print('D');
    delay(delay_sent);

    // PRESSION
    Serial1.print('E');
    Serial.print('E');
    delay(delay_sent);

    // VECTEUR EULER
    Serial1.print('F');
    Serial.print('F');
    delay(delay_sent);

//    // QUATERNION
//    Serial1.print('G');
//    Serial.print('G');
//    delay(delay_sent);

    // Vitesse Angulaire
    Serial1.print('H');
    Serial.print('H');
    delay(delay_sent);

    // ACCELERATION
    Serial1.print('I');
    Serial.print('I');
    delay(delay_sent);

    // Champs Magnetique
    Serial1.print('J');
    Serial.print('J');
    delay(delay_sent);

    /*// ACCELERATION LINEAIRE
    Serial1.print('K');
    Serial.print('K');*/

    // Vecteur Gravite
    Serial1.print('L');
    Serial.print('L');
    delay(delay_sent);

    /*// LUMINANCE
    Serial1.print('M');
    Serial.print('M');*/

    // GNSS
    Serial1.print('N');
    Serial.print('N');
    delay(delay_sent);

    Serial1.print('Z');
    Serial.println('Z');

    aff_lora = 0;
  }
}

//*--------------------------------------------------------------------------------
//* Function Name : serialRead
//* Object : Lecture des donnees des capteurs sur le bus UART 2 
//* Input Parameters : /
//* Output Parameters : /
//*--------------------------------------------------------------------------------
void serialRead() {
  //Serial.println("\n... new HELTEC reception event ...\n");
  inChar = Serial1.read();    // lecture des lettres envoyees pour connaitre les valeurs a lire 

  //affichage sur le moniteur Arduino
  //Serial.print("1er Char recu : ");
  //Serial.println(inChar);

  // EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS
  if  (inChar == 'B') 
  {
    Temp_String = Serial1.readStringUntil('@'); // lecture des donnees
    //Serial.println(Temp_String);  // affichage sur le moniteur Arduino
    // traitement des donnees pour les trier et les enregistrer dans des variables 
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);
    int pos4 = Temp_String.indexOf('#', pos3 + 1);
    int pos5 = Temp_String.indexOf('#', pos4 + 1);
    int pos6 = Temp_String.indexOf('#', pos5 + 1);

    // enregistrement des donnees
    var1 = Temp_String.substring(0, pos1);
    var2 = Temp_String.substring(pos1 + 1, pos2);
    var3 = Temp_String.substring(pos2 + 1, pos3);
    var4 = Temp_String.substring(pos3 + 1, pos4);
    var5 = Temp_String.substring(pos4 + 1, pos5);
    var6 = Temp_String.substring(pos5 + 1, pos6);
    var7 = Temp_String.substring(pos6 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

  // TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE
  if  (inChar == 'C') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    var8 = Temp_String;

    Temp_String = "";
  }

  // ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE
  if  (inChar == 'D') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    var9 = Temp_String;

    Temp_String = "";
  }

  // PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION
  if  (inChar == 'E') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    var10 = Temp_String;

    Temp_String = "";
  }

  // EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER
  if  (inChar == 'F') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    var11 = Temp_String.substring(0, pos1);
    var12 = Temp_String.substring(pos1 + 1, pos2);
    var13 = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";
  }

  // Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion
  if  (inChar == 'G') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);

    var14 = Temp_String.substring(0, pos1);
    var15 = Temp_String.substring(pos1 + 1, pos2);
    var16 = Temp_String.substring(pos2 + 1, pos3);
    var17 = Temp_String.substring(pos3 + 1, Temp_String.length());

    Temp_String = "";
  }

  // Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire
  if  (inChar == 'H') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    var18 = Temp_String.substring(0, pos1);
    var19 = Temp_String.substring(pos1 + 1, pos2);
    var20 = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";
  }

  // Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration
  if  (inChar == 'I') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    var21 = Temp_String.substring(0, pos1);
    var22 = Temp_String.substring(pos1 + 1, pos2);
    var23 = Temp_String.substring(pos2 + 1, Temp_String.length());

    //conversion (mm/s^2-->m/s^2)
    var21B = (atof((var21).c_str())) / 100;
    var22B = (atof((var22).c_str())) / 100;
    var23B = (atof((var23).c_str())) / 100;

    var21 = String(var21B);
    var22 = String(var22B);
    var23 = String(var23B);

    Temp_String = "";
  }

  // Champs Magnetique Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique
  if  (inChar == 'J') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    var24 = Temp_String.substring(0, pos1);
    var25 = Temp_String.substring(pos1 + 1, pos2);
    var26 = Temp_String.substring(pos2 + 1, Temp_String.length());

    //conversion
    var24B = (atof((var24).c_str())) / 10;
    var25B = (atof((var25).c_str())) / 10;
    var26B = (atof((var26).c_str())) / 10;

    var24 = String(var24B);
    var25 = String(var25B);
    var26 = String(var26B);

    Temp_String = "";
  }

  // Acceleration Lineaire Acceleration Lineaire Acceleration Lineaire Acceleration Lineaire Acceleration Lineaire Acceleration Lineaire Acceleration Lineaire Acceleration Lineaire
  if  (inChar == 'K') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    var27 = Temp_String.substring(0, pos1);
    var28 = Temp_String.substring(pos1 + 1, pos2);
    var29 = Temp_String.substring(pos2 + 1, Temp_String.length());

    //conversion (mm/s^2-->m/s^2)
    var27B = (atof((var27).c_str())) / 100;
    var28B = (atof((var28).c_str())) / 100;
    var29B = (atof((var29).c_str())) / 100;

    var27 = String(var27B);
    var28 = String(var28B);
    var29 = String(var29B);

    Temp_String = "";
  }

  // Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite
  if  (inChar == 'L') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    var30 = Temp_String.substring(0, pos1);
    var31 = Temp_String.substring(pos1 + 1, pos2);
    var32 = Temp_String.substring(pos2 + 1, Temp_String.length());

    //conversion (mm/s^2-->m/s^2)
    var30B = (atof((var30).c_str())) / 100;
    var31B = (atof((var31).c_str())) / 100;
    var32B = (atof((var32).c_str())) / 100;

    var30 = String(var30B);
    var31 = String(var31B);
    var32 = String(var32B);

    Temp_String = "";
  }

  // Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance
  if  (inChar == 'M') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);
    int pos4 = Temp_String.indexOf('#', pos3 + 1);

    var33 = Temp_String.substring(0, pos1);
    var34 = Temp_String.substring(pos1 + 1, pos2);
    var35 = Temp_String.substring(pos2 + 1, pos3);
    var36 = Temp_String.substring(pos3 + 1, pos4);
    var37 = Temp_String.substring(pos4 + 1, Temp_String.length());

    Temp_String = "";
  }

  // GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS
  if  (inChar == 'N') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);

    var38 = Temp_String.substring(0, pos1);
    var39 = Temp_String.substring(pos1 + 1, pos2);
    var40 = Temp_String.substring(pos2 + 1, pos3);
    var41 = Temp_String.substring(pos3 + 1, Temp_String.length());

    GNSS_Status = 1;

    Temp_String = "";
  }

  if  (inChar == 'P') {  // dans le cas ou carte envoie No-data
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    var38 = "";
    var39 = "";
    var40 = "";
    var41 = "";

    GNSS_Status = 0;

    Temp_String = "";
  }

  // FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN FIN

  if  (inChar == 'Z')  // code pour indiquer la fin des parametres choisi, et envoyer les resultats vers la console avec l'event
  {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    Temp_String = "";
  }
}
