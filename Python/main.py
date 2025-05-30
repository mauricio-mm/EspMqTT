import time
from paho.mqtt import client as mqtt_client

broker = 'broker.emqx.io'
port = 1883
topic = "lab318/temp"
client_id = "py_iot" 

def on_message(client, userdata, message):
    print(f"Mensagem recebida: {message.payload.decode()}")

def connect_mqtt():
    client = mqtt_client.Client(client_id)
    client.on_message = on_message
    client.connect(broker, port)
    print("Cliente conectado ao Broker MQTT")
    return client

def subscribe(client):
    client.subscribe(topic)
    print(f"Assinado no tópico: {topic}")

def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_start()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Encerrando o cliente...")
        client.loop_stop()

if __name__ == '__main__':
    run()
