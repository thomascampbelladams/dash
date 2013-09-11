
#ifndef __NODE_H
#define __NODE_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "Job.h"

/**
* Alias for a Node Struct
*/
typedef struct node Node;
/**
* Alias for a pointer to a Node Struct
*/
typedef struct node * NodePtr;

/**
* A structure to represent nodes in a list
*/
struct node {
	/*@{*/
	JobPtr data; /**< Job represented by this node */
	NodePtr next; /**< Next node in the list */
	NodePtr prev; /**< Previous node in the list */
	/*@}*/
};

NodePtr createNode (JobPtr data);
void freeNode(NodePtr node);
void printNode(NodePtr node);
int isNodeNull(NodePtr node);
#endif /* __NODE_H */
