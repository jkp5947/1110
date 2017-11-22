#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>

typedef struct _info
{
   int client_fd;
   char client_id[10];
   socklen_t client_len;
   struct sockaddr_in client_address;
   int chat_flag;
}Client_info; //클라이언트 정보를 담는 구조체
