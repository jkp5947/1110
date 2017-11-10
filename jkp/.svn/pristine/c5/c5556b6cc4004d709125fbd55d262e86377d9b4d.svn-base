#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


#define WORK_SIZE 1024
#define FSTR "UNIX"

int main(void)
{
   FILE *_read;
   int rd,i,j;
   char *p;
   char *f_str;
   char buf[WORK_SIZE]={'\0'};

   f_str=FSTR;

   _read = popen("cat test.txt","r");
   if(_read!=NULL)
   {
      rd = fread(buf,sizeof(char), WORK_SIZE, _read);
      while( rd >0 ){
         buf[rd-1]='\0';

         if((p=strstr(buf,f_str))!=0){
            printf("%d\n",(int)(p-buf));
            for (j=p-buf-10;j<p-buf+20;j++)
            {
               printf("%c",buf[j]);
            }
            printf("\n");
         }
         rd = fread(buf,sizeof(char),WORK_SIZE,_read);
      }
      pclose(_read);
      exit(0);
   }
   else{
      printf("NOT FOUND\n");
      exit(1);
   }
}
