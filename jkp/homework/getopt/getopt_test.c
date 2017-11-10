#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	int opt;
	
	while((opt = getopt(argc,argv,":i:p:h")) != -1)
	{
		switch(opt)
		{
		case 'i':
			printf("IP address : %s\n",optarg);
			break;
		case 'p':
			printf("Port number : %s\n",optarg);
			break;
		case 'h':
			printf("-i 뒤에 아이피 주소를 -p 뒤에 포트번호를 기입하세요.\n");
			break;
		case ':':
			printf("option needs a value\n");
			break;
		case '?':
			printf("unknown option : %c\n", optopt);
			break;
		}
	}
	exit(0);

}
