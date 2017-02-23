#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<windows.h>

//Global Variable
struct node *head = NULL;
struct node *current = NULL;
int count = 0;
int append_count = 0;
int remove_count = 0;
int error_count = 0;
volatile int running_threads = 0;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t append_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t remove_mutex = PTHREAD_MUTEX_INITIALIZER;

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

int isProducerFull(){
    return append_count == producer;
}

int isConsumerFull(){
    return remove_count == consumer;
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
			//printf("Buffer OVERFLOW\n");
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
		//printf("Buffer UNDERFLOW\n");
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
    clock_t st,en;
    double diff;
    int timeout;

    pthread_mutex_lock(&append_mutex);
    append_count++;
    pthread_mutex_unlock(&append_mutex);
    st = clock();
    srand(time(NULL)); //Get system time
    timeout = (rand()*(int)vargp)%5+1; //Random time out 1-5 sec

    while(isFull()==1){
        en = clock();
        diff = ((double)en-(double)st)/CLOCKS_PER_SEC;
        if((int)diff>=timeout){//Timeout
            printf("thread %d cannot append it time out\n",(int)vargp);
            pthread_mutex_lock(&running_mutex);
                error_count++;
                running_threads--;
            pthread_mutex_unlock(&running_mutex);
            pthread_exit(NULL);
        }
   }

   pthread_mutex_lock(&lock);
   int tid;
   tid = (int)vargp;
   //printf("buffer append, thread #%d!\n", tid);
   add(tid);
   pthread_mutex_lock(&append_mutex);
        append_count--;
   pthread_mutex_unlock(&append_mutex);
   pthread_mutex_lock(&running_mutex);
        running_threads--;
   pthread_mutex_unlock(&running_mutex);
   pthread_mutex_unlock(&lock);
   pthread_exit(NULL);
}

void *buffer_remove(void *vargp)
{
    clock_t st,en;
    double diff;
    int timeout;

    pthread_mutex_lock(&remove_mutex);
    remove_count++;
    pthread_mutex_unlock(&remove_mutex);
    st = clock();
    srand(time(NULL));
    timeout = (rand()*(int)vargp)%5+1;
   while(isEmpty()==1){
        en = clock();
        diff = ((double)en-(double)st)/CLOCKS_PER_SEC;
        if((int)diff>=timeout){
            printf("thread %d cannot remove it time out\n",(int)vargp);
            pthread_mutex_lock(&running_mutex);
                running_threads--;
                error_count++;
            pthread_mutex_unlock(&running_mutex);
            pthread_exit(NULL);
        }
   }
   pthread_mutex_lock(&lock);
   int tid;
   tid = (int)vargp;
   //printf("buffer remove, thread #%d!\n", tid);
   del();
    pthread_mutex_lock(&remove_mutex);
       remove_count--;
    pthread_mutex_unlock(&remove_mutex);
    pthread_mutex_lock(&running_mutex);
          running_threads--;
   pthread_mutex_unlock(&running_mutex);
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

  pthread_t threads[producer+consumer];
  int rc;
  int t,i;

    srand(time(NULL));
	//Start time
	initial=clock();
    add(1);
    add(2);
    add(3);
    add(4);
    add(5);
    add(6);

    /*for(i=0;i<request;i++)
    {
        if(rand()%2){
            while(isProducerFull()){}
            //printf("Append\n");
            pthread_mutex_lock(&running_mutex);
            running_threads++;
            pthread_mutex_unlock(&running_mutex);
            rc = pthread_create(&threads[append_count], NULL, buffer_append, (void *)append_count);

            if (rc){
              printf("ERROR; return code from pthread_create() is %d\n", rc);
              exit(-1);
            }
        }
        else{
            while(isConsumerFull()){}
            //printf("Remove\n");
            pthread_mutex_lock(&running_mutex);
            running_threads++;
            pthread_mutex_unlock(&running_mutex);
            rc = pthread_create(&threads[producer+remove_count], NULL, buffer_remove, (void *)remove_count);

            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
    }*/

    for(i=0;i<20;i++){
        while(isProducerFull()){}
            //printf("Append\n");
            pthread_mutex_lock(&running_mutex);
            running_threads++;
            pthread_mutex_unlock(&running_mutex);
            rc = pthread_create(&threads[append_count], NULL, buffer_append, (void *)append_count);

            if (rc){
              printf("ERROR; return code from pthread_create() is %d\n", rc);
              exit(-1);
            }
    }
    for(i=0;i<30;i++){
        while(isConsumerFull()){}
            //printf("Remove\n");
            pthread_mutex_lock(&running_mutex);
            running_threads++;
            pthread_mutex_unlock(&running_mutex);
            rc = pthread_create(&threads[producer+remove_count], NULL, buffer_remove, (void *)remove_count);

            if (rc){
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
    }

    while(running_threads>0){
        Sleep(1);
    }



	//Stop time
	final=clock();
	temp_cpu = ((double)final-(double)initial) / CLOCKS_PER_SEC ;



	//Outputs
	printf("Error count = %d\n",error_count);
	printf("\nSuccessfully consumed %d requests (%.2f%%)\n",request-error_count,((double)(request-error_count)/(double)request)* 100);
	printf("Elapsed Time: %.2f s\n",temp_cpu);
	printf("Throughput: %.2f successful requests/s\n",(double)(request-error_count)/temp_cpu);

  //Last thing that main() should do
  pthread_exit(NULL);
  return 0;

	//printf("%d", current->next->data);

}
