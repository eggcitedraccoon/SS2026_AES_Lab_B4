import paho.mqtt.client as mqtt

mqtt_client = None
latest_data = {}

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    client.subscribe(userdata["topic"])

def on_message(client, userdata, msg):
    global latest_data
    payload = msg.payload.decode()
    latest_data[msg.topic] = payload
    print(f"{msg.topic}: {payload}")

def init_mqtt(app):
    global mqtt_client

    mqtt_client = mqtt.Client(userdata={
        "topic": app.config["MQTT_TOPIC"]
    })

    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(
        app.config["MQTT_BROKER"],
        app.config["MQTT_PORT"]
    )

    mqtt_client.loop_start()
