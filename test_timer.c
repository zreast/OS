#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define max 10
#include <pthread.h>
#define NUM_THREADS	10
#include <time.h>

//Global Variable
struct node *head = NULL;
struct node *current = NULL;
int count = 0;
pthread_mutex_t lock;

typedef struct node
{
	int data;
	struct node* next;
}node;

int isEmpty() { //empty is 1
	return head == NULL;
}

int isFull() { //full is 1
	return count == max;
}

node* getNode(int num, node* nxt) {
	node*q = (node*)malloc(sizeof(node));
	q->data = num;
	q->next = nxt;
	return q;
}

int add(int num) {
	node *temp = (struct node*) malloc(sizeof(struct node));
	temp->data = num;

	if (isEmpty()) {
		head = temp;
		head->next = head;
		current = head;
		count++;
		return 0;//Sucess to add
	}
	else {
		if (count == max) {
			printf("It full shit!\n");
            return 1; //It fail to add
		}
		else {
			current->next = temp;
			temp->next = head;
			current = temp;
			count++;
			return 0;
		}
	}
}

int del() {

	if (isEmpty()) {
		printf("Fucking it empty\n");
		return 1;
	}
	else {
		node* del = head;
		head=head->next;
		current->next = head;
		free(del);//Love leak
		count--;
		if (count == 0) {
			head = NULL;
			current = NULL;
		}return 0;
	}
}

void *buffer_append(void *vargp)  //add when not full
{
    clock_t st,en;
    double diff;
    int timeout;
    st = clock();
    srand(time(NULL)); //Get system time
    timeout = (rand()*(int)vargp)%5+1; //Random time out 1-5 sec

    while(isFull()==1){
        en = clock();
        diff = ((double)en-(double)st)/CLOCKS_PER_SEC;
        if((int)diff>=timeout){//Timeout
            printf("thread %d cannot append it time out\n",(int)vargp);
            pthread_exit(NULL);
        }
   }

   pthread_mutex_lock(&lock);
   int tid;
   tid = (int)vargp;
   printf("buffer append, thread #%d!\n", tid);
   add(tid);
   pthread_mutex_unlock(&lock);
   pthread_exit(NULL);
}

void *buffer_remove(void *vargp)
{
    clock_t st,en;
    double diff;
    int timeout;
    st = clock();
    srand(time(NULL));
    timeout = (rand()*(int)vargp)%5+1;
   while(isEmpty()==1){
        en = clock();
        diff = ((double)en-(double)st)/CLOCKS_PER_SEC;
        if((int)diff>=timeout){
            printf("thread %d cannot remove it time out\n",(int)vargp);
            pthread_exit(NULL);
        }
   }
   pthread_mutex_lock(&lock);
   int tid;
   tid = (int)vargp;
   printf("buffer remove, thread #%d!\n", tid);
   del();
   pthread_mutex_unlock(&lock);
   pthread_exit(NULL);
}

int main(void) {

  //    add(1);
  //  add(2);
  //  add(3);add(4);add(5);
  //  add(6);

    if (pthread_mutex_init(&lock, NULL) != 0)
        {
            printf("\n mutex init failed\n");
            return 1;
        }


   pthread_t threads[NUM_THREADS];
   int rc;
   int t;
   for(t=0;t<NUM_THREADS;t++){
     printf("In main: creating thread %ld\n", t);
     rc = pthread_create(&threads[t], NULL, buffer_append, (void *)t);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

    for(t=NUM_THREADS;t<NUM_THREADS+NUM_THREADS;t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, buffer_remove, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
            }
    }


   pthread_exit(NULL);
   return 0;
	//printf("%d", current->next->data);

}
