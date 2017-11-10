#include "chat_server.h"

int main(void) {

   int i = 0;
   long ret = 0;
   char id[10] = "ijkl";
   char id2[10] = "fghi";

   linkedlist_enumerate(&(global_data.c_list),0);//열거
   Client_data first;
   first.ci.client_fd = 5;
   strncpy(first.ci.client_id, "abcd",10);
   first.ci.client_len = 4;
   first.chat_flag = 0;

   Client_data third;
   third.ci.client_fd = 100;
   strncpy(third.ci.client_id, "ijkl", 10);
   third.ci.client_len = 4;
   third.chat_flag =0;

   linkedlist_init(&(global_data.c_list));

   linkedlist_add(&(global_data.c_list), &first, 0);

   first.ci.client_fd = 10;
   strncpy(first.ci.client_id, "fghi", 10);
   first.ci.client_len = 4;
   first.chat_flag = 0;

   linkedlist_enumerate(&(global_data.c_list),0);//열거
   linkedlist_add(&(global_data.c_list), &first, 0);
   linkedlist_add(&(global_data.c_list), &third, 0);
   linkedlist_add(&(global_data.c_list), &first, 0);
   linkedlist_add(&(global_data.c_list), &first, 0);
   linkedlist_add(&(global_data.c_list), &first, 0);
   printf("add finish\n");
   linkedlist_enumerate(&(global_data.c_list),0);//열거
   ret = OFFSET(Client_data, ci.client_id);
   printf("client_id offset : %ld\n", ret);

   //(찾고자하는 대상의 list, 찾고자하는 것, offset, 비교할길이)
   i=linkedlist_search(&(global_data.c_list),id, ret, 10);
   printf("find %s, return fd = %d\n",id, i);
   i=linkedlist_search(&(global_data.c_list),id2, ret, 10);
   printf("find %s, return fd = %d\n",id2, i);

   //(찾고자하는 대상의 list, 찾아서 지울 node의 자료, offset, 비교길이)
   i = linkedlist_delete(&(global_data.c_list), &(third.ci.client_fd), 0, 4);
   if (i == 0)
      printf("find & delete success!\n");
   else if (i == 1)
      printf("find fail!\n");
   else 
      printf("error!\n");
    
   linkedlist_enumerate(&(global_data.c_list),0);//열거

   //(모든 노드를 지울 list)
   linkedlist_destroy(&(global_data.c_list));
   printf("destroy!\n");

   linkedlist_enumerate(&(global_data.c_list),0);//열거

   return 0;
}

