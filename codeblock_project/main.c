#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define max 3
#include <pthread.h>
#define NUM_THREADS	10
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

int isEmpty() {
	return head == NULL;
}

int isFull() {
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
		return 0;
	}
	else {
		if (count == max) {
			printf("It full shit!\n");
            return 1;
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
		free(del);
		count--;
		if (count == 0) {
			head = NULL;
			current = NULL;
		}return 0;
	}
}

void *buffer_append(void *vargp)  //add when not full
{
   while(isFull()==1){}
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
   while(isEmpty()==1){}
   pthread_mutex_lock(&lock);
   int tid;
   tid = (int)vargp;
   printf("buffer remove, thread #%d!\n", tid);
   del();
   pthread_mutex_unlock(&lock);
   pthread_exit(NULL);
}

int main(void) {
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
     for(t=NUM_THREADS;t<NUM_THREADS+10;t++){
     printf("In main: creating thread %ld\n", t);
     rc = pthread_create(&threads[t], NULL, buffer_remove, (void *)t);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

   /* Last thing that main() should do */
   pthread_exit(NULL);
   return 0;

	//printf("%d", current->next->data);

}

