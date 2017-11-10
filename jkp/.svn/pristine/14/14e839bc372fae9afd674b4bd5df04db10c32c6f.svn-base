CC			= gcc
CFLAGS	= -W -Wall
TARGET	= server

all : $(TARGET)

$(TARGET) : server.o linked_list.o
	$(CC) $(CFLAGS) -o $@ $^

test.o : server.c
	$(CC) $(CFLAGS) -c -o $@ $^

linked_list.o : linked_list.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean :
	rm -rf *.o server
