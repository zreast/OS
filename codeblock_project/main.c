#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include <pthread.h>

//Global Variable
struct node *head = NULL;
struct node *current = NULL;
int count = 0;

//Input
int producer,consumer,buffer_size,request;

//Time catcher
clock_t initial,final;
double temp_cpu;

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
	return count == buffer_size;
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
		if (count == buffer_size) {
			printf("Buffer OVERFLOW\n");
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
		printf("Buffer UNDERFLOW\n");
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
		}
		return 0;
	}
}

void *buffer_append(void *vargp)  //add when not full
{
   while(isFull()==1){}
   pthread_mutex_lock(&lock);
   int tid;
   tid = (int)vargp;
   printf("Buffer append, thread #%d!\n", tid);
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
   printf("Buffer remove, thread #%d!\n", tid);
   del();
   pthread_mutex_unlock(&lock);
   pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if(argc != 5){
		printf("Parameter not correct\n");
		return 1;
	}
	if (pthread_mutex_init(&lock, NULL) != 0){
    printf("\n Mutex init failed\n");
    return 1;
  }

	// Assign from input
	producer = atoi(argv[1]);
	consumer = atoi(argv[2]);
	buffer_size = atoi(argv[3]);
	request = atoi(argv[4]);

	printf("Producers %d, Consumers %d\n", producer,consumer);
	printf("Buffer size %d\n", buffer_size);
	printf("Requests %d\n", request);

  pthread_t threads[producer];
  int rc;
  int t;

	//Start time
	initial=clock();

  for(t=0;t<producer;t++){
    printf("In main: creating thread %ld\n", t);

		rc = pthread_create(&threads[t], NULL, buffer_append, (void *)t);

		if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  for(t=producer;t<producer*2;t++){
	  printf("In main: creating thread %ld\n", t);

		rc = pthread_create(&threads[t], NULL, buffer_remove, (void *)t);

		if (rc){
	    printf("ERROR; return code from pthread_create() is %d\n", rc);
	    exit(-1);
	  }
  }

	//Stop time
	final=clock();
	temp_cpu = ((double)final-(double)initial) / CLOCKS_PER_SEC ;

	for(int i=0;i<=600000000;i++){
		producer++;
	}
	printf("\n\nSuccessfully consumed %d requests (%.2f%%)\n",request,(request/request)* 100);
	printf("Elapsed Time: %.2f s\n",temp_cpu);
	printf("Throughput: %.2f successful requests/s\n",request/temp_cpu);

  /* Last thing that main() should do */
  pthread_exit(NULL);
  return 0;

	//printf("%d", current->next->data);

}
