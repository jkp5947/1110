#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
   pid_t pid=fork();

   if(pid==0)
      puts("This is the child");
   else
   {
      printf("child process id : %d \n",pid);
      sleep(30);
   }
   if(pid==0)
      puts("End child process");
   else 
      puts("end parent process");
   return 0;
}
