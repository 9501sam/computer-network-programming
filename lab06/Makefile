CC = gcc
CFLAGS = -Wall -std=c99

TARGET = server client

ALL: $(TARGET)

server: selectserver.c
	$(CC) -o $@ $^ $(CFLAGS)

client: client.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(TARGET)
