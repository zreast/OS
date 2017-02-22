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
  int id = (int)vargp;
  static int s = 0;
  s++; g++;

  printf("Thread : %d Static: %d Global: %d\n",id,++s,++g);
  return NULL;
}

// void *append(void *vargp)
// {
//   return NULL;
// }
//
// void *remove(void *vargp)
// {
//   return NULL;
// }


int main(void)
{
  int i;
  pthread_t tid;
  printf("t35t mult1th23ad\n");
  for(i=1;i<=7;i++)
  {
    pthread_create(&tid , NULL , testThread , (void *)i);
  }
  printf("Exit thread\n");
  return 0;
}
