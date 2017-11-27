#include "chat_server.h"

// 메시지를 수신하였을 때 상대 Client에게 send하고 history에 저장.
void msg_send(jkp_global *g_data, int fd, char *buf)
{
   long ofs_0, ofs_1;   // room의 att_fd offset 1,2
   int i;               // for문 사용하기 위한 변수
   int recv_fd;         // 메세지를 받을 fd
   int id_index;        // id를 찾기 위한 index 변수
   int room_index;      // room을 찾기 위한 index 변수
   char history[1024];  // history에 저장하기 위한 공간
   char message[1024];  // message를 보내기 위한 공간
   Node *r_eye;
   Node *c_eye;
   r_eye = g_data->r_list.head->next;
   c_eye = g_data->c_list.head->next;

   ofs_0 = OFFSET(Room, att_fd[0]);
   ofs_1 = OFFSET(Room, att_fd[1]);

   id_index = linkedlist_search(&(g_data->c_list), &fd, 0, 4, 0);
   for (i = 1; i < id_index; i++)
      c_eye = c_eye->next;
   sprintf(history, "[%s]%s", ((Client_data *)c_eye->pData)->ci.client_id, buf);

   if ((room_index = linkedlist_search(&(g_data->r_list), &fd, ofs_0, 4, 0)) != -1)
   {
      for (i = 1; i < room_index; i++)
         r_eye = r_eye->next;
      recv_fd = ((Room *)r_eye->pData)->att_fd[1];
   }
   else
   {
      recv_fd = linkedlist_search(&(g_data->r_list), &fd, ofs_1, 4, 1);
      room_index = linkedlist_search(&(g_data->r_list), &fd, ofs_1, 4, 0);
      for(i = 1; i < room_index; i++)
         r_eye = r_eye->next;
   }
   linkedlist_add(&(((Room *)r_eye->pData)->h_list), history, 1);

   sprintf(message, "%d|%s", 0, history);
   write(recv_fd, message, sizeof(message));

   return;
}

//recv한 클라이언트 대화명을 리스트에 추가하고
//Server화면에 입장 메시지를 출력
int id_recv(jkp_global *g_data, char *id, int fd)
{
   int index;  // id를 찾기위한 index
   int find;   // 찾은 id를 통해 반환된 fd를 저장하기 위한 변수
   int i;      // for문
   long ofs;   // client_id offset
   Node *eye;
   eye = g_data->c_list.head->next;

   ofs = OFFSET(Client_data, ci.client_id);
   find = linkedlist_search(&(g_data->c_list), id, ofs, sizeof(id), 0);
   if (find == -1)
   {
      index = linkedlist_search(&(g_data->c_list), &fd, 0, 4, 0);
      for( i = 1; i < index; i++)
         eye = eye->next;
      memcpy(((Client_data *)eye->pData)->ci.client_id, id, 10);
      return 0;
   }
   else 
      return -1;
}

//클라이언트에서 접속자 목록 요청시 클라이언트 데이터를 가지고있는
//linked list에서 대화명들을 요청한 클라이언트 fd를 통하여 전송.
void list_send(jkp_global *g_data, int request_fd)
{
   char buf[1024];
   Node *eye;
   eye = g_data->c_list.head->next;

   if (eye == g_data->c_list.tail)
   {
      sprintf(buf, "%d|%s", 2, "Nolist");
      write(request_fd, buf, sizeof(buf));
   }
   else
   {
      while (eye != g_data->c_list.tail)
      {
         sprintf(buf, "%d|%s", 2, ((Client_data *)eye->pData)->ci.client_id);
         write(request_fd, buf, sizeof(buf));    
         eye = eye->next;
         usleep(1000); 
      }
      sprintf(buf, "%d|%s", 2, "end");
      write(request_fd, buf, sizeof(buf));
   }
   return ;
}

//찾은 접속자에게 초대를 보냄
void invite_send(jkp_global *g_data, int inv_fd, int request_fd)
{
   Node *eye;
   char id[10];
   char buf[1024];
   char temp[1024];
   eye = g_data->c_list.head->next;

   while (eye != g_data->c_list.tail)
   {
      if (memcmp((char *)(eye->pData), &request_fd, 4) == 0)
      {
         memcpy(id, &(((Client_data *)eye->pData)->ci.client_id), 10); 
         break;
      }
      eye = eye->next;
   }
   sprintf(temp, "[%s]님이초대하셨습니다.수락하시겠습니까?(Y/N)",id);
   sprintf(buf, "%d|%s", 3, temp);
   write(inv_fd, buf, sizeof(buf));
   
   return ;
}

