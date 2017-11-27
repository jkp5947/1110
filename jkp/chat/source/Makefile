CC			= gcc
CFLAGS	= -W -Wall
TARGET1	= server
TARGET2	= client

all : $(TARGET1) $(TARGET2)

$(TARGET1) : server.o linked_list.o
	$(CC) $(CFLAGS) -o $@ $^

server.o : server.c
	$(CC) $(CFLAGS) -c -o $@ $^

linked_list.o : linked_list.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET2) : client.o
	$(CC) $(CFLAGS) -o $@ $^

client.o : client.c
	$(CC) $(CFLAGS) -c -o $@ $^
clean :
	rm -rf *.o server client
