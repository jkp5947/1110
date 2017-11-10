#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

void cpwrite(int flag, int in, int out, char *buf,int ofs)
{
   int copy;

   if(flag!=0)
      lseek(in,ofs,SEEK_SET);
   while((copy = read(in,buf,sizeof(buf)))>0)
      write(out,buf,copy);
}

int main(int argc, char *argv[])
{
   int opt;
   int in,out;

   char buf[1024];
   char *p;
   int ofs=0;
   int copy;
   int cpflag=0,wrflag=0;

   if(argc < 2)
      printf("-i 입력파일 -o 출력파일 [ -p 복사시작 위치 ]  [ -f ]\n");

   while((opt=getopt(argc,argv,":i:o:p:hf"))  != -1)
   {
      switch(opt)
      {
      case 'i':
         in = open(optarg, O_RDONLY);
         break;
      case 'o':
         p=optarg; 
         break;
      case 'p':
         cpflag=1; //-p 옵션의 유무를 파악하기 위함
         ofs =atoi(optarg); //입력받은 위치값의 문자열을 정수로 변환.
         break;
      case 'f':
         wrflag=1; //파일을 복사시 덮어 쓰기위한 flag 
         break;
      case ':':
         printf("파일 명을 입력 하세요!\n");
         break;
      case '?':
         printf("unknown option : %c\n",optopt);
         break;
      case 'h':
         printf("-i 입력파일 -o 출력파일 [ -p 복사시작 위치 ]  [ -f ]\n");
         break;
      }
   }

   if(wrflag==0) // option -f 가 입력되지 않았을 경우
      out = open(p, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);//파일생성시 존재하는 파일이면 파일끝에 내용을 붙여넣는 옵션
   else          // option -f 가 입력되었을 때
      out = open(p,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);// O_TRUNC 옵션을 통해 파일내용을 지운후 다시 작성한다.

   cpwrite(cpflag,in,out,buf,ofs);

   exit(0);
}
