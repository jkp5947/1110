#include "chat_h.h"
#include <pthread.h>


void *receiver(void *arg);

int main(void)
{
   int fd;
   int pid;
   char buf[BUF_SIZ];
   char msg[BUF_SIZ];
   pthread_t t_receiver;

   if((fd = open(SERVER_FIFO, O_WRONLY))==-1)
   {
      printf("open error_s\n");
      exit(1);
   }

   pid = getpid();

   pthread_create(&t_receiver, NULL, receiver, NULL);

   while(1)
   {
      fgets(buf,BUF_SIZ,stdin); 
      sprintf(msg,"%d%s",pid,buf);
//      printf("buf -- %s \n",msg);
      if(write(fd,msg,BUF_SIZ) != -1)
         continue;
   }
   close(fd);
}

void *receiver(void *arg)
{
   int fd, pid;
   char buf[BUF_SIZ];

   sprintf(buf,"%s%d",USER_FIFO,getpid());
   mkfifo(buf,0777);
   if((fd = open(buf,O_RDONLY)) == -1)
   {
      printf("open error_u\n");
      exit(1);
   }

   while(1)
   {
      memset(buf,0,BUF_SIZ);

      if(read(fd,buf,BUF_SIZ) == -1)
         break;
      sscanf(buf,"%d%[^\n]",&pid,buf);
      printf("[R %d] %s\n",pid,buf);
      memset(buf,0,BUF_SIZ);
   }
   close(fd);
}
