#include "soc_h.h"

int main(void)
{
   int sockfd;
   int len;
   struct sockaddr_un address;
   int result,run=1;
   char buf[BUFSIZ];


   while(run)
   {
      sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

      address.sun_family = AF_UNIX;
      strcpy(address.sun_path, "server_socket");
      len = sizeof(address);
      result = connect(sockfd, (struct sockaddr *)&address, len);

      if(result == -1)
      {
         ("oops : client1");
         exit(1);
      }
      fgets(buf, BUFSIZ, stdin);
      write(sockfd,buf,BUFSIZ);
      if(strncmp(buf,"end",3)==0)
         run = 0;
      read(sockfd,buf,BUFSIZ);
      printf("char from server = %s",buf);
      memset(buf, 0, BUFSIZ);

   }
   close(sockfd);
   exit(0);


}
