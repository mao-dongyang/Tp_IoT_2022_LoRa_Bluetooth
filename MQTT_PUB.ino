#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Dongyang";          // WiFi name
const char* password = "mdy123456";      // WiFi passzord
const char* mqtt_server = "test.mosquitto.org";  // MQTT Server

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // WiFi Connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// connect to MQTT Server
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // connect to MQTT Broker
    if (client.connect("NIG")) {
      Serial.println("connected");
      // publish
      client.publish("tp_popo/alban", "Hello from ESP32");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);  // set MQTT server and port
}

void loop() {
  // reconnectMQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //send Message each 5 seconds
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    String msg = "Hello World at " + String(now);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("tp_popo/alban", msg.c_str());
  }
}