#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *odd_func();
void *even_func();

pthread_mutex_t num_mutex;

int buf;
int flag=0;

int main()
{
   pthread_t odd_th,even_th;
   void *thread_result;

   pthread_mutex_init(&num_mutex, NULL);

   pthread_create(&odd_th, NULL, odd_func, NULL);
   pthread_create(&even_th, NULL, even_func, NULL);
   pthread_mutex_lock(&num_mutex);
   printf("Input number. Enter 999 to finish\n");
   while(!flag)
   {
      scanf("%d",&buf);
      pthread_mutex_unlock(&num_mutex);
      printf("main mutex unlock\n");
      while(1)
      {
         pthread_mutex_lock(&num_mutex);
         if(buf != 0)
         {
            pthread_mutex_unlock(&num_mutex);
            sleep(1);
         }
         else
            break;
      }
   } 
   pthread_mutex_unlock(&num_mutex);
   printf("\nWaiting for thread to finish...\n");
   
   pthread_join(odd_th, &thread_result);
   pthread_join(even_th, &thread_result);
   printf("thread joined\n");

   pthread_mutex_destroy(&num_mutex);
   exit(0);

}
void *odd_func()
{
   sleep(1);
   pthread_mutex_lock(&num_mutex);
   {
      while(buf != 999)
      {
         if(buf%2==1 && buf !=0)
         {
            printf("odd number! %d + 1 = %d\n",buf, buf+1);
            buf = 0;
            pthread_mutex_unlock(&num_mutex);
            sleep(1);
            pthread_mutex_lock(&num_mutex);
            while(buf == 0)
            {
               pthread_mutex_unlock(&num_mutex);
               sleep(1);
               pthread_mutex_lock(&num_mutex);
            }
            buf = 0;
            pthread_mutex_unlock(&num_mutex);
         }
         else
         {
            pthread_mutex_unlock(&num_mutex);
         }
      }
   }
   flag = 1;
   pthread_exit(0);
}

void *even_func()
{

   sleep(1);
   pthread_mutex_lock(&num_mutex);
   while(buf != 999)
   {
      if(buf%2==0 && buf !=0)
      {
         printf("even number! %d + 1 = %d\n",buf, buf+1);
         buf = 0;
         pthread_mutex_unlock(&num_mutex);
         sleep(1);
         pthread_mutex_lock(&num_mutex);
         while(buf == 0)
         {
            pthread_mutex_unlock(&num_mutex);
            sleep(1);
            pthread_mutex_lock(&num_mutex);
         }
         buf = 0;
         pthread_mutex_unlock(&num_mutex);
      }
      else
      {
         pthread_mutex_unlock(&num_mutex);
      }
   }
   flag = 1;
   pthread_exit(0);

}
