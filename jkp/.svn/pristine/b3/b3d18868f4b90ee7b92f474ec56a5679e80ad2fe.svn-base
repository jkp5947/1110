CC = gcc
LIBS = -lpthread

all : server client

server : server.c
	$(CC) -o $@ $^
client : client.c
	$(CC) -o $@ $^ $(LIBS)

clean:
	    rm -rf server client 
