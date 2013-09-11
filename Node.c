#include "Node.h"
#include "buddySystem.h"

/**
* Initializes a NodePtr and stores a JobPtr in it.
* @param data JobPtr represented by the new NodePtr
* @return New NodePtr
*/
NodePtr createNode(JobPtr data)
{
	NodePtr newNode = (NodePtr) buddy_malloc (sizeof(Node));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->data = data;
	return newNode;
}

/**
* Prints a string representation of the node to stdout.
* @param node Node to represent as a string
*/
void printNode(NodePtr node)
{
	char* output = toString(node->data);
	printf(" %s -->",output);
}

/**
* Determines if the NodePtr is null.
* @param n NodePtr to test.
* @return result of test.
*/
int isNodeNull(NodePtr n){
        return n == NULL;
}

/**
* Deallocs the NodePtr and the JobPtr it contains
* @param node NodePtr to dealloc
*/
void freeNode (NodePtr node)
{
	if (isNodeNull(node)) return;
	freeJob(node->data);
	buddy_free(node);
}
