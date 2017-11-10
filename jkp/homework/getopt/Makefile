CC = gcc
CFLAGS = -W -Wall
TARGET = getopt_test
all : $(TARGET)

$(TARGET) : getopt_test.c
	$(CC) $(CFLAGS) -o $@ getopt_test.c

clean :
	rm -rf $(TARGET)
