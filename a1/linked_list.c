#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

 
Node * add_newNode(Node* head, pid_t new_pid, char * new_path){
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->pid = new_pid;
	temp->path = new_path;
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



Node * deleteNode(Node* head, pid_t pid){
	if(PifExist(head,pid) == 0){ printf("no such pid\n"); return head; }
	Node * temp = head;
	if(temp->pid == pid){head = head->next; free(temp); return head;}
	else{
	Node * end = head;
	Node *prev = NULL;
	 while(end->next)
    {	
		prev = end;
		end = end->next;
		if(end->pid == pid){
			prev->next = end->next;
			free(end);
			return head;
		}
	
    }}
	return head;
}

void printList(Node *node){
	printf("\n");
	if (node == NULL){
		printf("No process so far!\n");
		}else{
	int counter = 0;
	Node *current_node = node;
	while ( current_node != NULL) {
        printf("current pid: %d current path: %s\n", current_node->pid, current_node->path);
        current_node = current_node->next;
		counter ++;
									}
		printf("total number of process is %d\n", counter);
	}
}

int PifExist(Node *node, pid_t pid){
	
	int return_value = 0;
	Node *current_node = node;
	while ( current_node != NULL) {
      if (current_node->pid == pid){
		return_value =1;
		break;
	  }
	  else{
		return_value = 0;
	  }
        current_node = current_node->next;
    }
  	return return_value ;
}

