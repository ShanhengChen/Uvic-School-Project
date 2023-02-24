#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

 
Node * enqueue(Node* head, int new_id, int new_class){
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->id = new_id;
	temp->class = new_class;
	temp->next = NULL;
	if(head == NULL){ 
	head = temp;
	return head;}
	Node *curr = head;
	while(curr->next != NULL){ 
		curr = curr->next;}
	curr->next = temp;
	return head; 
}

Node * dequeue(Node* head){
	Node * temp = head;
	head = head->next; free(temp); 
	return head;
	}
	
void printList(Node *node){
    int counter = 0;
	Node *current_node = node;
    	while ( current_node != NULL) {
        printf("current pid: %d and classtype %d\n", current_node->id, current_node->class);
        current_node = current_node->next;
		counter ++;
									}
		printf("total number of process is %d\n", counter);
	}
