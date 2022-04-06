#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

String val;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.print("LoRa Spreading Factor : "); Serial.println(LoRa.getSpreadingFactor());
  Serial.print("LoRa Signal Bandwidth (Hz): "); Serial.println(LoRa.getSignalBandwidth());
  Serial.print("LoRa Signal Frequency (MHz): "); Serial.println(LoRa.getFrequency());
  Serial.print("LoRa Coding Rate (4/x): "); Serial.println(LoRa.getCodingRate4());
  Serial.print("LoRa Preamble Length : "); Serial.println(LoRa.getPreambleLength());

}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      val = LoRa.readString();
      Serial.print(val);
    }

    // print RSSI of packet
    Serial.print("'    with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
