//*----------------------------------------------------------------------------
//*  Projet FlatSat 4A
//*----------------------------------------------------------------------------
//* File Name : Com_UART_drone.ino
//* Object :  Communication UART par la carte Heltech ESP32 vers drone / Emission en C++
//* Creation : Armand Guzzonato/Leo Spery  15/01/2022
//* Modification : Armand Guzzonato 02/04/2022
//*----------------------------------------------------------------------------
// Include Standard files
#include <Arduino.h>
#include <ESP32Ping.h>
#include <SPI.h>
#include <Wire.h>
#include <math.h>

/* Global constants */
#define LORA_DATA_SEND 25  // led temoins when data is transmitted

/* Global variables */
//define variable for measurment
String var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15, var16, var17, var18, var19, var20, var21, var22, var23, var24, var25;
String var26, var27, var28, var29, var30, var31, var32, var33, var34, var35, var36, var37, var38, var39, var40, var41, var42, var43, var44, var45, var46, var47, var48, var49, var50;
float var21B, var22B, var23B, var24B, var25B, var26B, var27B, var28B, var29B, var30B, var31B, var32B;
String Data[37]; // Variables to read Data
int i = 0; //incrémentation
// variable used for UART Tx
int readingID = 0;
int counter = 1;
//GNSS
int GNSS_Status = 0;
//other
char inChar ;
String Temp_String = "";         // String pour stocker les donnees recus du microcontrolleur principal
int aff_UART = 1;
// Delay
double delay_sent = 40;   //ms
double delay_sent_read = 100;   //ms
double delay_loop = 2000;   //ms

/*************************************************************************************************************

     Format des donnees exportees sur l UART:
      num_data var1 var4 var6 var7 TEMP ALT PRESSION TANGAGE LACET ROULIS QUAR1 QUAR2 QUAR3 QUAR4 Vx Vy Vz Ax Ay Az Bx By Bz Gx Gy Gz +X -X +Y -Y +Z LONG LAT NBRSAT

 *************************************************************************************************************/

//**************************************************************************************************************

//*--------------------------------------------------------------------------------
//* Function Name : setup()
//* Object : Initialisation du programme
//*--------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(LORA_DATA_SEND, OUTPUT); // LED SEND DATA LORA

  // initialisation du tableau de valeurs Data à 0
  for (i = 0; i <= 35; i++)
  {
    Data[i] = "0";
  }

  delay(200);
}

//*--------------------------------------------------------------------------------
//* Function Name : loop()
//* Object : Boucle principale du programme
//*--------------------------------------------------------------------------------
void loop() {

  
  for (i = 0; i < 14; i++)
  {
    if (aff_UART == 1)
    {
      serialSend();
      delay(delay_sent_read);
    }

    
    digitalWrite(LORA_DATA_SEND,HIGH);

    if (Serial1.available())
    {
      serialRead();
    }
    //Serial.println("boucle");
    delay(150);
  }

  Data[0] = counter;

  //Envoie des donnees sur le bus UART0
  for (i = 0; i <= 34; i++)
  {
    Serial.print(Data[i]);
    Serial.print(" ");
    Data[i] = "0";  // reset des valeurs
  }
  Serial.println("");

  aff_UART = 1;
  counter ++;
  digitalWrite(LORA_DATA_SEND,LOW);
  //Serial.println("          FIN DE BOUCLE         ");
  delay(delay_loop);
}

