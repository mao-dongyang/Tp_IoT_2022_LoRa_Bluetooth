#include <SPI.h>
#include <LoRa.h>
#define SCK 5 // GPIO5 SX127x's SCK
#define MISO 19 // GPIO19 SX127x's MISO
#define MOSI 27 // GPIO27 SX127x's MOSI
#define SS 18 // GPI018 SX127x's CS
#define RST 14 // GPI014 SX127x's RESET
#define DI0 26 // GPIO26 SX127x's IRQ(Interrupt Request)
#define sb 125E3
#define sf 8
#define freq 869E6

void setup() {
  Serial.begin(115200);
  while (!Serial);
  pinMode(DI0, INPUT);
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  
  Serial.println("LoRa Receiver");

  // Initialize LoRa，Frequency set as 869MHz
  if (!LoRa.begin(869E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(sf);
  LoRa.setSignalBandwidth(sb);
}

void loop() {
  // check if a packet has been received
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet: ");
    while (LoRa.available()) {
      String received = LoRa.readString();
      Serial.print(received);
    }

    // print RSSI of packet
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}