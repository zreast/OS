/*
Operating System Assignment

Computer Engineering KMITL
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 1000

//buffer counter
int buffer_counter = 0;
int buffer[BUFFER_SIZE];
int producer,consumer,size,request;

pthread_mutex_t mutex;
sem_t full,empty;

pthread_t tid;
pthread_attr_t attr;


void *buffer_append(void *vargp)
{
  return NULL;
}

void *buffer_remove(void *vargp)
{
  return NULL;
}


int main(void)
{
  int i;

  scanf("%d %d %d %d",&producer,&consumer,&size,&request);

  //init mutex lock
  pthread_mutex_init(&mutex, NULL);
  //init full semaphore to 0 , empty to buffer_size
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, BUFFER_SIZE);

  pthread_attr_init(&attr);


  for(i=1 ; i<=request ; i++)
  {
    if(cmd==0)
    {
      if(buffer_size > 1000)
      {
        // wait
      }
      pthread_create(&tid , NULL , buffer_append , (void *)i);
    }
    else
    {
      if(buffer_size == 0)
      {
        // wait
      }
      pthread_create(&tid , NULL , buffer_remove , (void *)i);
    }
  }

  return 0;
}
