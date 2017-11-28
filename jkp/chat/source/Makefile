CC			= gcc
CFLAGS	= -W -Wall
TARGET1	= server
TARGET2	= client

all : $(TARGET1) $(TARGET2)

$(TARGET1) : chat_server.o server.o linked_list.o 
	$(CC) $(CFLAGS) -o $@ $^

chat_server.o : chat_server.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
server.o : server.c
	$(CC) $(CFLAGS) -c -o $@ $^

linked_list.o : linked_list.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET2) : client.o chat_client.o
	$(CC) $(CFLAGS) -o $@ $^

client.o : client.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
chat_client.o : chat_client.c
	$(CC) $(CFLAGS) -c -o $@ $^
clean :
	rm -rf *.o server client
