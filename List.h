
/*
	List.h: Defines the interface for a doubly-linked list.
*/

#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "Node.h"

/**
* Alias for a List Struct
*/
typedef struct list List;
/**
* Alias for a pointer to a List Struct
*/
typedef struct list * ListPtr;

/**
* A structure to represent linked lists
*/
struct list {
	/*@{*/
	int size;  /**< Size of the list */
	NodePtr head; /**< Head node of the list */
	NodePtr tail; /**< Tail Node of the list */
	/*@}*/
};

/* prototypes of public methods */

ListPtr createList(); /* constructor */
void freeList(ListPtr L); /* destructor */

int getSize(ListPtr L);

Boolean isEmpty(ListPtr L);

void addAtFront(ListPtr list, NodePtr node);
void addAtRear(ListPtr list, NodePtr node);

NodePtr removeFront(ListPtr list);
NodePtr removeRear(ListPtr list);
NodePtr removeNode(ListPtr list, NodePtr node);

NodePtr search(ListPtr list, int key);

void reverseList(ListPtr  L);

void printList(ListPtr L);

NodePtr searchOnRunning(ListPtr list, int status);
NodePtr searchOnJobNu(ListPtr list, int number);

#endif /* __LIST_H */ 