//Fonctions
//*--------------------------------------------------------------------------------
//* Function Name : serialSend
//* Object : envoie des lettres vers la carte OBC pour definir les donnees attendues sur le bus UART 1
//* Input Parameters : /
//* Output Parameters : /
//*--------------------------------------------------------------------------------
void serialSend() {

  // EPS
  Serial1.print('B');   // envoi sur le bus UART 1
  //Serial.print('B');    // affichage sur le moniteur Arduino
  delay(delay_sent);

  // TEMPERATURE
  Serial1.print('C');
  //Serial.print('C');
  delay(delay_sent);

  // ALTITUDE
  Serial1.print('D');
  //Serial.print('D');
  delay(delay_sent);

  // PRESSION
  Serial1.print('E');
  //Serial.print('E');
  delay(delay_sent);

  // VECTEUR EULER
  Serial1.print('F');
  //Serial.print('F');
  delay(delay_sent);

  //    // QUATERNION
  //    Serial1.print('G');
  //    //Serial.print('G');
  //    delay(delay_sent);

  // Vitesse Angulaire
  Serial1.print('H');
  //Serial.print('H');
  delay(delay_sent);

  // ACCELERATION
  Serial1.print('I');
  //Serial.print('I');
  delay(delay_sent);

  // Champs Magnetique
  Serial1.print('J');
  //Serial.print('J');
  delay(delay_sent);

  /*// ACCELERATION LINEAIRE
    Serial1.print('K');
    //Serial.print('K');*/

  // Vecteur Gravite
  Serial1.print('L');
  //Serial.print('L');
  delay(delay_sent);

  /*// LUMINANCE
    Serial1.print('M');
    //Serial.print('M');*/

  // GNSS
  //Serial1.print('N');
  //Serial.print('N');
  delay(delay_sent);

  Serial1.print('Z');
  //Serial.println('Z');

  aff_UART = 0;
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
    Data[1] = Temp_String.substring(0, pos1);
    var2 = Temp_String.substring(pos1 + 1, pos2);
    var3 = Temp_String.substring(pos2 + 1, pos3);
    Data[2] = Temp_String.substring(pos3 + 1, pos4);
    Data[3] = Temp_String.substring(pos4 + 1, pos5);
    var6 = Temp_String.substring(pos5 + 1, pos6);
    Data[4] = Temp_String.substring(pos6 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

  // TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE TEMPERATURE
  if  (inChar == 'C') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    Data[5] = Temp_String;

    Temp_String = "";
  }

  // ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE ALTITUDE
  if  (inChar == 'D') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    Data[6] = Temp_String;

    Temp_String = "";
  }

  // PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION PRESSION
  if  (inChar == 'E') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    Data[7] = Temp_String;

    Temp_String = "";
  }

  // EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER
  if  (inChar == 'F') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    Data[8] = Temp_String.substring(0, pos1);
    Data[9] = Temp_String.substring(pos1 + 1, pos2);
    Data[10] = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";
  }

  // Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion
  if  (inChar == 'G') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);

    Data[11] = Temp_String.substring(0, pos1);
    Data[12] = Temp_String.substring(pos1 + 1, pos2);
    Data[13] = Temp_String.substring(pos2 + 1, pos3);
    Data[14] = Temp_String.substring(pos3 + 1, Temp_String.length());

    Temp_String = "";
  }

  // Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire
  if  (inChar == 'H') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    Data[15] = Temp_String.substring(0, pos1);
    Data[16] = Temp_String.substring(pos1 + 1, pos2);
    Data[17] = Temp_String.substring(pos2 + 1, Temp_String.length());

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

    var21B = (atof((var21).c_str())) / 100;
    var22B = (atof((var22).c_str())) / 100;
    var23B = (atof((var23).c_str())) / 100;

    Data[18] = String(var21B);
    Data[19] = String(var22B);
    Data[20] = String(var23B);

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

    var24B = (atof((var24).c_str())) / 10;
    var25B = (atof((var25).c_str())) / 10;
    var26B = (atof((var26).c_str())) / 10;

    Data[21] = String(var24B);
    Data[22] = String(var25B);
    Data[23] = String(var26B);

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

    var30B = (atof((var30).c_str())) / 100;
    var31B = (atof((var31).c_str())) / 100;
    var32B = (atof((var32).c_str())) / 100;

    Data[24] = String(var30B);
    Data[25] = String(var31B);
    Data[26] = String(var32B);

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

    Data[27] = Temp_String.substring(0, pos1);
    Data[28] = Temp_String.substring(pos1 + 1, pos2);
    Data[29] = Temp_String.substring(pos2 + 1, pos3);
    Data[30] = Temp_String.substring(pos3 + 1, pos4);
    Data[31] = Temp_String.substring(pos4 + 1, Temp_String.length());

    Temp_String = "";
  }

  // GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS
  if  (inChar == 'N') {
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);

    //Data[32] = Temp_String.substring(0, pos1); //horodotage --> probleme de format dans matlab donc pas transmis
    Data[32] = Temp_String.substring(pos1 + 1, pos2);
    Data[33] = Temp_String.substring(pos2 + 1, pos3);
    Data[34] = Temp_String.substring(pos3 + 1, Temp_String.length());

    // suppresion des lettres N et E car probleme de lecture dans matlab
    int val;

    val=Data[32].indexOf('N');
    Data[32].remove(val);
    
    val=Data[33].indexOf('E');
    Data[33].remove(val);
    
    GNSS_Status = 0;

    Temp_String = "";
  }

  if  (inChar == 'P') {  // dans le cas ou carte envoie No-data
    Temp_String = Serial1.readStringUntil('@');
    //Serial.println(Temp_String);

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
