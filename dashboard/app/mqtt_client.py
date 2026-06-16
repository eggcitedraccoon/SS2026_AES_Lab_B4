import paho.mqtt.client as mqtt

mqtt_client = None
devices = {}

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT broker successfully")
        # Subscribe to all responses to discover devices
        # We subscribe to both /+/response and +/response to be safe, 
        # although the standard in this project seems to be starting with /
        client.subscribe("/+/response")
        client.subscribe("+/response")
    else:
        print(f"Failed to connect to MQTT broker with code: {rc}")

def on_message(client, userdata, msg):
    global devices
    topic = msg.topic
    # Normalize topic: remove leading slash if present
    if topic.startswith('/'):
        topic = topic[1:]
    
    topic_parts = topic.split('/')
    if len(topic_parts) < 2:
        return
    
    device_id = topic_parts[0]
    if not device_id.startswith("WS-"):
        return

    try:
        import json
        payload_str = msg.payload.decode()
        # Clean up common malformed JSON issues (like missing quotes around keys)
        # However, it's better to log the error so the user knows why it failed
        try:
            payload = json.loads(payload_str)
        except json.JSONDecodeError:
            # Try to handle common non-strict JSON if possible, or just log it
            # Simple attempt: wrap unquoted keys in quotes if they look like {key: value}
            import re
            fixed_payload = re.sub(r'(\{|,)\s*([a-zA-Z0-9_]+)\s*:', r'\1"\2":', payload_str)
            payload = json.loads(fixed_payload)
            print(f"Warning: Handled malformed JSON from {device_id}")

        # Expected JSON: {"moisture": 45, "water_low": false}
        devices[device_id] = {
            "id": device_id,
            "name": f"Watering Station {device_id[3:]}",
            "moisture": payload.get("moisture"),
            "water_low": payload.get("water_low"),
            "last_seen": __import__('time').time()
        }
        print(f"Updated {device_id}: {payload}")
    except Exception as e:
        print(f"Error parsing MQTT message from {device_id}: {e}")
        print(f"Raw payload was: {msg.payload.decode()}")

def water_plant(device_id):
    global mqtt_client
    if mqtt_client:
        topic = f"/{device_id}/request"
        mqtt_client.publish(topic, "WATER")
        print(f"Sent WATER command to {topic}")

def init_mqtt(app):
    global mqtt_client

    mqtt_client = mqtt.Client()

    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(
        app.config["MQTT_BROKER"],
        app.config["MQTT_PORT"]
    )

    mqtt_client.loop_start()
