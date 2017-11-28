#include "chat_client.h"

int main(void)
{
   int result;
   char buf[1024];
   char msg[1024];
   int fd;
   int recv_flag;
   int check;
   int exit_flag;
   ct.chat_flag = 0;
   ct.client_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
   ct.client_address.sin_family = AF_INET;
   ct.client_address.sin_addr.s_addr = htonl(INADDR_ANY);
   ct.client_address.sin_port = htons(5947);
   ct.client_len = sizeof(ct.client_address);
   fd_set readfds; 

   result = connect(ct.client_fd, (struct sockaddr *)&(ct.client_address), ct.client_len);

   if (result == -1)
   {
      printf("ERROR : %d,%s\n",errno, strerror(errno));
      return 1;
   }

   list_recv(&ct);
   id_create(&ct); 
   menu_display();

   while(1)
   {
      FD_ZERO(&readfds);
      FD_SET(fileno(stdin), &readfds);
      FD_SET(ct.client_fd, &readfds);
      result = select(4, &readfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
      if (result < 1)
      {
         perror("select");
         return 1;
      }

      for (fd = 0; fd < 4; fd ++)
      {
         if (FD_ISSET(fd, &readfds))
         {
            if (fd == 0)
            {
               read(fd, msg, sizeof(msg));
               if (ct.chat_flag == 0)
               {
                  exit_flag = menu_choice(&ct, msg);
                  if(exit_flag == -1)
                     return 0;
               }
               else
               {
                  if (strncmp(msg, "/나가기", 7) == 0)
                     client_exit(&ct);
                  else if (strncmp(msg, "/history", 8) == 0)
                     history_request(&ct);
                  else if (strncmp(msg, "/명령어", 7) == 0)
                  {
                     printf("/나가기 : 채팅방 나가기\n");
                     printf("/history : 대화 히스토리 출력\n");
                  }
                  else
                  {
                     if (strncmp(msg, "\n", 1) != 0)
                     {
                        snprintf(buf,sizeof(buf), "%d|%s", 0, msg);
                        write(3, buf, sizeof(buf));
                     }
                     else
                        break;
                  }
               }
            }
            else 
            {
               read(fd, buf, sizeof(buf)); 
               sscanf(buf, "%d|%[^\n]", &recv_flag, buf);
               switch(recv_flag)
               {
                  case 0:
                     printf("%s\n",buf);
                     break;
                  case 1:
                     check = strcmp("DuplicateID", buf);
                     if (check == 0)
                     {
                        printf("중복된 아이디 입니다.\n");
                        id_create(&ct);
                     }
                     else
                        printf("ID add success\n");
                     break;
                  case 2:
                     list_recv(&ct);
                     break;
                  case 3:
                     printf("%s\n",buf);
                     if (strncmp(buf, "No", 2) != 0)
                        invite_recv(&ct);
                     else
                        menu_display();
                     break;
                  case 4:
                     printf("%s\n",buf);
                     if (strncmp(buf, "채팅", 4)==0)
                        ct.chat_flag = 1; 
                     else
                        menu_display();
                     break;
                  case 5:
                     printf("%s\n",buf);
                     break;
                  case 6:
                     ct.chat_flag = 0;
                     printf("대화가 종료되었습니다.\n");
                     menu_display();
                     break;
               }
            }
         }
      }
   }
   close(ct.client_fd);
   return 0;
}
