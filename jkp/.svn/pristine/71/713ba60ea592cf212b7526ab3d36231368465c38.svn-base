CC = gcc
LIBS = -lpthread

all : thcreate oddeven semeng

thcreate : thcreate.c
	$(CC) -o $@ $^ $(LIBS)

oddeven : oddeven_mutex.c
	$(CC) -o $@ $^ $(LIBS)

semeng : semeng1.c
	$(CC) -o $@ $^ $(LIBS)
clean :
	rm -rf thcreate oddeven semeng 
