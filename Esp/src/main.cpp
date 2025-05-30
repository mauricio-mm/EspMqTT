#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "FS.h"
#include "SPIFFS.h"

#define ID_MQTT "esp_iot"
#define topico_pub_temp "lab318/temp"
#define topico_pub_umid "lab318/umid"

#define led 2 //corrente do led 410mA
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

static long long tempo=0;

const char* ssid = "POCOX6Pro";    // ALTERE O NOME DA REDE
const char* password = "esppucrs";  // INFORME A SENHA DA REDE

const char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient MQTT(espClient);

void MQTTConnect();
void publish_data();
void callback(char *topic, byte *payload, unsigned int length);
void WIFIConnect();

void setup() 
{  
    Serial.begin(9600);
    WIFIConnect();
    MQTT.setServer(mqtt_broker, mqtt_port);    
}

void loop() 
{
  static long long pooling = 0;
  
  if(!MQTT.connected()) MQTTConnect();
  if(WiFi.status() != WL_CONNECTED) WIFIConnect();
  
  if(millis()>pooling+10000)
  {
    pooling = millis();
    publish_data();
  }
  MQTT.loop();
}

void MQTTConnect()
{
  while(!MQTT.connected()) 
  {
    if (MQTT.connect(ID_MQTT))
    {
      Serial.println("Conectado ao Broker!");
    } 
    else 
    {
      Serial.print("Falha na conexão. O status é: ");
      Serial.print(MQTT.state());
      delay(2000);
    }
  }
}

void publish_data()
{
  float umid = 52.6;  
  float temp = 23.5; 

  MQTT.publish(topico_pub_temp,String(temp).c_str());
  MQTT.publish(topico_pub_umid,String(umid).c_str());
}

void callback(char *topic, byte *payload, unsigned int length) 
{
  Serial.print("Mensagem recebida do topico ");
  Serial.println(topic);
  Serial.print("Mensagem: ");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void WIFIConnect()
{
  Serial.println("Conectando a rede wifi!");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Conectando a rede wifi....");
  }
  Serial.println("Conectado a rede wifi");
}