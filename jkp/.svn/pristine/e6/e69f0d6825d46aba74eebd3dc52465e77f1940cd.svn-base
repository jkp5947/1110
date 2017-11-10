#include "chat_h.h"

int main(void)
{
   int fd;
   int i,j,c_pid;
   int pid[2] = {0};
   char filename[BUF_SIZ];
   char buf[BUF_SIZ];
   char msg[BUF_SIZ];
   int clientfd[2];

   mkfifo(SERVER_FIFO, 0777);

   if((fd = open(SERVER_FIFO, O_RDWR)) == -1)
   {
      perror("server open error\n");
      exit(1);
   }
   printf("Start!\n");

   while(1)
   {
      i=0;
      memset(buf,0,BUF_SIZ);
      if(read(fd,buf,BUF_SIZ)==-1)
         break;
//      printf("buf = %s\n",buf);
      sscanf(buf, "%d%[^\n]", &c_pid, buf); //%[^\n]의 의미 : []->scanlist
                                            // scanlist에 명시된 글자만을 입력
                                            // 으로 받는다. ^ -> 명시된 글자 
                                            // 제외한 글자만을 입력을 받는다.
                                            // EOF가 나오기 전까지 읽는다는 뜻.
      printf("%d-> %s\n",c_pid,buf);
      if(pid[i] == 0 || pid[0]==c_pid)
      {
         pid[i] = c_pid;
      }
      else
      {
         pid[++i] = c_pid;
      }
      sprintf(filename, "%s%d", USER_FIFO, c_pid);
      mkfifo(filename, 0777);
      clientfd[i] = open(filename, O_WRONLY);
      if (i == 0)
      {
         if(clientfd[1] != -1)
         {
           sprintf(msg, "%d%s", pid[0],buf);
           write(clientfd[1],msg,strlen(msg));
         }         
         else
         {
           printf("not enough client\n");
           continue;
         }
         
      }      
      else{
         sprintf(msg, "%d%s", pid[1],buf);
         write(clientfd[0],msg,strlen(msg));
      }
   }

}
