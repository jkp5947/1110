#include "chat_server.h"

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
