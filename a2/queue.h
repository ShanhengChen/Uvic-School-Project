#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct Node Node;

struct Node{
    int id;
    int class;
    Node * next;
};


Node * enqueue(Node* head, int id, int class);
Node * dequeue(Node* head);
void printList(Node *node);
//void printList(struct Node *node);
//int PifExist(Node *node, pid_t pid);


#endif