
#include <stdio.h>
#include <stdlib.h>
#include "buddySystem.h"
#include "common.h"
#include "Job.h"
#include "Node.h"
#include "List.h"


const int NUM_TESTS = 7;
/*const int DEBUG = 2;*/
const int DEBUG = 0;
static const char sep[] = "=========================================";

void print_stats(int *tests)
{
	printf(" ========================\n");
	printf(" Function  #invocations\n");
	printf(" ========================\n");
	printf(" addAtFront  %d \n", tests[0]);
	printf(" addAtRear   %d \n", tests[1]);
	printf(" removeFront %d \n", tests[2]);
	printf(" removeRear  %d \n", tests[3]);
	printf(" removeNode  %d \n", tests[4]);
	printf(" reverseList %d \n", tests[5]);
	printf(" search      %d \n", tests[6]);
	printf(" ========================\n");
}

void runRandomTests(int count, unsigned int seed, int n, ListPtr list)
{
	int i;
	int test;
	NodePtr node;
	JobPtr job;
	int *tests;

	tests = (int *) buddy_malloc(sizeof(int)*NUM_TESTS);
	for (i=0; i<NUM_TESTS; i++) 
		tests[i]=0;
   	srandom(seed);
    for (i=0; i<count; i++) {
		printf("\rRunning test #%d", i);
		test = (int) (NUM_TESTS * (double) rand()/RAND_MAX);
		tests[test]++;
        switch (test) {
            case 0:
				if (DEBUG > 1) fprintf(stderr,"addAtFront\n");
                n++;
                job = createJob(n, "some info", n);
                node = createNode(job);
                addAtFront(list, node);
                break;
            case 1:
				if (DEBUG > 1) fprintf(stderr,"addAtRear\n");
                n++;
                job = createJob(n, "some info", n);
                node = createNode(job);
                addAtRear(list, node);
                break;
            case 2:
				if (DEBUG > 1) fprintf(stderr,"removeFront\n");
                node = removeFront(list);
				freeNode(node);
                break;
            case 3:
				if (DEBUG > 1) fprintf(stderr,"removeRear\n");
                node = removeRear(list);
				freeNode(node);
                break;
            case 4:
				if (DEBUG > 1) fprintf(stderr,"removeNode\n");
                node = removeNode(list, search(list, i));
				freeNode(node);
                break;
            case 5:
				if (DEBUG > 1) fprintf(stderr,"reverseList\n");
                reverseList(list);
            case 6:
				if (DEBUG > 1) fprintf(stderr,"searchList\n");
                node = search(list, i);
				break;
            default:
                break;
        }
    }
	printf("\n");
	print_stats(tests);
	buddy_free(tests);
}
/**
* Runs edge tests on the current Doubly Linked List implementation
**/
void runEdgeCases()
{
	ListPtr l = createList();
	NodePtr n = NULL;
	printf("\n%s\n%s", sep, "Running edge cases with NULL values");
	printf("\n%s", "addAtFront");
	addAtFront(NULL, NULL);
	addAtFront(l, NULL);
	addAtFront(NULL, n);
	if(l->size == 0
		&& l->head == NULL
		&& l->tail == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");

	
	printf("\n%s", "addAtRear");
	addAtRear(NULL, NULL);
	addAtRear(l, NULL);
	addAtRear(NULL, n);
	if(l->size == 0
		&& l->head == NULL
		&& l->tail == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
	
	printf("\n%s", "removeFront");
	n = removeFront(NULL);
	n = removeFront(l);
	if(l->size == 0 
		&& l->head == NULL
		&& l->tail == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
	
	printf("\n%s", "removeRear");
	n = removeRear(NULL);
	n = removeRear(l);
	if(l->size == 0 
		&& l->head == NULL
		&& l->tail == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "removeNode");
	n = removeNode(NULL, NULL);
	n = removeNode(l, NULL);
	n = removeNode(NULL, n);
	if(l->size == 0 
		&& n == NULL
		&& l->head == NULL
		&& l->tail == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "reverseList");
	reverseList(NULL);
	reverseList(l);
	if(l->size == 0
		&& l->head == NULL
		&& l->tail == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
	
	printf("\n%s", "search");
	int i = 0;
	n = search(NULL, 0);
	n = search(l, 0);
	n = search(NULL, i);
	if(n == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s\n%s", sep, "Running edge cases with Single values");
	printf("\n%s", "addAtFront");
	JobPtr job = createJob(1, "Single", 1);
	n = createNode(job);
	l = createList();
	addAtFront(l, n);
	if(l->size == 1 
		&& l->head == n 
		&& l->tail == n 
		&& l->head->next == NULL 
		&& l->head->prev == NULL
		&& l->tail->next == NULL
		&& l->tail->prev == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "removeFront");
	NodePtr removeFrontNode = removeFront(l);
	if(l->size == 0 
		&& l->head == NULL 
		&& l->tail == NULL 
		&& removeFrontNode == n)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "addAtRear");
	addAtRear(l, n);
	if(l->size == 1 
		&& l->head == n 
		&& l->tail == n 
		&& l->head->next == NULL 
		&& l->head->prev == NULL
		&& l->tail->next == NULL
		&& l->tail->prev == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
	
	printf("\n%s s", "removeRear");
	NodePtr removeRearNode = removeRear(l);
	if(l->size == 0 
		&& l->head == NULL 
		&& l->tail == NULL 
		&& removeRearNode == n)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "removeNode, empty list");
	NodePtr testnode = removeNode(l, n);
	if(l->size == 0 
		&& l->head == NULL 
		&& l->tail == NULL 
		&& testnode == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "removeNode, one node, matches");
	addAtFront(l, n);
	testnode = removeNode(l, n);
	if(l->size == 0 
		&& l->head == NULL 
		&& l->tail == NULL 
		&& testnode == n)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "removeNode, one node, no match");
	JobPtr noMatchJob = createJob(1, "no match", 1);
	NodePtr noMatch = createNode(noMatchJob);
	addAtFront(l, noMatch);
	testnode = removeNode(l, n);
	if(l->size == 1 
		&& l->head == noMatch 
		&& l->tail == noMatch 
		&& testnode == NULL
		&& l->head->next == NULL 
		&& l->head->prev == NULL
		&& l->tail->next == NULL
		&& l->tail->prev == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
	
	printf("\n%s", "reverseList");
	reverseList(l);
	if(l->size == 1 
		&& l->head == noMatch 
		&& l->tail == noMatch 
		&& l->head->next == NULL 
		&& l->head->prev == NULL
		&& l->tail->next == NULL
		&& l->tail->prev == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "search, matches");
	testnode = search(l, 1);
	if(l->size == 1 
		&& l->head == noMatch 
		&& l->tail == noMatch 
		&& testnode == noMatch
		&& l->head->next == NULL 
		&& l->head->prev == NULL
		&& l->tail->next == NULL
		&& l->tail->prev == NULL)
		printf("\nSuccess");
	else
		printf("\nFail");
		
	printf("\n%s", "search, no match");
	testnode = search(l, 0);
	if(l->size == 1 
		&& l->head == noMatch 
		&& l->tail == noMatch 
		&& testnode == NULL
		&& l->head->next == NULL 
		&& l->head->prev == NULL
		&& l->tail->next == NULL
		&& l->tail->prev == NULL)
		printf("\nSuccess\n");
	else
		printf("\nFail\n");
		
	freeList(l);
}
/**
* Displays the list and all the information it includes
**/
void displayDebugInfo(ListPtr list)
{
	if(list->size == 0 || list == NULL) { printf("%s\n%s", sep, "List is empty"); return; }
	printf("%s\nHead Node: ", sep);
	printNode(list->head);
	printf("\n%s\nTail Node: ", sep);
	printNode(list->tail);
	printf("\n%s\nSize: %d ", sep, list->size);
	printf("\n%s\nList: ", sep);
	printList(list);
}
/**
* Verifies that the addAtFront function executed correctly
**/
void verifyAddAtFrontList(ListPtr list, int count)
{
	int i;
	if (list->size != count)
		printf("\n%s\n%s\n", sep, "Error: List Size Mismatch");
	else if(list->head != NULL && list->head->data->jobid != count-1)
		printf("\n%s\n%s\n", sep, "Error: List Header Mismatch");
	else if(list->tail != NULL && list->tail->data->jobid != 0)
		printf("\n%s\n%s\n", sep, "Error: List Tail Mismatch");
	NodePtr node = list->head;
	for(i = count-1; i >= 0; i--)
	{
		if(node->data->jobid != i)
		{
			printf("\n%s\n%s\n", sep, "Error: List out of order");
			break;
		}
		node = node->next;
	}
}
/**
* Verifies that the addAtRear function executed correctly
**/
void verifyAddAtRearList(ListPtr list, int count)
{
	int i;
	if(list->size != count)
		printf("\n%s\n%s\n", sep, "Error: List Size Mismatch");
	else if(list->head != NULL && list->head->data->jobid != 0)
		printf("\n%s\n%s\n", sep, "Error: List Header Mismatch");
	else if(list->tail != NULL && list->tail->data->jobid != count-1)
		printf("\n%s\n%s\n", sep, "Error: List Tail Mismatch");
	NodePtr node = list->head;
	for(i = 0; i < count; i++)
	{
		if(node->data->jobid != i)
		{
			printf("\n%s\n%s\n", sep, "Error: List out of order");
			break;
		}
		node = node->next;
	}
}
/**
* Runs tests on the addAtFront function
**/
void addAtFrontTest(ListPtr list, int count)
{
	int i;
	for(i = 0; i < count; i++)
	{
		char str[80];
		sprintf(str, "%s: %d", "Job", i);
		JobPtr job = createJob(i, str, i);
		NodePtr node = createNode(job);
		addAtFront(list, node);
	}
	if(DEBUG > 0)
		verifyAddAtFrontList(list, count);
}
/**
* Runs tests on the addAtRear function
**/
void addAtRearTest(ListPtr list, int count)
{
	int i;
	for(i = 0; i < count; i++)
	{
		char str[80];
		sprintf(str, "%s: %d", "Job", i);
		JobPtr job = createJob(i, str, i);
		NodePtr node = createNode(job);
		addAtRear(list, node);
	}
	if(DEBUG > 0)
		verifyAddAtRearList(list, count);
}
/**
* Verifies that removeFront function ran correctly
**/
void verifyFrontAll(ListPtr list, int count)
{
	if(list->size != 0)
		printf("\n%s\n%s\n", sep, "Error: removeFrontAll invalid size");
	if(list->head != NULL || list->tail != NULL)
		printf("\n%s\n%s\n", sep, "Error: removeFrontAll invalid head and/or tail pointers");
}
/**
* Verifies that removeRear function ran correctly
**/
void verifyRearAll(ListPtr list, int count)
{
	if(list->size != 0)
		printf("\n%s\n%s\n", sep, "Error: removeFrontAll invalid size");
	if(list->head != NULL || list->tail != NULL)
		printf("\n%s\n%s\n", sep, "Error: removeFrontAll invalid head and/or tail pointers");
}
/**
* Runs tests on the removeFront function by removing all the nodes in the list
**/
void removeFrontAll(ListPtr list, int count)
{
	int i;
	for(i = count; i > 0; i--)
	{
		NodePtr node = removeFront(list);
		if(DEBUG > 0)
		{
			if(node == NULL) {printf("Node is NULL"); continue;}
			//printf("i: %d jobid: %d size: %d", i, node->data->jobid, list->size);
			if(node->data->jobid != i-1 
				|| list->size != i-1 
				|| (list->head != NULL && list->head->data->jobid != i-2) 
				|| (list->tail != NULL && list->tail->data->jobid != 0)
			)
			{
				printf("\n%s\n%s\n", sep, "Error: removeFrontAll invalid");
				break;
			}
		}
		freeNode(node);
	}
	if(DEBUG > 0)
		verifyFrontAll(list, count);
}
/**
* Runs tests on the removeRear function by removing all the nodes in the list
**/
void removeRearAll(ListPtr list, int count)
{
	int i;
	int testSize = count;
	for(i = 0; i < count; i++)
	{
		NodePtr node = removeRear(list);
		testSize--;
		if(node == NULL) {printf("Node is NULL"); continue;}
		//printf("i: %d jobid: %d size: %d", i, node->data->jobid, list->size);
		if(node->data->jobid != testSize
			|| list->size != testSize
			|| (list->head != NULL && list->head->data->jobid != 0)
			|| (list->tail != NULL && list->tail->data->jobid != testSize - 1)
		)
		{
			printf("\n%s\n%s\n", sep, "Error: removeRearAll invalid");
			break;
		}
		freeNode(node);
	}
	if(DEBUG > 0)
		verifyRearAll(list, count);
}
/**
* Runs tests on the removeNode function by removing all the nodes in the list
**/
void removeNodeAllTest(ListPtr list, int count)
{
	int i;
	int testSize = count;
	for(i = 0; i < count; i++)
	{
		testSize--;
		NodePtr node = removeNode(list, search(list, testSize));
		if(node == NULL) {printf("Node is NULL"); continue;}
		//printf("i: %d jobid: %d size: %d testSize: %d tail: %d", i, node->data->jobid, list->size, testSize, list->tail->data->jobid );
		//printf("1: %d 2: %d 3: %d 4: %d", node->data->jobid != testSize, list->size != testSize, (list->head != NULL && list->head->data->jobid != 0), (list->tail != NULL && list->tail->data->jobid != testSize - 1));
		if(node->data->jobid != testSize
			|| list->size != testSize
			|| (list->head != NULL && list->head->data->jobid != 0)
			|| (list->tail != NULL && list->tail->data->jobid != testSize - 1)
		)
		{
			printf("\n%s\n%s\n", sep, "Error: removeNodeAll invalid");
			break;
		}
		freeNode(node);
	}
	if(DEBUG > 0)
		verifyRearAll(list, count);
}
/**
* Runs tests on the removeFront function by removing one node
**/
void removeFrontOne(ListPtr list)
{
	NodePtr node = removeFront(list);
	if(DEBUG > 0)
	{
		printf("\n%s\nRemoved Node: jobid: %d info: %s list size: %d", sep, node->data->jobid, node->data->command, list->size);
		//displayDebugInfo(list);
	}
}

int main(int argc, char **argv)
{	
	int i;
	int n;
	int count;
	unsigned int seed=0;

	NodePtr node;
	JobPtr job;
	ListPtr list;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <list size> [<test size=list size>] [<seed>] \n",argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	count = n;
	if (argc >= 3) {
		count = atoi(argv[2]);
	}
	if (argc == 4) {
		seed = atoi(argv[3]);
	}

	list = createList();
	runEdgeCases();
	printf("%s\n%s\n", sep, "Running addAtFrontTest");
	addAtFrontTest(list, count);	
	
	if (DEBUG > 0)
		displayDebugInfo(list);
		
	printf("\n%s\n%s\n", sep, "Running removeFromFrontAll Test");
	removeFrontAll(list, count);

	if (DEBUG > 0)
		displayDebugInfo(list);
	
	printf("\n%s\n%s\n", sep, "Running addAtRear Test");
	addAtRearTest(list, count);
	
	if (DEBUG > 0)
		displayDebugInfo(list);
	
	printf("\n%s\n%s\n", sep, "Running removingFromRear Test");
	removeRearAll(list, count);
	
	if (DEBUG > 0)
		displayDebugInfo(list);
	//for(i = count; i > 0; i--)
		//removeFrontOne(list);
		
	printf("\n%s\n%s\n", sep, "Running removeNode Test");
	addAtRearTest(list, count);
	removeNodeAllTest(list, count);
	
	if (DEBUG > 0)
		displayDebugInfo(list);
	
	printf("\n%s\n%s\n", sep, "Running reverseList Test");
	addAtRearTest(list, count);
	reverseList(list);
	verifyAddAtFrontList(list, count);
	
	if (DEBUG > 0)
		displayDebugInfo(list);
	
	printf("\n%s\n%s\n", sep, "Running random tests");
	for (i=0; i<n; i++)
	{
		job = createJob(i, "args", i);
		node = createNode(job);
		/*printf("job ptr = %p node ptr = %p\n", job, node);*/
		addAtFront(list, node);
	}
	runRandomTests(count, seed, n, list);
	freeList(list);
	exit(0);
}

/* vim: set tabstop=4: */
