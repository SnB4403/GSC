import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    client.subscribe("/esp8266/ultrasonic")

def on_message(client, userdata, message):
    print("Received message '" + str(message.payload) + "' on topic '" + message.topic)
    

def main():
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set("rpi", "groundscrew")
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect("192.168.43.195", 1883) 
    # Connect to the MQTT server and process messages in a background thread. 
    mqtt_client.loop_forever() 

if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
