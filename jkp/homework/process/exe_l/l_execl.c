#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

   printf("Running ls with execl\n");
   execl("/bin/ls","ls","-l",(char *)NULL);
   printf("Done\n");
                      
   return 0;
                        
}
