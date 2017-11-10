#include "soc_h.h"

int main(void)
{
   int server_sockfd, client_sockfd;
   int server_len,client_len;
   struct sockaddr_un server_address;
   struct sockaddr_un client_address;
   int i;

   char buf[BUFSIZ];
   
   unlink("server_socket");
   server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

   server_address.sun_family = AF_UNIX;
   strcpy(server_address.sun_path, "server_socket");
   server_len = sizeof(server_address);
   bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

   listen(server_sockfd, 5);
   
   while(1)
   {
      printf("server waiting\n");
      client_len = sizeof(client_address);
      client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,&client_len);

      read(client_sockfd,buf, BUFSIZ);
      printf("From client : %s", buf);
      if(strncmp(buf,"end",3) == 0)
         break;
      for(i=0; i<strlen(buf)-1; i++)
      {
         if(buf[i] <91 && buf[i] > 64)
            buf[i]+=32;
         else if(buf[i]>96 && buf[i]<123)
            buf[i]-=32;
      }
      printf("To client : %s",buf);
      write(client_sockfd,buf,BUFSIZ);
      memset(buf,0,BUFSIZ);         
      
   }

   close(client_sockfd);

   exit(0);
}
