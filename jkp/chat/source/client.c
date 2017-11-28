#include "chat_client.h"

int main(void)
{
   int result;
   char buf[1024];
   char msg[1024];
   int fd;
   int flag_recv; // 수신 flag
   int flag_id;   // id check flag
   int flag_exit; // exit flag
   fd_set readfds; 
   
   ct.chat_flag = 0;
   ct.client_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
   ct.client_address.sin_family = AF_INET;
   ct.client_address.sin_addr.s_addr = htonl(INADDR_ANY);
   ct.client_address.sin_port = htons(5947);
   ct.client_len = sizeof(ct.client_address);

   result = connect(ct.client_fd, (struct sockaddr *)&(ct.client_address), ct.client_len);

   if (result == -1)
   {
      printf("ERROR : %d,%s\n",errno, strerror(errno));
      return 1;
   }

   list_recv(&ct);   // 접속자 목록 수신/display
   id_create(&ct);   // id 생성
   menu_display();   // 메뉴 디스플레이.

   while(1)
   {
      FD_ZERO(&readfds);
      FD_SET(fileno(stdin), &readfds); //fileno() stdin->0, stdout->1, stderr->2
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
            if (fd == 0)   // STDIN (입력시)
            {
               read(fd, msg, sizeof(msg));
               if (ct.chat_flag == 0)  // 채팅중이 아닐 때
               {
                  flag_exit = menu_choice(&ct, msg);
                  if (flag_exit == -1)  //종료하기 선택시
                     return 0;
               }
               else                    // 채팅중 일때
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
                     if (strncmp(msg, "\n", 1) != 0)  //개행문자 발신하지 않기 위함.
                     {
                        snprintf(buf,sizeof(buf), "%d|%s", 0, msg);
                        write(3, buf, sizeof(buf));
                     }
                     else
                        break;
                  }
               }
            }
            else           // socket으로 수신시
            {
               read(fd, buf, sizeof(buf)); 
               sscanf(buf, "%d|%[^\n]", &flag_recv, buf);
               switch(flag_recv)
               {
                  case 0:  // 채팅 메세지
                     printf("%s\n",buf);
                     break;
                  case 1:  // 대화명 확인
                     flag_id = strcmp("DuplicateID", buf);
                     if (flag_id == 0)
                     {
                        printf("중복된 아이디 입니다.\n");
                        id_create(&ct);
                     }
                     else
                        printf("ID add success\n");
                     break;
                  case 2:  // 접속자목록 수신
                     list_recv(&ct);
                     break;
                  case 3:  // 초대 수신
                     printf("%s\n",buf);
                     if (strncmp(buf, "No", 2) != 0)  // 초대 상대가 있을때
                        invite_recv(&ct);
                     else
                        menu_display();               // 초대 상대가 없을때
                     break;
                  case 4:  // 수락/거절 확인
                     printf("%s\n",buf);
                     if (strncmp(buf, "채팅", 4)==0)
                        ct.chat_flag = 1; 
                     else
                        menu_display();
                     break;
                  case 5:  // history
                     printf("%s\n",buf);
                     break;
                  case 6:  // 대화 종료시
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