//요청한 fd를 통해 대화내용을 불러들이고 대화내용을 send
void history_call(jkp_global *g_data, int request_fd)
{
   int index;           //요청한 fd를 찾기위한 index
   int i;
   char buf[1024];      //history를 보내거나 history가 없음을 보낼 buf 
   Node *eye, *find;
   
   // request_fd의 room index를 찾는 과정.
   index = linkedlist_search(&(g_data->r_list), &request_fd, 0, 4, 0);
   if (index == -1)
   {
      index = linkedlist_search(&(g_data->r_list), &request_fd, 4, 4, 0);
      if (index == -1)
      {
         printf("찾을 수 없는 채팅 참여자\n");
         return ;
      }
   }      
   // 찾은 room_index의 history를 확인.
   eye = g_data->r_list.head->next;
   for (i = 1; i < index; i++)
      eye = eye->next;
   find = ((Room *)eye->pData)->h_list.head->next;
   if (find == ((Room *)eye->pData)->h_list.tail)
   {
      sprintf(buf, "%d|%s", 5, "No history");
      write(request_fd, buf, sizeof(buf));
   } 
   else
   {
      while(find != ((Room *)eye->pData)->h_list.tail)
      {
         sprintf(buf, "%d|%s", 5, ((History *)find->pData)->msg);
         write(request_fd, buf, sizeof(buf));
         find = find->next;
         usleep(1000);
      }
   }   
   return ;
}

//초대 수락시 방생성(h_list init)
void room_create(jkp_global *g_data, int index)
{
   Node *eye;
   int i;

   eye = g_data->r_list.head->next;
   for (i = 1; i < index; i++)
      eye = eye->next;
   linkedlist_init(&(((Room *)eye->pData)->h_list));
}

//수락/거절 회신시 flag를 통하여 수락상황과 거절상황 처리
// 수락시 -> chat_flag를 변경하고 채팅시작을 알림
// 거절시 -> 초대요청한 Client에게 거절알림.
void invite_recv(jkp_global *g_data, int fd, int flag_reply)
{
   long room_ofs; //요청당한 fd의 offset
   int index;
   int inv_fd;    //초대 요청한 fd
   char buf[1024];

   room_ofs = OFFSET(Room, att_fd[1]); 

   if (flag_reply == 0) //수락시 초대 요청한 fd를 찾고 방생성, chat_flag를 변경후 알린다.
   {
      index = linkedlist_search(&(g_data->r_list), &fd, room_ofs, 4, 0);
      room_create(g_data, index);
      inv_fd = linkedlist_search(&(g_data->r_list), &fd, room_ofs, 4, 1);

      chat_flag_change(g_data, fd, 1);
      chat_flag_change(g_data, inv_fd, 1);

      printf("[%d], [%d]의 채팅이 시작되었습니다.\n", inv_fd, fd);
      sprintf(buf, "%d|%s", 4, "채팅이시작되었습니다.");
      write(inv_fd, buf, sizeof(buf));
      write(fd, buf, sizeof(buf));

   }
   else  //거절시 
   {
      inv_fd = linkedlist_search(&(g_data->r_list), &fd, room_ofs, 4, 1);
      sprintf(buf, "%d|%s", 4, "거절하였습니다");
      write(inv_fd, buf, sizeof(buf));
      linkedlist_delete(&(g_data->r_list), &inv_fd, 0, 4);
   }
   return ;
}

// //chat_flag의 값을 Change_value로 변경하는 함수.
void chat_flag_change(jkp_global *g_data, int fd, int change_value)
{
   int index;
   int i;
   Node *eye;
   eye = g_data->c_list.head->next;
   
   index = linkedlist_search(&(g_data->c_list), &fd, 0, 4, 0);
   for (i = 1; i < index; i++)
      eye = eye->next;
   ((Client_data *)eye->pData)->ci.chat_flag = change_value;

   return;
}

//chat_flag의 값이 무엇인지 찾는 함수 (return chat_flag)
int chat_flag_search(jkp_global *g_data, int fd)
{
   int index;
   int i;
   Node *eye;
   eye = g_data->c_list.head->next;
   
   index = linkedlist_search(&(g_data->c_list), &fd, 0, 4, 0);
   for (i = 1; i < index; i++)
      eye = eye->next;
   return ((Client_data *)eye->pData)->ci.chat_flag;
}

//채팅방 종료시 채팅방의 모든 history data를 destroy, room data를 삭제 
//채팅 종료를 알림.
void chat_quit(jkp_global *g_data, int fd)
{
   int index;        // 종료한 fd가 속해 있는 room의 index
   int opponent_fd;  // 대화 상대의 fd
   int i;
   int chk = 0;      // fd가 att_fd 0번째인지 1번째인지 저장.
   char buf[1024];
   Node *eye;
   eye = g_data->r_list.head->next;

   if ((index = linkedlist_search(&(g_data->r_list), &fd, 0, 4, 0)) == -1)
   {
      index = linkedlist_search(&(g_data->r_list), &fd, 4, 4 ,0);
      chk = 1;
   }
   for (i = 1; i < index; i++)
      eye = eye->next;
   linkedlist_destroy(&((Room *)eye->pData)->h_list);

   if (chk == 0)
   {
      opponent_fd = ((Room *)eye->pData)->att_fd[1];
      linkedlist_delete(&(g_data->r_list), &fd, 0, 4);
   }
   else
   {
      opponent_fd = ((Room *)eye->pData)->att_fd[0];
      linkedlist_delete(&(g_data->r_list), &fd, 4, 4);
   }
   
   chat_flag_change(g_data, fd, 0);
   chat_flag_change(g_data, opponent_fd, 0);
   sprintf(buf, "%d|%s", 6, "대화종료");
   write(opponent_fd, buf, sizeof(buf));
   write(fd, buf, sizeof(buf));
   printf("[%d], [%d]의 채팅이 종료되었습니다.\n", fd, opponent_fd);

   return;
}

