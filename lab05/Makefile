CC=gcc
CFLAGS=-std=c99

TARGET=client server

all: $(TARGET)

client: client.c
	$(CC) -o client client.c $(CFLAGS)

server: server.c
	$(CC) -o server server.c $(CFLAGS)

ser: server
	./server 51717

cli: client
	./client localhost 51717

clean:
	$(RM) $(TARGET)
