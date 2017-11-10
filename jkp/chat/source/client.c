#include "chat_client.h"

//아이디 생성.
void create_name(Client_info *c_data)
{
   char buf[10];
   puts("사용하고자 하는 ID를 입력하세요.");
   fgets(c_data->client_id,sizeof(c_data->client_id)-1,stdin);      
   sprintf(buf, "%d|%s", 1, c_data->client_id);
   write(c_data->client_fd, buf, sizeof(buf));
}

//user_list와 메뉴를 화면에 디스플레이
void display(Client_info *ct, char *user_list[])
{

}
//메뉴를 선택시 그에 맞는 역할을 수행
void choice_menu(Client_info *ct, int num)
{

}
//접속자 목록을 받아 내용을 리턴.
char recv_list(Client_info *ct)
{

}
//나가기 입력시 종료시키는 클라이언트의 fd 를 리턴하여 서버에 알림.
int exit_client(Client_info *ct)
{

}
//history 요청시 server에 요청을 send.
void history_request(Client_info *ct)
{

}
// 수락거절 회신시 데이터 받고 거절시 안내 메시지 출력
char acp_recv(Client_info *ct, char *acp)
{

}
// 초대 요청 수신시 수락/거절을 선택하여 서버로 send
char inv_recv(Client_info *ct, char *request)
{

}

int main(void)
{
   int result;
   char buf[BUFSIZ];
   int fd;
   int recv_flag;
   ct.client_fd = socket(AF_INET, SOCK_STREAM, 0);
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
   create_name(&ct); 
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
               printf("썼넹\n");
               read(fd, buf, sizeof(buf));

            }
            else 
            {
               printf("읽엇넹\n");
               read(fd, buf, sizeof(buf)); 
               sscanf(buf, "%d|%[^/n]", &recv_flag, buf);
               switch(recv_flag)
               {
                  case 0:
                     break;
                  case 1:
                     check = strcmp("Duplicate ID", buf);
                     if (check == 0)
                     {
                        printf("중복된 아이디 입니다.\n");
                        create_name(&ct);
                     }
                     else
                        printf("ID add success\n");
                     break;
                  case 2:
                     check = strncmp("No list", buf, 10);
                     if (check == 0)
                        printf("접속자가 없습니다.\n");
                     else
                        printf("접속자 : %s\n", buf);   
                     break;
                  case 3:
                     break;
                  case 4:
                     break;
                  case 5:
                     break;
                  case 6:
                     break;
               }
            }
         }
      }
   }
   close(ct.client_fd);
   return 0;
      
}
