#include "msg_com.h"

int run = 1;

void *receiver(void *arg);

int main(void)
{
   struct msg_st send_data;
   int msgid;
   char buf[BUFSIZ];
   pthread_t t_receiver;

   
   pthread_create(&t_receiver, NULL, receiver, NULL);
   msgid = msgget((key_t)5678, 0777|IPC_CREAT);

   if(msgid == -1)
   {
      fprintf(stderr, "msgget failed with error : %d\n",errno);
      exit(1);
   }

   printf("Enter some data!!\n");
   while(run)
   {
      fgets(buf,BUFSIZ,stdin);
      send_data.msg_type = 2;
      strcpy(send_data.data, buf);

      if(msgsnd(msgid, (void *)&send_data, MAX_TEXT, 0) == -1)
      {
         fprintf(stderr, "msgsnd failed\n");
         exit(1);
      }
      if (strncmp(buf, "end", 3)==0)
         run = 0;
   }

   exit(0);
}

void *receiver(void *arg)
{
   int msgid;
   struct msg_st recv_data;
   char buf[BUFSIZ];

   msgid = msgget((key_t)1234, 0777 | IPC_CREAT);
   if(msgid == -1)
   {
      fprintf(stderr, "msgget failed with error : %d\n",errno);
      exit(1);
   }

   while(run)
   {
      if(msgrcv(msgid, (void *)&recv_data, BUFSIZ, 1,0) == -1)
      {
         fprintf(stderr, "msgrcv failed with error: %d\n",errno);
         exit(1);
      }
      printf("rcv data : %s", recv_data.data);
   }
   if (msgctl(msgid, IPC_RMID, 0) == -1)
   {
      fprintf(stderr, "msgctl(IPC_RMID) failed\n");
      exit(1);
   }
   
}
