#include "sock_h.h"
#include <arpa/inet.h>



int main(void)
{
   int sockfd;
   int len;
   struct sockaddr_in address;
   int result;
   char buf[BUFSIZ];
   int running = 1;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   printf("fd = %d\n",sockfd);
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = htonl(INADDR_ANY);
   address.sin_port = htons(9734);
   len = sizeof(address);

   result = connect(sockfd, (struct sockaddr *)&address, len);

   if(result == -1) {
      printf("ERROR : %d,%s\n",errno, strerror(errno));
      exit(1);
   }

   while(running)
   {
      fgets(buf, BUFSIZ, stdin);
      buf[strlen(buf)-1]=0;
      write(sockfd, buf, BUFSIZ);
      if(strncmp(buf, "end", 3)==0)
         running = 0; 
      read(sockfd, buf, BUFSIZ);
      printf("return from server = %s\n", buf);
      memset(buf,0,BUFSIZ);
   }
   close(sockfd);
   exit(0);
}


