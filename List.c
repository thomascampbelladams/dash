#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "buddySystem.h"

/*

	list.c 
		Contains functions to manipulate a doubly-linked list.
 
*/

/* private methods */

static NodePtr reverse(NodePtr L);
static void print(NodePtr node);
static const char sep[] = "=========================================";

/**
* Creates a ListPtr and returns it
* @returns New ListPtr
*/
ListPtr createList()
{
	ListPtr list;
	list = (ListPtr) buddy_malloc(sizeof(List));
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

/**
* Deallocs a ListPtr and any nodes it contains.
* @param L ListPtr to Dealloc
*/
void freeList(ListPtr L)
{
	if(L==NULL) return;
	if(L->size == 0) buddy_free(L);
	else
	{
		NodePtr currentNode = L->head;
		while(currentNode != NULL){
			NodePtr temp = currentNode;
			currentNode = currentNode->next;
			freeNode(temp);
		}
		buddy_free(L);
	}
}

/**
* Returns the size of the ListPtr
* @param L ListPtr to get size of.
* @return Size of ListPtr.
*/
int getSize(ListPtr L)
{
	return L->size;
}

/**
* Tests the list to see if it is empty
* @param L ListPtr to test.
* @return Result of test.
*/
Boolean isEmpty(ListPtr L)
{
	if (L->size == 0)
		return TRUE;
	else
		return FALSE;
}
/**
* Adds a node to the front of the list
* @param list List to add node to.
* @param node Node to add to list.
*/
void addAtFront(ListPtr list, NodePtr node)
{
	if (list == NULL) return;
	if (node == NULL) return;
	list->size++;
	node->next = list->head;
	node->prev = NULL;
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	} else {
		list->head->prev = node;
		list->head = node;
	}
}
/**
* Adds a node to the rear of the list
* @param list List to add node to.
* @param node Node to add to list.
*/
void addAtRear(ListPtr list, NodePtr node)
{
	if(list == NULL || node == NULL) return;
	list->size++;
	node->next = NULL;
	node->prev = list->tail;
	if(list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
}
/**
* Removes the front most node and returns it
* @param list ListPtr to get front most node from.
* @return NodePtr that was the front most node.
*/
NodePtr removeFront(ListPtr list)
{
	if(list == NULL || list->size == 0) return NULL;
	list->size--;
	NodePtr prevHeadNode = list->head;
	list->head = list->head->next;
	if(list->head != NULL) 
	{
		list->head->prev = NULL;
		prevHeadNode->next = NULL;
	} else {
		list->tail = NULL;
	}
	return prevHeadNode;
}
/**
* Removes the rear most node and returns it
* @param list ListPtr to get rear most node from.
* @return NodePtr that was the rear most node.
*/
NodePtr removeRear(ListPtr list)
{
	if(list == NULL || list->size == 0) return NULL;
	list->size--;
	NodePtr prevTailNode = list->tail;
	list->tail = list->tail->prev;
	if(list->tail != NULL)
	{
		list->tail->next = NULL;
		prevTailNode->prev = NULL;
	} else {
		list->head = NULL;
	}
	return prevTailNode;
}
/**
* Removes the node passed in and returns it
* @param list ListPtr to remove the node from.
* @param node NodePtr to search for on the list.
* @param NodePtr removed from the list, or NULL if it was not found in the list.
*/
NodePtr removeNode(ListPtr list, NodePtr node)
{
	if(list == NULL || list->size == 0 || node == NULL) return NULL;
	NodePtr currentNode = list->head;
	while (currentNode != node){
		if(currentNode->next != NULL)
			currentNode = currentNode->next;
		else{
			currentNode = NULL;
			break;
		}
	}
	if(currentNode != NULL)
	{
		if(currentNode->next != NULL)
			currentNode->next->prev = currentNode->prev;
		if(currentNode->prev != NULL)
			currentNode->prev->next = currentNode->next;
		if(list->head == currentNode)
			list->head = currentNode->next;
		if(list->tail == currentNode)
			list->tail = currentNode->prev;
		currentNode->next = currentNode->prev = NULL;
		list->size--;
	}
	return currentNode;
}
/**
* Searches for a node with a specific jobid and returns it
* @param list ListPtr to search.
* @param key jobid to search for.
* @param NodePtr found in the list, or NULL if it was not found in the list.
*/
NodePtr search(ListPtr list, int key)
{
	if(list == NULL || list->size == 0) return NULL;
	NodePtr currentNode = list->head;
	while(currentNode->data->jobid != key){
		if(currentNode->next != NULL)
			currentNode = currentNode->next;
		else{
			currentNode = NULL;
			break;
		}
	}
	return currentNode;
}

/**
* Searches for a node with a specific running value and returns it
* @param list ListPtr to search.
* @param key running value to search for.
* @param NodePtr found in the list, or NULL if it was not found in the list.
*/
NodePtr searchOnRunning(ListPtr list, int status)
{
	if(list == NULL || list->size == 0) return NULL;
	NodePtr currentNode = list->head;
	while(currentNode->data->running != status){
		if(currentNode->next != NULL)
			currentNode = currentNode->next;
		else{
			currentNode = NULL;
			break;
		}
	}
	return currentNode;
}

/**
* Searches for a node with a specific jobNu and returns it
* @param list ListPtr to search.
* @param key jobNu to search for.
* @param NodePtr found in the list, or NULL if it was not found in the list.
*/
NodePtr searchOnJobNu(ListPtr list, int number)
{
        if(list == NULL || list->size == 0) return NULL;
        NodePtr currentNode = list->head;
        while(currentNode->data->jobNu != number){
        	if(currentNode->next != NULL)
			currentNode = currentNode->next;
		else{
	        	currentNode = NULL;
	        	break;
		}
	}
        return currentNode;
}
/**
* Reverse's the list
* @param L ListPtr to reverse.
*/
void reverseList(ListPtr L)
{
	if (L == NULL) return;
	L->tail = L->head;
	L->head  = reverse (L->head);
}
/**
* Reverses the NodePtr's prev and next links and returns it
* @param L NodePtr to reverse.
* @return Reversed NodePtr.
*/
static NodePtr reverse(NodePtr L)
{
	NodePtr list = NULL;
	while (L != NULL) {
		NodePtr tmp = L;
		L = L->next;
		if (L != NULL) L->prev = tmp;
		tmp->next = list;
		tmp->prev = L;
		list = tmp;
	}
	return list;
}

/**
* Prints a string representation of the ListPtr
* @param L ListPtr to print
*/
void printList(ListPtr L)
{
	if (L) print(L->head);
}

/**
* Prints out a string representation of NodePtr and all the other NodePtr's it is connected to.
* @param node NodePtr to print.
*/
static void print(NodePtr node)
{
	char *output;

	while (node) {
		output = toString(node->data);
		printf("%s",output);
		buddy_free(output);
		node = node->next;
	} 
}
 

		

