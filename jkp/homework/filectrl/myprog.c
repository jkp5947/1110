#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
   int opt;
   int in,out;

   char buf[1024];
   int copy;

   if(argc<2)
      printf("-i<입력파일> -o<출력파일> -h(help)\n");//인자가 없을 경우

   while((opt=getopt(argc,argv,":i:o:h"))  != -1)
   {
      switch(opt)
      {
         case 'i':
            in = open(optarg, O_RDONLY);
            break;
         case 'o':
            out = open(optarg, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            while((copy = read(in,buf,sizeof(buf))) > 0)
            write(out,buf,copy);
            break;
         case 'h':
            printf("-i<입력파일> -o<출력파일> -h(help)\n");//help!
            break;
         case '?':
            printf("unknown option : %c\n",optopt);
            printf("-i<입력파일> -o<출력파일> -h(help)\n");//잘못된 옵션이 입력 되었을 경우 
            break;
         case ':':
            printf("파일 명을 입력 하세요!\n");
            break;
      }
    
   }

   exit(0);

}
