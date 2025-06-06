import time
from paho.mqtt import client as mqtt_client

broker      = 'broker.emqx.io'
port        = 1883
client_id   = "py_iot"
topic_dht   = "lab318/dht"
topic_led   = "lab318/led"
topic_servo = "lab318/servo"

enable    = False
angle     = 5
step      = 30  
direction = 1


def on_message(client, userdata, message):
    print(f"Mensagem recebida de {message.topic}: {message.payload.decode()}")

def connect_mqtt():
    client = mqtt_client.Client(client_id)
    client.on_message = on_message
    client.connect(broker, port)
    print("Cliente conectado ao Broker MQTT")
    return client

def subscribe(client):
    client.subscribe(topic_dht)
    print(f"Assinado no tópico: {topic_dht}")

def publish_led(client):
    global enable
    enable = not enable    
    client.publish(topic_led, b'1' if enable else b'0')

def publish_servo(client):
    global angle, direction

    angle += step * direction
    if angle >= 180:
        angle = 180
        direction = -1
    elif angle <= 0:
        angle = 0
        direction = 1

    client.publish(topic_servo, str(angle).encode())
    print(f"Publicado em {topic_servo}: {angle} graus")


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_start()

    try:
        while True:
            publish_led(client)
            publish_servo(client)
            time.sleep(5)
    except KeyboardInterrupt:
        print("Encerrando o cliente...")
        client.loop_stop()

if __name__ == '__main__':
    run()
