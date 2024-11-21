#include <LoRa.h>
#include <WiFi.h>
#include <SPI.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Dongyang";          // WiFi name
const char* password = "mdy123456";      // WiFi password

// MQTT broker
const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);

// LoRa parameters
#define SCK 5 // GPIO5 SX127x's SCK
#define MISO 19 // GPIO19 SX127x's MISO
#define MOSI 27 // GPIO27 SX127x's MOSI
#define SS 18 // GPI018 SX127x's CS
#define RST 14 // GPI014 SX127x's RESET
#define DI0 26 // GPIO26 SX127x's IRQ(Interrupt Request)

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void setupMQTT() {
  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("LoRaSender")) {
      Serial.println("Connected to MQTT");
    } else {
      delay(1000);
    }
  }
}

void setupLoRa() {
  pinMode(DI0,INPUT);
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(869E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initialized");
}

void sendMQTTParameters() {
  const char* topic = "tp_popo/NIG";
  const char* message = "{\"sb\":125,\"sf\":8,\"freq\":869}";
  client.publish(topic, message);
  Serial.println("MQTT message sent: " + String(message));
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
  setupLoRa();
  sendMQTTParameters();
}

void loop() {
  if (!client.connected()) {
    setupMQTT();
  }
  client.loop();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }
    Serial.println("LoRa received: " + received);
  }
}
