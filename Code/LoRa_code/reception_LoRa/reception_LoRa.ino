//*----------------------------------------------------------------------------
//*  Projet FlatSat 4A
//*----------------------------------------------------------------------------
//* File Name : Reception_LoRa.ino
//* Object :  Communication LoRa par la carte Heltech ESP32 / Reception en C++
//* Creation : Armand Guzzonato/Leo Spery  15/01/2022
//* Modification : Armand Guzzonato 19/02/2022
//*----------------------------------------------------------------------------
// Include Standard files
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

/* Global variables */
String val;

String Data[37]; // Variables to read Data

double Rssi;
double Rssi_prec;

String Temp_String = "";  // String pour stocker les donnees recus du microcontrolleur principal
String num_data = "";
String tram = "";
int num_tram;
int num_tram_tempo=1;

int i = 0;

/**************************************************************************************************************
     Format des donnees recues en LoRa :
      LoRaMessage[0] = @numTram_1/ String(var1) + "#" + String(var4) + "#" + String(var6) + "#" + String(var7);   //EPS
      LoRaMessage[1] = @numTram_2/TEMP#ALTITUDE#PRESSION
      LoRaMessage[2] = @numTram_3/TANGAGE#LACET#ROULIS    //EULER
      LoRaMessage[3] = @numTram_4/QUAR1#QUAR2#QUAR3#QUAR4    //QUATERION
      LoRaMessage[4] = @numTram_5/Vx#Vy#Vz    //V ANGULAIRE
      LoRaMessage[5] = @numTram_6/Ax#Ay#Az    //ACCELERATION
      LoRaMessage[6] = @numTram_7/Bx#By#Bz    //CHAMP MAGNETIQUE
      LoRaMessage[7] = @numData_8/Gx#Gy#Gz    //VECTEUR GRAVITE
      LoRaMessage[8] = @numTram_9/+X#-X#+Y#-Y#+Z    //LUMINANCE
      LoRaMessage[9] = @numTram_10/HORODOTAGE#LONGITUDE#LATITUDE#NBRSAT    //GNSS

     Format des donnees exportees sur l UART:
      num_data Vbat Icharge Tbat etat_bat TEMP ALT PRESSION TANGAGE LACET ROULIS QUAR1 QUAR2 QUAR3 QUAR4 Vx Vy Vz Ax Ay Az Bx By Bz Gx Gy Gz +X -X +Y -Y +Z LONG LAT NBRSAT RSSI

 *************************************************************************************************************/

//*--------------------------------------------------------------------------------
//* Function Name : setup()
//* Object : Initialisation du programme
//*--------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial);

  //Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  /*
  Serial.print("LoRa Spreading Factor : "); Serial.println(LoRa.getSpreadingFactor());
  Serial.print("LoRa Signal Bandwidth (Hz): "); Serial.println(LoRa.getSignalBandwidth());
  Serial.print("LoRa Signal Frequency (MHz): "); Serial.println(LoRa.getFrequency());
  Serial.print("LoRa Coding Rate (4/x): "); Serial.println(LoRa.getCodingRate4());
  Serial.print("LoRa Preamble Length : "); Serial.println(LoRa.getPreambleLength());
  */

}

//*--------------------------------------------------------------------------------
//* Function Name : loop()
//* Object : Boucle principale du programme
//*--------------------------------------------------------------------------------
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {

    // read packet
    while (LoRa.available()) {
      val = LoRa.readString();
      //Serial.println(val);
    }

    // print RSSI of packet
   Rssi=LoRa.packetRssi();

   LoRaReading();
  }
}

// Function :

