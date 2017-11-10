#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

void *thread_function();
void *print_function();

sem_t bin_sem;
sem_t bin_sem2;
int sem_value;

char work_area[1024];

int main()
{
   int res,res2;
   pthread_t a_thread;
   pthread_t b_thread;

   void *thread_result;
   
   
   res = sem_init(&bin_sem,0,0);
   res2 = sem_init(&bin_sem2,0,0);
   if(res != 0 || res2 != 0)
   {
      perror("Semaphore initialization failed");
      exit(EXIT_FAILURE);
   }


   res = pthread_create(&a_thread, NULL, thread_function,NULL);
   res2 = pthread_create(&b_thread, NULL, print_function,NULL);
   if(res != 0 || res2 != 0)
   {
      perror("Thread creation failed");
      exit(EXIT_FAILURE);
   }

   printf("Input lower case. Enter 'end' to finish\n");
   while(strncmp("end", work_area, 3) != 0)
   {
      fgets(work_area, 1024, stdin);
      sem_post(&bin_sem);
   }
  
   res = pthread_join(a_thread, &thread_result);
   res2 = pthread_join(b_thread, &thread_result);

   if(res != 0 || res2 != 0)
   {
      perror("Thread join failed");
      exit(EXIT_FAILURE);
   }
   printf("Thread joined\n");

   sem_destroy(&bin_sem);
   sem_destroy(&bin_sem2);
   exit(EXIT_SUCCESS);
}

void *print_function()
{
    int rd;
    char buf[100]={0};
    sem_wait(&bin_sem2);
    rd = open("out.txt",O_RDONLY);
    while( read(rd,buf,sizeof(buf))>0)
       write(1,buf,strlen(buf));
    printf("\n");
//       sem_wait(&bin_sem2);
    pthread_exit(NULL);
}

void *thread_function()
{
   int wr;
   char text;

   sem_wait(&bin_sem);
   while(strncmp("end",work_area,3) !=0 )
   {
      wr = open("out.txt", O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IXUSR);
      text = work_area[0]-32;
      write(wr,&text,1);
      sem_wait(&bin_sem);
   }
   
   sem_post(&bin_sem2);

   pthread_exit(NULL);
}

