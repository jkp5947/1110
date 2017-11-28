#include "chat_client.h"

//아이디 생성.
void id_create(Client_info *c_data)
{
   char buf[1024]={0};
   puts("사용하고자 하는 ID를 입력하세요.");
   scanf("%s",c_data->client_id);
   sprintf(buf, "%d|%s", 1, c_data->client_id);
   write(c_data->client_fd, buf, sizeof(buf));
   return ;
}

//접속자 목록 요청
void list_request(Client_info *ct)
{
   char buf[1024];
   sprintf(buf, "%d|%s", 2, "list");
   write(ct->client_fd, buf, sizeof(buf));
   return ;
}

//user_list와 메뉴를 화면에 디스플레이
void menu_display(void)
{
   printf("----메뉴 선택----\n");
   printf("|  1.초대하기   |\n");
   printf("|  2.종료하기   |\n");
   printf("-----------------\n");
   return ;
}

//메뉴를 선택시 그에 맞는 역할을 수행
int menu_choice(Client_info *ct, char *msg)
{
   char buf[1024]={0};
   char temp[10];
   if (strncmp(msg, "1.초대하기",10) == 0)
   {
      list_request(ct);
      list_recv(ct);
      puts("초대할 아이디를 입력해 주세요.");
      scanf("%s",temp);
      sprintf(buf, "%d|%s", 3, temp);    
      write(3, buf, sizeof(buf));
      return 0;
   }
   else if (strncmp(msg, "2.종료하기",10) == 0)
      return -1;
   else
      return 0;
}

//접속자 목록을 받아 내용을 리턴.
void list_recv(Client_info *ct)
{
   int recv_flag;
   char buf[1024];

   read(ct->client_fd, buf, sizeof(buf));
   sscanf(buf, "%d|%s", &recv_flag, buf);
   if (strcmp(buf, "Nolist")==0)
      printf("접속자가 없습니다.\n");
   else
   {
      printf("---접속자 목록---\n");
      printf("|%-15s|\n", buf);
      while (1)
      {  
         read(ct->client_fd, buf, sizeof(buf));
         sscanf(buf, "%d|%s", &recv_flag, buf);
         if (strcmp(buf, "end")==0)
            break;
         else
            printf("|%-15s|\n", buf);
      }
      printf("-----------------\n");
   }
   return ;
}

//나가기 입력시 server로 채팅방 종료 요청
void client_exit(Client_info *ct)
{
   char buf[1024];

   sprintf(buf, "%d|%s", 6, "나가기");
   write(ct->client_fd, buf, sizeof(buf)); 
   return ;
}

//history 요청시 server에 요청을 send.
void history_request(Client_info *ct)
{
   char buf[1024];

   sprintf(buf, "%d|%s", 5, "call_history");
   write(ct->client_fd, buf, sizeof(buf));                  
   return ;
}

// 초대 요청 수신시 수락/거절을 선택하여 서버로 send
void invite_recv(Client_info *ct)
{
   char buf[10];
   scanf("%s",buf);
   if (strncmp(buf, "y", 1) == 0 
      || strncmp(buf, "Y", 1) == 0)
   {
      sprintf(buf, "%d|%s", 4, "수락");
      write(3, buf, 10);
   }  
   else if (strncmp(buf, "n", 1) == 0 
      || strncmp(buf, "N", 1) == 0)
   {
      sprintf(buf, "%d|%s", 4, "거절");
      write(3, buf, 10);
   }
   else
      invite_recv(ct);
   return ;
}
