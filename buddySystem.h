#ifndef __BUDDYSYSTEM_H
#define __BUDDYSYSTEM_H
#define MAX_SIZE 67108864
#define MAX_ARRAY_SIZE 26
#define MIN_KVAL 5
#define RESERVED 1
#define FREE 0
#define SIZE_OF_BLOCK_HEADER 9

#include <unistd.h>
#include <string.h>
#include <stdio.h>
//#include <stdlib.h>
#include <errno.h>

/**
* Alias for a Block Struct
*/
typedef struct block Block;
/**
* Alias for a pointer to a Block Struct
*/
typedef struct block * BlockPtr;
/**
* Alias for an address to memory
*/
typedef char * MemoryAddress;
/**
* Alias for an unsigned int
*/
typedef unsigned int uint;

struct block {
	/*@{*/
	int tag; /**< Reserved flag */
	int kval; /**< Power of 2 that represents the size of this block */
	MemoryAddress blockStartingAddress; /**< Starting address for the free memory in this block */
	BlockPtr next; /**< Next node in the list */
	BlockPtr prev; /**< Previous node in the list */
	/*@}*/
};

void *buddy_malloc(size_t bytes);
void buddy_free(void *address);
void printMemory();

#endif /* __BUDDYSYSTEM_H */
