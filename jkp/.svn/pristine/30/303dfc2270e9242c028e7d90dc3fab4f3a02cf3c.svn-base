#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
int main()
{
   char *const ls_argv[]={"ls","-l",(char *)NULL};
   printf("Running ls with execv\n");
   execv("/bin/ls",ls_argv);
   printf("Done.\n");

   return 0;
}
