#include "chat_server.h"

//linked list 초기화 
void linkedlist_init(List *list)
{
   list->head = (Node *)malloc(sizeof(Node));
   list->tail = (Node *)malloc(sizeof(Node));
   list->head->next = list->tail;
   list->tail->next = list->tail;
}

//linekd list 추가 flag를 통해 구조체 선택
void linkedlist_add(List *list, void *struct_data, int choice_flag)
{
   if(list->head== 0)
   {
      printf("error : list is empty\n");
      return ;
   }
   Node *nw = (Node *)malloc(sizeof(Node));
   Node *front;

   Client_data *c_nw_data;
   History *h_nw_data;
   Room *r_nw_data;

   if (choice_flag == 0)
   {
      c_nw_data = (Client_data *)malloc(sizeof(Client_data));
      *c_nw_data = *(Client_data *)struct_data;
      nw->pData =c_nw_data;
      memset(((Client_data *)nw->pData)->ci.client_id, 0x00, 10);
   }
   else if (choice_flag == 1)
   {
      h_nw_data = (History *)malloc(sizeof(History));
      *h_nw_data = *(History *)struct_data;
      nw->pData =h_nw_data;

   }
   else if (choice_flag == 2)
   {
      r_nw_data = (Room *)malloc(sizeof(Room));
      *r_nw_data = *(Room *)struct_data;
      nw->pData =r_nw_data;

   }
   nw->next = list->tail;

   front = list->head;
   while (front->next != list->tail)
   {
      front = front->next;
   }
   front->next = nw;
   printf("ADD SUCCESS\n");
}

//linked list 에서 원하는 데이터 찾기
int linkedlist_search(List *list, void *data, long ofs, int len, int return_flag)
{  
   Node *eye;
   int index = 1;

   if(list->head == 0)
   {
      printf("error : list is empty\n");
      return -1;
   }
 
   eye = list->head->next;

   while (eye != list->tail)
   {   
      if (memcmp((char *)(eye->pData) + ofs, data, len) == 0)
      {   
         printf("FOUND\n");
         if (return_flag == 0)
            return index;
         else 
            return *((int *)(eye-> pData));
      }   
      eye = eye -> next;
      index++;
   }   
   printf("NOT FOUND\n");
   return -1;
}

//linked list 에서 원하는 데이터가 있는 노드 삭제
int linkedlist_delete(List *list, void *find, long ofs, int len)
{
   if(list->head == 0)
   {
      printf("error : list is empty\n");
      return -1;
   }
   Node *chk, *help;

   help = list->head;
   chk = list->head->next;

   while (chk != list->tail)
   {
      if (memcmp((char *)(chk->pData) + ofs, find, len) == 0)
      {
         help->next = chk->next;
         free(chk->pData);
         free(chk);
         return 0;
      }
      help = help->next;
      chk = chk->next;
   }
   return -1;
}



//linked list 삭제 
void linkedlist_destroy(List *list)
{
   if(list->head == 0)
   {
      printf("error : list is empty\n");
      return ;
   }
   Node *kill, *help;
   kill = list->head->next;
   while (kill != list->tail)
   {
      help = kill->next;
      free(kill->pData);
      free(kill);
      kill = help;
   }
   list->head = 0;
   list->tail = 0;
   free(list->head);
   free(kill);
   
}

//linked list 열거
void linkedlist_enumerate(List *list,int struct_flag)
{
   if(list->head == 0)
   {
      printf("error : list is empty\n");
      return ;
   }

   Node *eye;
   eye = list->head->next;
   if (struct_flag == 0)
   {
      while(eye != list->tail)
      {
         printf("fd  : %-5d ",((Client_data *)eye->pData)->ci.client_fd);
         printf("id  : %-10s",((Client_data *)eye->pData)->ci.client_id);
         printf("flag  : %-5d\n",((Client_data *)eye->pData)->ci.chat_flag);
         eye = eye->next;
      }
   }
   else if (struct_flag == 1)
   {
      while (eye != list->tail)
      {
         printf("msg : %s\n",((History *)eye->pData)->msg);
         eye = eye->next;
      }
   }
   else if (struct_flag == 2)
   {
      while (eye != list->tail)
      {
         printf("att_fd_1 : %d ",((Room *)eye->pData)->att_fd[0]);
         printf("att_fd_2 : %d\n",((Room *)eye->pData)->att_fd[1]);
         eye = eye->next;
      }
   }
}
