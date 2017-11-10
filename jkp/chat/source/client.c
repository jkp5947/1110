#include "chat_client.h"

//아이디 생성.
void create_name(Client_info *c_data)
{
   puts("사용하고자 하는 ID를 입력하세요.\n");
   fgets(c_data->client_id,sizeof(c_data->client_id)-1,stdin);      
   sprintf(c_data->client_id, "%d|%s", 7, c_data->client_id);
   write(c_data->client_fd, c_data->client_id, sizeof(c_data->client_id));
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
      FD_ZERO(&readfds);
      FD_ZERO(&writefds);
      FD_SET(fileno(stdin), &writefds);
      FD_SET(fileno(stdout), &readfds);
      result = select(4, &readfds, &writefds, (fd_set *)0, (struct timeval *)0);
      if (result < 1)
      {
         perror("select");
         return 1;
      }

      for (fd = 0; fd < 4; fd ++)
      {
         if (FD_ISSET(fd, &readfds))
         {
            read(stdout, buf, sizeof(buf));
            puts(buf);
         }
         if (FD_ISSET(fd, &writefds))
         {
            read(stdin, buf, sizeof(buf));
            write(ct.client_fd, buf, sizeof(buf));

         }
      }
   }
   close(ct.client_fd);
   return 0;
      
}
