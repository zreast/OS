/*
Operating System Assignment

Computer Engineering KMITL
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int buffer[1000],N,g;

void *testThread(void *vargp)
{
  char *str = (char*)vargp;
  int id = (int)vargp;
  static int s = 0;
  s++; g++;

  printf("test : %s Thread : %d Static: %d Global: %d\n",str,id,++s,++g);
  return NULL;
}

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
  printf("t35t mult1th23ad\n");
  for(i=1;i<=20;i++)
  {
    pthread_create(&tid , NULL , testThread , (void *)i);
  }
  printf("Exit thread\n");
  return 0;
}
