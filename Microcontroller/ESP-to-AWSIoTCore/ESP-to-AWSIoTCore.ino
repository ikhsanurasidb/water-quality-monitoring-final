#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net;
PubSubClient client(net);

float temperature;
int tds;
float ph;

void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  net.setCACert(AWS_CERT_CA, sizeof(AWS_CERT_CA));
  net.setCertificate(AWS_CERT_CRT, sizeof(AWS_CERT_CRT));
  net.setPrivateKey(AWS_CERT_PRIVATE, sizeof(AWS_CERT_PRIVATE));
  
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["tds"] = tds;

  char temperatureStr[6];
  dtostrf(temperature, 4, 2, temperatureStr);
  doc["suhu"] = String(temperatureStr);

  char phStr[6];
  dtostrf(ph, 4, 2, phStr);
  doc["ph"] = String(phStr);

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("incoming: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup() {
  Serial.begin(9600);
  connectAWS();
}

void loop() {
  temperature = random(20, 25);
  tds = random(300, 1000);
  ph = random(6, 9);

  Serial.print("Temp: ");
  Serial.println(temperature);
  Serial.print("TDS: ");
  Serial.println(tds);
  Serial.print("pH: ");
  Serial.println(ph);

  publishMessage();
  client.loop();
  delay(10000);
}
