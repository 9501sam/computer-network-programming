# publisher.py
import paho.mqtt.client as mqtt

client = mqtt.Client()

client.username_pw_set("myuser","mypassword")

client.connect("210.240.193.178", 1883)

client.publish("hello/world", "test message")