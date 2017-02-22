#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#define max 1000
//Global Variable
struct node *head = NULL;
struct node *current = NULL;
int count = 0;

typedef struct node
{
	int data;
	struct node* next;
}node;

int isEmpty() {
	return head == NULL;
}

node* getNode(int num, node* nxt) {
	node*q = (node*)malloc(sizeof(node));
	q->data = num;
	q->next = nxt;
	return q;
}

void add(int num) {
	node *temp = (struct node*) malloc(sizeof(struct node));
	temp->data = num;

	if (isEmpty()) {
		head = temp;
		head->next = head;
		current = head;
		count++;
	}
	else {
		if (count == max) {
			printf("It full shit!\n");
		}
		else {
			current->next = temp;
			temp->next = head;
			current = temp;
			count++;
		}
	}
}

void del() {

	if (isEmpty()) {
		printf("Fucking it empty\n");
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
	}
}

int main(void) {

	add(1);
	add(2);
	add(3);
	add(4);
	add(5);
	add(6);
	add(7);
	add(8);
	add(9);
	add(10);


	//printf("%d", current->next->data);

}
