#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "FS.h"
#include "SPIFFS.h"
#include "utils.h"

#define led 2
#define DHTPIN 4
#define DHTTYPE DHT22
#define topico_pub "lab318/esp_py"

WiFiClient espClient;
PubSubClient MQTT(espClient);
DHT dht(DHTPIN, DHTTYPE);

static long long tempo=0;

const char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;

void setup() 
{  
    Serial.begin(9600);
    WIFIConnect(&espClient);
    MQTT.setServer(mqtt_broker, mqtt_port);   
    MQTT.setCallback(callback);
}

void loop() 
{
  static long long pooling = 0;
  
  if(!MQTT.connected()) MQTTConnect(&MQTT);
  if(WiFi.status() != WL_CONNECTED) WIFIConnect(&espClient);
  
  if(millis()>pooling+10000)
  {
    pooling = millis();
    publish_data(&MQTT, topico_pub, "teste\n");
  }

  MQTT.loop();
}