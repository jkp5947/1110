CC = gcc
LIBS = -lpthread

all : msg_p1 msg_p2

msg_p1 : msg_p1.c
	$(CC) -o $@ $^ $(LIBS)

msg_p2 : msg_p2.c
	$(CC) -o $@ $^ $(LIBS)


clean:
	rm -rf msg_p1 msg_p2
