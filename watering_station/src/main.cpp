#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"
#include "esp_flash.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

unsigned long lastWifiRetry = 0;
unsigned long lastMqttRetry = 0;
const unsigned long RETRY_INTERVAL = 5000;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  Serial.println(message);

  // Echo the message back to test/echo_response
  mqttClient.publish("test/echo_response", message);
  Serial.println("Echoed back to test/echo_response");
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.print("Attempting MQTT connection...");
  if (mqttClient.connect("ESP32WateringStation")) {
    Serial.println("connected");
    mqttClient.subscribe("test/echo");
    Serial.println("Subscribed to test/echo");
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 5 seconds");
  }
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println("WiFi connected to AP");
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("WiFi connected and got IP");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("--- ESP32 Diagnostic Info ---");

  uint32_t realSize = 0;
  esp_flash_get_size(NULL, &realSize);
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();
  uint32_t realId = 0;
  esp_flash_read_id(NULL, &realId);
  Serial.printf("Flash real id: %08X\n", realId);
  Serial.printf("Flash real size: %u bytes\n\n", realSize);
  Serial.printf("Flash ide size: %u bytes\n", ideSize);
  Serial.printf("Flash ide speed: %u Hz\n", ESP.getFlashChipSpeed());
  Serial.printf("Flash ide mode: %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "Unknown"));
  Serial.println("-----------------------------");

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(callback);

  WiFi.onEvent(WiFiEvent);
  connectToWifi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - lastWifiRetry > RETRY_INTERVAL) {
      Serial.println("WiFi not connected.");
      // WiFi.begin is called in setup and ESP32 handles reconnection automatically
      // But we can call it again if it stays disconnected for long
      lastWifiRetry = millis();
    }
  } else {
    if (!mqttClient.connected()) {
      if (millis() - lastMqttRetry > RETRY_INTERVAL) {
        connectToMqtt();
        lastMqttRetry = millis();
      }
    } else {
      mqttClient.loop();
    }
  }
}
