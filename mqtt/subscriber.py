#subscriber.py
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    client.subscribe("IoT/timer")

def on_message(client, userdata, msg):
    print("[{}]: {}".format(msg.topic, str(msg.payload)))

client = mqtt.Client()

client.on_connect = on_connect

client.on_message = on_message

#client.username_pw_set("myuser","mypassword")

client.connect("210.240.193.178", 1883)

client.loop_forever()
