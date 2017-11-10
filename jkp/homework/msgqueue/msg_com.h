#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

#define MAX_TEXT 1024

struct msg_st{
   long int msg_type;
   char data[MAX_TEXT];
};
