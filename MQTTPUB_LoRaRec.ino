#include <LoRa.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// MQTT broker
const char* mqtt_server = "test.mosquitto.org";
WiFiClient espClient;
PubSubClient client(espClient);

// LoRa parameters
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26

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
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initialized");
}

void sendMQTTParameters() {
  const char* topic = "lora/parameters";
  const char* message = "{\"sb\":125,\"sf\":7,\"freq\":868}";
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
