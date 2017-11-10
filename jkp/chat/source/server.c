#include "chat_server.h"


void send_msg(jkp_global *g_data, int fd, char *buf)
{
   int ofs, i;
   int recv_fd;
   int id_index, h_index;
   char id[10];
   char msg[BUFSIZ];
   Node *eye;
   ofs = OFFSET(Room, att_fd[1]);
   recv_fd = linkedlist_search(&(g_data->r_list), &fd, ofs, 4, 1);
   id_index = linkedlist_search(&(g_data->c_list), &fd, 0, 4, 0);
   eye = g_data->c_list.head->next;
   for (i = 1; i < id_index; i++)
      eye = eye->next;
   strcpy(id, ((Client_data *)eye->pData)->ci.client_id);
   sprintf(msg, "[%s]%s", id, buf);

   h_index = linkedlist_search(&(g_data->r_list), &fd, ofs, 4, 0);
   eye = g_data->r_list.head->next;
   for(i = 1; i < h_index; i++)
      eye = eye->next;
   linkedlist_add(&(((Room *)eye->pData)->h_list), msg, 1);

   sprintf(msg, "%d[%s]%s", 0, id, buf);
   write(recv_fd, msg, sizeof(msg));

   return ;
}

int recv_id(jkp_global *g_data, char *id, int fd)
{
   int index, find, i;
   long ofs;
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

void send_list(jkp_global *g_data, int request_fd)
{
   Node *eye;
   char buf[11]={0};
   eye = g_data->c_list.head->next;

   if (eye == g_data->c_list.tail)
   {
      sprintf(buf, "%d|%s", 2, "No list");
      write(request_fd, buf, sizeof(buf));
   }
   while (eye != g_data->c_list.tail)
   {
      sprintf(buf, "%d|%s", 2, ((Client_data *)eye->pData)->ci.client_id);
      write(request_fd, buf, 11);    
      eye = eye->next;
   }
   return ;
}

void create_room(jkp_global *g_data, int index)
{
   Node *eye;
   int i;
   eye = g_data->r_list.head->next;
   for (i = 1; i < index; i++)
      eye = eye->next;
   linkedlist_init(&(((Room *)eye->pData)->h_list));
}

void send_inv(jkp_global *g_data, int inv_fd, int request_fd)
{
   Node *eye;
   char id[10]={0};
   char buf[BUFSIZ]={0};
   eye = g_data->c_list.head->next;

   while (eye != g_data->c_list.tail)
   {
      if (memcmp((char *)(eye->pData), &request_fd, 4) == 0)
      {
         memcpy(id, &(((Client_data *)eye->pData)->ci.client_fd), 10); 
         break;
      }
      eye = eye->next;
   }
   sprintf(buf, "[%s]님이 초대하셨습니다. 수락하시겠습니까? (Y/N)",id);
   write(inv_fd, buf, sizeof(buf));
}

void call_history(jkp_global *g_data, int request_fd)
{
   int index;
   int i;
   char err[10];
   char message[1024];
   Node *eye, *find;
   

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
   eye = g_data->r_list.head->next;
   for (i = 1; i < index; i++)
      eye = eye->next;
   find = ((Room *)eye->pData)->h_list.head->next;
   if (find == ((Room *)eye->pData)->h_list.tail)
   {
      sprintf(err, "%d%s", 5, "no msg");
      write(request_fd, err, 10);
   } 
   else
   {
      while(find != ((Room *)eye->pData)->h_list.tail)
      {
         sprintf(message, "%d%s", 5, ((History *)find->pData)->msg);
         find = find->next;
      }
   }   
}

void exit_client(jkp_global *g_data, int exit_client_fd)
{
   Node *eye;
   int id_index;
   int i;
   char id[10];
   int flag;
   id_index = linkedlist_search(&(g_data->c_list), &exit_client_fd, 0, 4, 0);
   
   eye = g_data->c_list.head->next;
   for (i = 1; i < id_index; i++)
      eye = eye->next;
   memcpy(id, ((Client_data *)eye->pData)->ci.client_id, 10);

   flag = linkedlist_delete(&(g_data->c_list), &exit_client_fd, 0, 4);
   if (flag == 0)
      printf("[%s]가 퇴장하였습니다.\n", id);
   else
      printf("삭제 실패\n");
}

int main(void)
{
   int server_sockfd;
   int server_len;
   struct sockaddr_in server_address;
   int result;
   int nfds = 4;
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
      char buf[BUFSIZ];
      int fd;
      int recv_flag;
      int inv_fd;
      int reply_flag; 
      int index, ofs;
      testfds = readfds;

      printf("server waiting\n");

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
               c_data.ci.client_len = sizeof(c_data.ci.client_address);
               c_data.ci.client_fd = accept(server_sockfd, (struct sockaddr *)&(c_data.ci.client_address), &(c_data.ci.client_len));
               FD_SET(c_data.ci.client_fd, &readfds);
               nfds++;
               linkedlist_add(&(global_data.c_list), &c_data, 0);
               send_list(&global_data, c_data.ci.client_fd);
            }
            else
            {
               read(fd, buf, sizeof(buf));
               printf("buf = %s\n",buf);
               sscanf(buf, "%d|%[^\n]", &recv_flag, buf);
               switch (recv_flag)
               {
                  case 0:
                     send_msg(&global_data, fd, buf);
                     break; 
                  case 1:
                     printf("id = %s\n",buf);
                     reply_flag = recv_id(&global_data, buf, fd);
                     if (reply_flag == -1)
                     {
                        sprintf(buf, "%d|%s", 1, "Duplicate ID");
                        write(fd, buf, sizeof(buf));
                     }
                     else
                     {
                        sprintf(buf, "%d|%s", 1, "success");
                        write(fd, buf, sizeof(buf));
                     }
                     break;
                  case 2: 
                     send_list(&global_data, fd);
                     break;
                  case 3: 
                     r_data.att_fd[1] = linkedlist_search(&(global_data.c_list), 
                           buf, 0, 4, 1);
                     if (r_data.att_fd[1] == -1)
                     {
                        sprintf(buf, "%d%s", 3, "No user");
                        write(fd, buf, sizeof(buf));
                     }
                     else
                     {
                        r_data.att_fd[0] = fd;
                        linkedlist_add(&(global_data.r_list), &r_data, 2);
                        send_inv(&global_data, r_data.att_fd[1], fd);       
                     }
                     break;
                  case 4:
                     reply_flag = strncmp(buf, "수락", sizeof("수락"));
                     if (reply_flag == 0) //수락시
                     {
                        ofs = OFFSET(Room, att_fd[1]);
                        index = linkedlist_search(&(global_data.r_list), &fd, ofs, 4, 0);
                        create_room(&global_data, index);
                        printf("[%d], [%d]의 채팅이 시작되었습니다.\n", 
                              linkedlist_search(&(global_data.r_list), &fd, ofs, 4, 1), fd);
                     }
                     else 
                     {
                        inv_fd = linkedlist_search(&(global_data.r_list), &fd, 4, 4, 1);
                        sprintf(buf, "%d%s", 4, "거절하였습니다");
                        write(inv_fd, buf, sizeof(buf));
                        linkedlist_delete(&(global_data.r_list), &inv_fd, 0, 4);
                     }
                     break;
                  case 5:
                     call_history(&global_data, fd);
                     break;
                  case 6:
                     exit_client(&global_data, fd);

               }
            }
         }
      }
   }
} 
