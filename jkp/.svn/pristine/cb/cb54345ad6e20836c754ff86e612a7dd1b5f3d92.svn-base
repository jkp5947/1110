#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
   char command[100];
   int i=2;
   int exit_code;
   int opt;
   pid_t pid;
   pid=fork();
   
   while((opt = getopt(argc, argv,"h")) != -1)
   {
      if(opt = 'h')
      {
         printf("./ioexe [명령어]\n");
         exit(1); 
      }
   }
   if(argc<2)
      printf("명령어를 입력하세요\n");
   else
   {
      switch(pid)
      {
         case -1:
            perror("fork failed");
            exit(1);
         case 0:
            strcpy(command,argv[1]);
      
            while(argv[i])
            {
               strcat(command," ");
               strcat(command,argv[i]);
               i++;
            }
            
            system(command);
            exit_code=30;
            break;
         case 1: 
            exit_code=0;
            break;
      }
      if (pid!=0)
      {
         int stat_val;
         pid_t child_pid;

         child_pid = wait(&stat_val);

         printf("Child has finished : PID = %d\n",child_pid);
         if(WIFEXITED(stat_val))
            printf("Child exited wit code %d\n",WEXITSTATUS(stat_val));
         else
            printf("Child terminated abnormally\n");
      }
      exit(exit_code);
   }
}
