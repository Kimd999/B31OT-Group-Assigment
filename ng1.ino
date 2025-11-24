#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

// -------- LED -----------
#define LED_PIN 5
#define NUMPIXELS 1
Adafruit_NeoPixel led(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// -------- WiFi ----------
const char* ssid     = "Test";        
const char* password = "123456789";   

// -------- HiveMQ Cloud TLS ----------
const char* mqttServer = "2bcacdc6c78e4b73a575478294c5953b.s1.eu.hivemq.cloud";
const int mqttPort     = 8883;  // TLS port
const char* mqttUser   = "Ananthapadmanabhan_Manoj";
const char* mqttPass   = "Padmanabham@23";

// -------- Topics ----------
const char* dataSubTopic = "greenhouse/+/data";
const char* cmdTopic     = "greenhouse/gateway/cmd";
const char* statusTopic  = "greenhouse/gateway/status";

// -------- TLS Client --------
WiFiClientSecure secureClient;
PubSubClient mqtt(secureClient);

// -------- LED helper --------
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  led.setPixelColor(0, led.Color(r, g, b));
  led.show();
}

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to HiveMQ Cloud... ");

    String clientId = "ESP32_Gateway_";
    clientId += WiFi.macAddress();
    clientId.replace(":", "");

    if (mqtt.connect(clientId.c_str(), mqttUser, mqttPass)) {
      Serial.println("CONNECTED ✔");
      mqtt.subscribe(dataSubTopic);
      mqtt.subscribe(cmdTopic);
      mqtt.publish(statusTopic, "gateway-online");
    } else {
      Serial.print("FAILED → state ");
      Serial.println(mqtt.state());
      delay(3000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("---- MQTT ----");
  Serial.println(topic);
  Serial.println(message);

  if (String(topic) == cmdTopic) {
    StaticJsonDocument<200> doc;
    if (!deserializeJson(doc, message)) {
      int r = doc["red"] | 0;
      int g = doc["green"] | 0;
      int b = doc["blue"] | 0;
      setColor(r, g, b);
    }
  }
}

void setup() {
  Serial.begin(115200);
  led.begin();
  led.clear();
  led.show();
  setColor(0, 0, 100);

  connectWiFi();

  secureClient.setInsecure();   // IMPORTANT (no certificate needed)

  mqtt.setServer(mqttServer, mqttPort);
  mqtt.setCallback(mqttCallback);
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}
