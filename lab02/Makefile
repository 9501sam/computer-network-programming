CC=gcc
CFLAGS=-Wall

all: getHostaddr getHostname getip

getHostaddr: getHostaddr.c
	$(CC) -o getHostaddr getHostaddr.c
getHostname: getHostname.c
	$(CC) -o getHostname getHostname.c
getip: getip.c
	$(CC) -o getip getip.c

clean:
	$(RM) getHostaddr getHostname getip
