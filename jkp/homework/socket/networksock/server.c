#include "sock_h.h"
#include <sys/time.h>

int main()
{
   int server_sockfd, client_sockfd;
   int server_len, client_len;
   struct sockaddr_in server_address;
   struct sockaddr_in client_address;
   int result, i;
   int cnt = 0;
   fd_set readfds,testfds;

   server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(9734);
   server_len = sizeof(server_address);

   bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

   listen(server_sockfd, 5);

   FD_ZERO(&readfds);
   FD_SET(server_sockfd, &readfds);

   while (1)
   {
      char buf[BUFSIZ];
      int fd;
      int nread;

      testfds = readfds;

      printf("server waiting\n");
      result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
      if (result < 1)
      {
         perror("server");
         exit(1);
      }

      for (fd = 0; fd < FD_SETSIZE; fd++)
      {
         if (FD_ISSET(fd, &testfds))
         {
            if (fd == server_sockfd)
            {
               client_len = sizeof(client_address);
               client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
               FD_SET(client_sockfd, &readfds);

               printf("adding client on fd %d\n", client_sockfd);
               cnt++;
            }
            else
            {
               read(fd, buf, BUFSIZ);
               printf("[%d] : %s\n", fd, buf);
               if (strncmp(buf, "end", 3) == 0)
               {
                  close(fd);
                  FD_CLR(fd, &readfds);
                  printf("removing client on fd %d\n",fd);
                  if (--cnt == 0)
                     exit(0);
               }         
               else
               {
                  sprintf(buf, "%s(%d)", buf, (int)strlen(buf));
                  write(fd, buf, BUFSIZ);
               }
            }
         }
      }
   }
}
