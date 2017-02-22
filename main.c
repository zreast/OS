/*
Operating System Assignment

Computer Engineering KMITL
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 1000

int buffer[BUFFER_SIZE];
int producer,consumer,size,request;


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
  pthread_t tid;

  scanf("%d %d %d %d",&producer,&consumer,&size,&request);
  buffer_size = size;

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
