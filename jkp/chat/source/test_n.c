#include <stdio.h>
#include <string.h>
#include <stdlib.h>





int main(void) {
   int i,j;
   char(*list)[10] = (char(*)[10])malloc(sizeof(char)*5*10);

   memset(list, 0, sizeof(char)*5*10);

   for (i = 0; i < 5; i++)
   {
      for (j = 0; j < 10; j++)
      {
         printf("%d ", list[i][j]);
      }
      printf("\n");
   }
   free(list);
   return 0;
}
