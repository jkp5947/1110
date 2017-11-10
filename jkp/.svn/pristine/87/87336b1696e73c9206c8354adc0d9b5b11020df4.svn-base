#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>

#include "com_shm.h"

int main()
{
   int running = 1;
   void *shared_memory = (void *)0;
   struct shared_use_st *shared_stuff;
   char buf[BUFSIZ];
   int shmid;

   shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);

   if(shmid == -1)
   {
      fprintf(stderr, "shmget failed\n");
      exit(EXIT_FAILURE);
   }
   shared_memory = shmat(shmid, (void *)0, 0);
   if(shared_memory == (void *)-1)
   {
      fprintf(stderr, "shmat failed\n");
      exit(EXIT_FAILURE);
   }

   printf(" Memory attached at %p\n",shared_memory);

   shared_stuff = (struct shared_use_st *)shared_memory;

   while(running)
   {
      printf("Enter some text : ");
      fgets(buf, BUFSIZ, stdin);

      strncpy(shared_stuff->some_text, buf,TEXT_SZ);
      shared_stuff -> written_by_you = 1;

      if(strncmp(buf,"end",3) == 0)
      {
         running = 0;
      }
   }

   if(shmdt(shared_memory) == -1)
   {
      fprintf(stderr,"shmdt failed\n");
      exit(EXIT_FAILURE);
   }

   exit(EXIT_SUCCESS);

}
