/*
Operating System Assignment

Computer Engineering KMITL
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int buffer[1000],N;

void *testThread(void *vargp)
{
  sleep(1);
  printf("This is thread\n");
  return NULL;
}

int main(void)
{
  pthread_t tid;
  printf("t35t th23ad\n");
  pthread_create(&tid , NULL , testThread , NULL);
  pthread_join(tid , NULL);
  printf("Exit thread\n");
  return 0;
}
