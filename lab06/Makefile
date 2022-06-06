CC = gcc
CFLAGS = -Wall

TARGET = selectserver client

ALL: $(TARGET)

selectserver: selectserver.c
	$(CC) -o $@ $^ $(CFLAGS)

client: client.c
	$(CC) -o $@ $^ $(CFLAGS)

ser: selectserver
	./selectserver

cli: client
	./client 127.0.0.1 12345

clean:
	$(RM) $(TARGET)
