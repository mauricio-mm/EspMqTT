import random
import time

from paho.mqtt import client as mqtt_client

broker = 'broker.emqx.io'
port = 1883
topic = "lab318/temp"
client_id = "p3_iot" 

def on_message(client, userdata, message):
    print(f"Mensagem recebida: {message.payload.decode()}")

def connect_mqtt():
    client = mqtt_client.Client(client_id)
    client.on_message = on_message
    client.connect(broker, port)
    print("Cliente conectado ao Broker MQTT")
    return client

def publish(client):
    while True:
        time.sleep(1)
        valor_temp = random.randint(0, 100)
        msg = f"Temperatura: {valor_temp}"
        result = client.publish(topic, msg)
        print(f"Mensagem publicada: {msg}")

def subscribe(client):
    client.subscribe(topic)
    print(f"Assinado no tópico: {topic}")

def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_start() 
    publish(client)  	

if __name__ == '__main__':
    run()
