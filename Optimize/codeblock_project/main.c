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
int error_count = 0, request_temp;
volatile int running_threads = 0;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t append_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t remove_mutex = PTHREAD_MUTEX_INITIALIZER;
int temp_consumer_thread[500]={0};
int temp_producer_thread[500]={0};

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
    do{
    if(request<0)break;
    pthread_mutex_lock(&append_mutex);
    append_count++;
    pthread_mutex_unlock(&append_mutex);
    st = clock();
    //srand(time(NULL)); //Get system time
    //timeout = (rand()*(int)vargp)%100+1; //Random time out 1-5 sec
		timeout = ((int)vargp)%100+100;

    if(isFull()==1){
            while(isFull()==1){
            en = clock();
            diff = ((double)en-(double)st)/(CLOCKS_PER_SEC/1000);
            if((int)diff>=timeout){//Timeout
                //printf("thread %d cannot append it time out\n",(int)vargp);
                pthread_mutex_lock(&running_mutex);
                    error_count++;
                    request--;
                pthread_mutex_unlock(&running_mutex);
                break;
            }
       }
    }
    else{
             pthread_mutex_lock(&lock);
        int tid;
        tid = (int)vargp;
       //printf("buffer append, thread #%d!\n", tid);
       if(request<0){
        running_threads--;  pthread_mutex_unlock(&lock);   pthread_exit(NULL);
       }
        add(tid);
       //pthread_mutex_lock(&append_mutex);
        append_count--;
       //pthread_mutex_unlock(&append_mutex);
       //pthread_mutex_lock(&running_mutex);
       //pthread_mutex_unlock(&running_mutex);
       request--;
       pthread_mutex_unlock(&lock);
       temp_producer_thread[tid]=0;
    }

    }while(request>0);
    running_threads--;
   pthread_exit(NULL);
}

void *buffer_remove(void *vargp)
{

    clock_t st,en;
    double diff;
    int timeout;
    do{
    if(request<0)break;
    pthread_mutex_lock(&remove_mutex);
    remove_count++;
    pthread_mutex_unlock(&remove_mutex);
    st = clock();
    //srand(time(NULL));
    //timeout = (rand()*(int)vargp)%100+1;
		timeout=((int)vargp)%100+100;
    if(isEmpty()==1){
        while(isEmpty()==1){
        en = clock();
        diff = ((double)en-(double)st)/(CLOCKS_PER_SEC/1000);
        if((int)diff>=timeout){
            //printf("thread %d cannot remove it time out\n",(int)vargp);
            pthread_mutex_lock(&running_mutex);
                error_count++;
                request--;
            pthread_mutex_unlock(&running_mutex);
            break;
        }
   }
    }
    else{
     pthread_mutex_lock(&lock);
   int tid;
   tid = (int)vargp;
   //printf("buffer remove, thread #%d!\n", tid);
       if(request<0){
         running_threads--;  pthread_mutex_unlock(&lock);  pthread_exit(NULL);
      }
   del();
    //pthread_mutex_lock(&remove_mutex);
       remove_count--;
    //pthread_mutex_unlock(&remove_mutex);
    //pthread_mutex_lock(&running_mutex);
   //pthread_mutex_unlock(&running_mutex);
      request--;
   pthread_mutex_unlock(&lock);
   temp_consumer_thread[tid]=0;
    }
   }while(request>0);
   running_threads--;
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

  pthread_t threads[200];
  int rc;
  int t,i,j;

    srand(time(NULL));
	//Start time
	initial=clock();
    request_temp = request;

    for(i=0;i<producer;i++){
                if(temp_producer_thread[i]==0){
                    pthread_create(&threads[i], NULL, buffer_append, (void *)i);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     //request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_producer_thread[i]=1;

                }
        }
        for(j=producer;j<(producer+consumer);j++){
                if(temp_consumer_thread[i]==0){
                    pthread_create(&threads[j], NULL, buffer_remove, (void *)j);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     //request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_consumer_thread[j]=1;
                }
        }
          while(request>0){
           // Sleep(1);
           //printf("running thread is %d\n",running_threads);
          //printf("\rrequest is %d",request);
       }
    /*
    while(1){

        for(i=0;i<producer/4;i++){
                if(temp_producer_thread[i]==0){
                    pthread_create(&threads[i], NULL, buffer_append, (void *)i);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_producer_thread[i]=1;

                }
                if(request<=0)break;
        }
        for(j=producer;j<(producer+consumer)/4;j++){
                if(temp_consumer_thread[i]==0){
                    pthread_create(&threads[j], NULL, buffer_remove, (void *)j);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_consumer_thread[j]=1;
                }
                if(request<=0)break;
        }
        /////////////////////////////////////////////////////////////////////////////////
        for(i=producer/4;i<producer/2;i++){
                if(temp_producer_thread[i]==0){
                    pthread_create(&threads[i], NULL, buffer_append, (void *)i);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_producer_thread[i]=1;

                }
                if(request<=0)break;
        }
        for(j=(producer+consumer)/4;j<(producer+consumer)/2;j++){
                if(temp_consumer_thread[i]==0){
                    pthread_create(&threads[j], NULL, buffer_remove, (void *)j);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_consumer_thread[j]=1;
                }
                if(request<=0)break;
        }
        ////////////////////////////dsfafadfsasdfds//////////////////
        ////////////////////////////asdfsasdsadsfdsdaafsd//////////
         for(i=producer/2;i<producer*3/4;i++){
                if(temp_producer_thread[i]==0){
                    pthread_create(&threads[i], NULL, buffer_append, (void *)i);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_producer_thread[i]=1;

                }
                if(request<=0)break;
        }
        for(j=producer/2;j<(producer+consumer)*3/4;j++){
                if(temp_consumer_thread[i]==0){
                    pthread_create(&threads[j], NULL, buffer_remove, (void *)j);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_consumer_thread[j]=1;
                }
                if(request<=0)break;
        }
        /////////////////////////////////////////////////////////////////////////////////
        for(i=producer*3/4;i<producer;i++){
                if(temp_producer_thread[i]==0){
                    pthread_create(&threads[i], NULL, buffer_append, (void *)i);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_producer_thread[i]=1;

                }
                if(request<=0)break;
        }
        for(j=(producer+consumer)*3/4;j<producer+consumer;j++){
                if(temp_consumer_thread[i]==0){
                    pthread_create(&threads[j], NULL, buffer_remove, (void *)j);
                    pthread_mutex_lock(&running_mutex);
                     running_threads++;
                     request--;
                    pthread_mutex_unlock(&running_mutex);
                    temp_consumer_thread[j]=1;
                }
                if(request<=0)break;
        }

         while(running_threads>0){
            Sleep(5);
            }

           if(request<=0)break;
           printf("request is %d\n",request);
    }*/

	//Stop time
	final=clock();
	temp_cpu = ((double)final-(double)initial) / CLOCKS_PER_SEC ;

	//Outputs
	//printf("Error count = %d\n",error_count);
	printf("\nSuccessfully consumed %d requests (%.2f%%)\n",request_temp-error_count,((double)(request_temp-error_count)/(double)request_temp)* 100);
	printf("Elapsed Time: %.2f s\n",temp_cpu);
	printf("Throughput: %.2f successful requests/s\n",(double)(request_temp-error_count)/temp_cpu);

  //Last thing that main() should do
  pthread_exit(NULL);
  return 0;


}
