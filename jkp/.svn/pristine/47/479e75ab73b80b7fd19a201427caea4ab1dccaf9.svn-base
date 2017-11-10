#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
int main()
{
   char *const ls_argv[]={"ls","-l",(char *)NULL};
   printf("Running ls with execvp\n");
   execvp("ls", ls_argv);  
   printf("Done.\n");

   return 0;
}
