#include "chat_client.h"

//아이디 생성.
void create_name(Client_info *c_data)
{
   char buf[10];
   puts("사용하고자 하는 ID를 입력하세요.");
   scanf("%s",c_data->client_id);
   sprintf(buf, "%d|%s", 1, c_data->client_id);
   write(c_data->client_fd, buf, sizeof(buf));
}

//user_list와 메뉴를 화면에 디스플레이
void display(Client_info *ct)
{
   printf("-----메뉴 선택-----\n");
   printf("|   1.초대하기    |\n");
   printf("|   2.종료하기    |\n");
   printf("-----------------\n");

}
//메뉴를 선택시 그에 맞는 역할을 수행
void choice_menu(Client_info *ct, int num)
{

}
//접속자 목록을 받아 내용을 리턴.
void recv_list(Client_info *ct, char *buf)
{
   int recv_flag;

   read(3, buf, sizeof(buf));
   sscanf(buf, "%d|%s", &recv_flag, buf);
   if (strcmp(buf, "Nolist")==0)
      printf("접속자가 없습니다.\n");
   else
   {
      printf("---접속자 목록---\n");
      printf("|%-15s|\n", buf);
      while (1)
      {  
         read(3, buf, sizeof(buf));
         sscanf(buf, "%d|%s", &recv_flag, buf);
         if (strcmp(buf, "end")==0)
            break;
         else
            printf("|%-15s|\n", buf);
      }
      printf("-----------------\n");
   }

}
//나가기 입력시 종료시키는 클라이언트의 fd 를 리턴하여 서버에 알림.
int exit_client(Client_info *ct)
{

}
//history 요청시 server에 요청을 send.
void history_request(Client_info *ct)
{
   
}
// 초대 요청 수신시 수락/거절을 선택하여 서버로 send
void inv_recv(Client_info *ct)
{
   char buf[10];
   char temp[10];
   //printf("inv_recv!!!\n");
   scanf("%s",buf);
   //printf("%s\n",buf);
   if (strncmp(buf, "y", 1) == 0 
      || strncmp(buf, "Y", 1) == 0)
   {
      sprintf(temp, "%d|%s", 4, "수락");
      write(3, temp, 10);
   }  
   else if (strncmp(buf, "n", 1) == 0 
      || strncmp(buf, "N", 1) == 0)
   {
      sprintf(temp, "%d|%s", 4, "거절");
      write(3, temp, 10);
   }
   else
   {
      inv_recv(ct);
   }
   return ;
}

int main(void)
{
   int result;
   char buf[BUFSIZ];
   char temp[BUFSIZ];
   int fd;
   int recv_flag;
   ct.chat_flag = 0;
   ct.client_fd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
   ct.client_address.sin_family = AF_INET;
   ct.client_address.sin_addr.s_addr = htonl(INADDR_ANY);
   ct.client_address.sin_port = htons(5947);
   ct.client_len = sizeof(ct.client_address);
   fd_set readfds,writefds;


   result = connect(ct.client_fd, (struct sockaddr *)&(ct.client_address), ct.client_len);

   if (result == -1)
   {
      printf("ERROR : %d,%s\n",errno, strerror(errno));
      return 1;
   }


   read(3, buf, sizeof(buf));
   sscanf(buf, "%d|%s", &recv_flag, temp);
   if (strcmp(temp, "Nolist")==0)
      printf("접속자가 없습니다.\n");
   else
   {
      printf("---접속자 목록---\n");
      printf("|%-15s|\n", temp);
      while (1)
      {  
         read(3, buf, sizeof(buf));
         sscanf(buf, "%d|%s", &recv_flag, temp);
         if (strcmp(temp, "end")==0)
            break;
         else
            printf("|%-15s|\n", temp);
      }
      printf("-----------------\n");
   }
   create_name(&ct); 
   display(&ct);
   
   while(1)
   {
      int check;
      FD_ZERO(&readfds);
      FD_ZERO(&writefds);
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
               read(fd, buf, sizeof(buf));
               //printf("쎴녱 %s\n", buf);
               if (ct.chat_flag == 0)
               {
                  if (strncmp(buf, "1.초대하기",10) == 0)
                  {
                     puts("초대할 아이디를 입력해 주세요.");
                     scanf("%s",buf);
                     sprintf(temp, "%d|%s", 3, buf);    
                     write(3, temp, sizeof(temp));
                  }
                  else if (strncmp(buf, "2.종료하기",10) == 0)
                     return 0;
               }
               else
               {
                  if (strcmp(buf, "/나가기") == 0)
                  {
                  
                  }
                  else if (strncmp(buf, "/history", 8) == 0)
                  {
                     sprintf(temp, "%d|%s", 5, "call_history");
                     write(3, temp, sizeof(temp));                  
                  }
                  else
                  {
                     if (strncmp(buf, "\n", 1) != 0)
                     {
                        sprintf(temp, "%d|%s", 0, buf);
                        write(3, temp, sizeof(buf));
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
               //printf("읽엇넹\n");
               //printf("recv_flag = %d\n",recv_flag);
               //printf("buf = %s\n", buf);
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
                        create_name(&ct);
                     }
                     else
                        printf("ID add success\n");
                     break;
                  case 2:
                     break;
                  case 3:
                     printf("%s\n",buf);
                     if (strncmp(buf, "No", 2) != 0)
                        inv_recv(&ct);
                     else
                        display(&ct);
                     break;
                  case 4:
                     printf("%s\n",buf);
                     if (strncmp(buf, "채팅", 4)==0)
                        ct.chat_flag = 1; 
                     else
                        display(&ct);
                     break;
                  case 5:
                     //history_request(&ct);
                     printf("%s\n",buf);
                     break;
               }
            }
         }
      }
   }
   close(ct.client_fd);
   return 0;
      
}
