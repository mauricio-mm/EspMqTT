
#include "utils.h"

const char* ssid = "POCOX6Pro";    
const char* password = "esppucrs";

void WIFIConnect(WiFiClient *espClient)
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

void MQTTConnect(PubSubClient *MQTT)
{
  while(!MQTT->connected()) 
  {
    if (MQTT->connect(ID_MQTT))
    {
      Serial.println("Conectado ao Broker!");
      MQTT->subscribe(topic_led);      
    } 
    else 
    {
      Serial.print("Falha na conexão. O status é: ");
      Serial.print(MQTT->state());
      delay(2000);
    }
  }
}

void publish_data(PubSubClient *MQTT,const char *topic, String data)
{
  MQTT->publish(topic, data.c_str());
}

void callback(char *topic, byte *payload, unsigned int length) 
{
  if(String(topic) == "lab318/led") 
  {
      char msg_temp;
      
      for (int i = 0; i < length; i++) 
      {              
        //Serial.print((char) payload[i]);  
        int estado = (char) payload[i] == '1' ? HIGH : LOW;      
        digitalWrite(LEDPIN, estado);

      }        
  }

  if(String(topic) == "lab318/servo") 
  {
      for (int i = 0; i < length; i++) 
      {                      
        int val = (char) payload[i];              
        // servo.write(map(val, 0, 1023, 0, 180)); 

      }        
  }
  
}