#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ThingSpeak.h> 

#include "secrets.h"

#define SOIL_MOISTURE_PIN 34
#define LDR_PIN 35        
#define DHT_PIN 17       
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

const int READ_DELAY = 30000;
unsigned long lastReadTime = 0;

unsigned long channelID = SECRET_CH_ID;
const char *mqttServer = "mqtt3.thingspeak.com";
const char *mqttUsername = SECRET_MQTT_USER;
const char *mqttPassword = SECRET_MQTT_PASS;

void connectToWiFi();
void reconnectMQTT();
void readAndPublishData();

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nPhytoNode - Booting...");

  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  dht.begin();

  connectToWiFi();

  mqttClient.setServer(mqttServer, 1883);
  ThingSpeak.begin(espClient);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  if (millis() - lastReadTime > READ_DELAY) {
    readAndPublishData();
    lastReadTime = millis();
  }

  delay(100);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi ");
  Serial.println(SECRET_SSID);

  WiFi.begin(SECRET_SSID, SECRET_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected! IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("PhytoNodeClient", mqttUsername, mqttPassword)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void readAndPublishData() {
  Serial.println("Reading sensor data...");

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = -1;
    humidity = -1;
  }

  int soilRaw = analogRead(SOIL_MOISTURE_PIN);
  int soilMoisturePercent = map(soilRaw, 1500, 3500, 100, 0);
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  int lightRaw = analogRead(LDR_PIN);
  int lightPercent = map(lightRaw, 500, 4095, 100, 0);
  lightPercent = constrain(lightPercent, 0, 100);

  Serial.printf("Temp: %.2f°C | Humidity: %.2f%% | ", temperature, humidity);
  Serial.printf("Soil: %d%% (Raw: %d) | ", soilMoisturePercent, soilRaw);
  Serial.printf("Light: %d%% (Raw: %d)\n", lightPercent, lightRaw);

  String payload = "field1=" + String(temperature);
  payload += "&field2=" + String(humidity);
  payload += "&field3=" + String(soilMoisturePercent);
  payload += "&field4=" + String(lightPercent);

  String topicString = "channels/" + String(channelID) + "/publish";
  
  char topicBuffer[50];
  char payloadBuffer[100];
  topicString.toCharArray(topicBuffer, 50);
  payload.toCharArray(payloadBuffer, 100);


  if (mqttClient.publish(topicBuffer, payloadBuffer)) {
    Serial.println("Data published to ThingSpeak!");
  } else {
    Serial.println("Data publish failed. Trying HTTP...");

    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.setField(3, soilMoisturePercent);
    ThingSpeak.setField(4, lightPercent);
    
    int httpCode = ThingSpeak.writeFields(channelID, SECRET_WRITE_APIKEY);
    if (httpCode == 200) {
      Serial.println("HTTP update successful.");
    } else {
      Serial.println("HTTP update failed. Error code: " + String(httpCode));
    }
  }
}

}}
