#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
   int opt;
   FILE *fp;
   
   if(argc<2)
      printf("-r <삭제파일> -i(파일 삭제 시간)\n");

   while((opt=getopt(argc,argv,":r:ih"))!= -1)
   {
      switch(opt)
      {
      case 'r':
         unlink(optarg);
         break;
      case 'i':
         fp= fopen("delete_files.log","w");
         system("date > delete_files.log");	
         break;
      case ':':
         printf("unknown file name!\n");
         break;
      case '?':
         printf("unknown option : %c\n",optopt);
         break;
      case 'h':
         printf("-r <삭제파일> -i(파일 삭제 시간)\n");
         break;
      }
   }
}

