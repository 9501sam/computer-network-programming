# multithread.py
import paho.mqtt.client as mqtt
import time, datetime
from datetime import datetime

def on_connect(client, userdata, flags, rc):
    client.subscribe("IoT/timer")

def on_message(client, userdata, msg):
    print("[{}]: {}".format(msg.topic, str(msg.payload)))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
#client.username_pw_set("myuser","mypassword")
client.connect("210.240.193.178", 1883)

client.loop_start()

while True:
    now = datetime.now()
    msg = now.strftime("%Y/%m/%d %H:%M:%S")
    client.publish("IoT/timer", msg)
    time.sleep(1)
