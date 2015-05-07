CC=gcc
CFLAGS=-pthread -Wall

all: server client

server:
server-pthread:
	$(CC) $(CFLAGS) -DUSE_PTHREAD server.c -o server

server-fork:
	$(CC) $(CFLAGS) server.c -o server

client:
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm server client