int main(void)
{
   int server_sockfd;
   int server_len;
   struct sockaddr_in server_address;
   int result;
   int nfds = 4;  //최대 fd + 1
   Room r_data;
   Client_data c_data;
   fd_set readfds, testfds;
   
   linkedlist_init(&(global_data.c_list));
   linkedlist_init(&(global_data.r_list));
   server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(5947);
   server_len = sizeof(server_address);

   bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

   listen(server_sockfd, 10);
   FD_ZERO(&readfds);
   FD_SET(server_sockfd, &readfds);

   while (1)
   {
      char buf[1024];
      int fd;           // 현재 연결된 fd
      int flag_recv;    // 수신 flag
      int flag_reply;   // 채팅초대 수락/거절 회신 flag 
      int nread;        // 읽은 갯수(Client의 종료를 알기 위해 사용)
      int check_id;     // id 중복 check
      int flag_chat;    // chat_flag를 확인하는 flag
      testfds = readfds;

      printf("server waiting\n");
      linkedlist_enumerate(&(global_data.c_list),0);

      result = select(nfds, &testfds, (fd_set *)0, 
            (fd_set *)0, (struct timeval *)0);
      if (result <1)
      {
         perror("server");
         return 1;
      }

      for (fd = 0; fd < nfds; fd++)
      {
         if (FD_ISSET(fd, &testfds))
         {
            if (fd == server_sockfd)
            {
               c_data.ci.chat_flag = 0;
               c_data.ci.client_len = sizeof(c_data.ci.client_address);
               c_data.ci.client_fd = accept(server_sockfd, 
                     (struct sockaddr *)&(c_data.ci.client_address), 
                        &(c_data.ci.client_len));
               FD_SET(c_data.ci.client_fd, &readfds);
               nfds++;
               list_send(&global_data, c_data.ci.client_fd);
               linkedlist_add(&(global_data.c_list), &c_data, 0);
            }
            else
            {
               ioctl(fd, FIONREAD, &nread);

               if (nread == 0)
               {
                  flag_chat = chat_flag_search(&global_data, fd);
                  if (linkedlist_delete(&(global_data.c_list), &fd, 0, 4) == -1)
                     printf("error : fd is already deleted!\n");
                  else
                  {
                     if (flag_chat == 1)
                        chat_quit(&global_data, fd);

                     close(fd);
                     FD_CLR(fd, &readfds);

                     printf("[%d]가 퇴장하였습니다.\n", fd);
                  }
               }
               else
               {
                  read(fd, buf, sizeof(buf));
                  sscanf(buf, "%d|%[^\n]", &flag_recv, buf);
                  printf("buf - %s\n",buf);
                  switch (flag_recv)
                  {
                     case 0:  //채팅 메세지 일때
                        msg_send(&global_data, fd, buf);
                        break; 
                     case 1:  //대화명 recv하였을 때
                        check_id = id_recv(&global_data, buf, fd);
                        if (check_id == -1)
                        {
                           sprintf(buf, "%d|%s", 1, "DuplicateID");
                           write(fd, buf, sizeof(buf));
                        }
                        else
                        {
                           sprintf(buf, "%d|%s", 1, "success");
                           write(fd, buf, sizeof(buf));
                        }
                        break;
                     case 2:  //Client에서 접속자 목록 요청시
                        list_send(&global_data, fd);
                        break;
                     //요청한 fd를 att_fd[0]에 요청당한 fd를 att_fd[1]에 저장 후 수락대기.
                     case 3:  //Client가 초대 요청시
                        r_data.att_fd[0] = fd;     
                        r_data.att_fd[1] = linkedlist_search(&(global_data.c_list), 
                              buf, 4, 10, 1);
                        flag_chat = chat_flag_search(&global_data, r_data.att_fd[1]);
                        linkedlist_enumerate(&(global_data.r_list), 2);
                        if (r_data.att_fd[1] == -1)
                        {
                           sprintf(buf, "%d|%s", 3, "No user");
                           write(fd, buf, sizeof(buf));
                        }
                        else
                        {
                           if (flag_chat == 0)
                           {
                              linkedlist_add(&(global_data.r_list), &r_data, 2);
                              invite_send(&global_data, r_data.att_fd[1], fd);       
                           }
                           else 
                           {
                              sprintf(buf, "%d|%s", 3, "No! User is already chatting");
                              write(fd, buf, sizeof(buf));
                           }
                        }
                        break;
                     case 4:  //수락/거절 회신받았을 때
                        flag_reply = strncmp(buf, "수락", 4);
                        invite_recv(&global_data, fd, flag_reply);
                        break;
                     case 5:  //Client에서 히스토리 요청시
                        history_call(&global_data, fd);
                        break;
                     case 6:  //Client가 채팅 중 /나가기 입력시
                        chat_quit(&global_data, fd);
                        break;
                  }
               }
            }
         }
      }
   }
} 