//*--------------------------------------------------------------------------------
//* Function Name : LoRaReading
//* Object : Lit les donnees recu en LoRa et les enregistre dans le tableau Data
//* Input Parameters : /
//* Output Parameters : /
//*--------------------------------------------------------------------------------
void LoRaReading()
{
  int pos_slash = val.indexOf('/');
  int pos_underscore = val.indexOf('_');
  int pos_a = val.indexOf('@');  
  
  tram = val.substring(pos_a+1,pos_underscore);
  num_tram = tram.toInt();
  num_data = val.substring(pos_underscore+1,pos_slash);

  WriteData();
  
  Temp_String = val.substring(pos_slash+1,val.length());
  

// EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS EPS
  if (num_data == "1")      
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);

    // enregistrement des donnees
    Data[1] = Temp_String.substring(0, pos1);
    Data[2] = Temp_String.substring(pos1 + 1, pos2);
    Data[3] = Temp_String.substring(pos2 + 1, pos3);
    Data[4] = Temp_String.substring(pos3 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

// TEMPERATURE ALTITUDE PRESSION TEMPERATURE ALTITUDE PRESSION TEMPERATURE ALTITUDE PRESSION TEMPERATURE ALTITUDE PRESSION TEMPERATURE ALTITUDE PRESSION TEMPERATURE ALTITUDE PRESSION 
    if (num_data == "2")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    // enregistrement des donnees
    Data[5] = Temp_String.substring(0, pos1);
    Data[6] = Temp_String.substring(pos1 + 1, pos2);
    Data[7] = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

  
  // EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER EULER
  if (num_data == "3")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    // enregistrement des donnees
    Data[8] = Temp_String.substring(0, pos1);
    Data[9] = Temp_String.substring(pos1 + 1, pos2);
    Data[10] = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

// Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion Quaterion
  if (num_data == "4")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);

    // enregistrement des donnees
    Data[11] = Temp_String.substring(0, pos1);
    Data[12] = Temp_String.substring(pos1 + 1, pos2);
    Data[13] = Temp_String.substring(pos2 + 1, pos3);
    Data[14] = Temp_String.substring(pos3 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

  // Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire Vitesse Angulaire
  if (num_data == "5")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    // enregistrement des donnees
    Data[15] = Temp_String.substring(0, pos1);
    Data[16] = Temp_String.substring(pos1 + 1, pos2);
    Data[17] = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

// Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration Acceleration
  if (num_data == "6")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    // enregistrement des donnees
    Data[18] = Temp_String.substring(0, pos1);
    Data[19] = Temp_String.substring(pos1 + 1, pos2);
    Data[20] = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

// Champs Magnetique Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique  Champs Magnetique
  if (num_data == "7")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    // enregistrement des donnees
    Data[21] = Temp_String.substring(0, pos1);
    Data[22] = Temp_String.substring(pos1 + 1, pos2);
    Data[23] = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

// Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite Vecteur Gravite
  if (num_data == "8")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);

    // enregistrement des donnees
    Data[24] = Temp_String.substring(0, pos1);
    Data[25] = Temp_String.substring(pos1 + 1, pos2);
    Data[26] = Temp_String.substring(pos2 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

// Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance Luminance
  if (num_data == "9")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);
    int pos4 = Temp_String.indexOf('#', pos3 + 1);

    Data[27] = Temp_String.substring(0, pos1);
    Data[28] = Temp_String.substring(pos1 + 1, pos2);
    Data[29] = Temp_String.substring(pos2 + 1, pos3);
    Data[30] = Temp_String.substring(pos3 + 1, pos4);
    Data[31] = Temp_String.substring(pos4 + 1, Temp_String.length());

    Temp_String = "";   // reset de la variable "lecture de donnees"
  }

// GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS GNSS
  if (num_data == "10")
  {
    int pos1 = Temp_String.indexOf('#');
    int pos2 = Temp_String.indexOf('#', pos1 + 1);
    int pos3 = Temp_String.indexOf('#', pos2 + 1);

    //Data[32] = Temp_String.substring(0, pos1);
    Data[32] = Temp_String.substring(pos1 + 1, pos2);
    Data[33] = Temp_String.substring(pos2 + 1, pos3);
    Data[34] = Temp_String.substring(pos3 + 1, Temp_String.length());

    int val;

    val=Data[32].indexOf('N');
    Data[32].remove(val);
    
    val=Data[33].indexOf('E');
    Data[33].remove(val);

    Temp_String = "";   // reset de la variable "lecture de donnees"
  } 
}

//*--------------------------------------------------------------------------------
//* Function Name : WriteData
//* Object : Ecrit les donnees  du tableau Data sur le Bus Serial
//* Input Parameters : /
//* Output Parameters : /
//*--------------------------------------------------------------------------------
void WriteData()
{
  if(num_tram != num_tram_tempo)
  {
    Data[0]=String(num_tram_tempo);

    i=0;
    for(i=0;i<=35;i++)
    {
      Serial.print(Data[i]);
      Serial.print(" ");
      Data[i]="0";
    }

    Serial.println("");
    num_tram_tempo = num_tram;
  }
  else
  {
    Data[36]=String(Rssi); 
  }
}
