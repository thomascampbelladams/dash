#include "buddySystem.h"

MemoryAddress BaseAddress;
BlockPtr blockAddress[MAX_ARRAY_SIZE+1];
BlockPtr initialBlock;
size_t initialBlockAddress;
int initialized = 0;
const uint b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
const uint S[] = {1, 2, 4, 8, 16};

/**
* Adds an arbitrary amount to a block's address
* @param block Pointer to the block
* @param amount Amount to add to the block's address
* @return New address from the result of the sum of the amount and the block's address
*/
size_t addToAddress(BlockPtr block, int amount){
	size_t *iA = (size_t *) &block;
	size_t initialAddress = *iA;
	size_t newAddress = initialAddress + amount;
	return newAddress;
}

/**
* Determines the address for the buddy block for a specified block
* @param block BlockPtr to find the buddy block for
* @return Address to the buddy block.
*/
size_t buddy(BlockPtr block){
	size_t address = addToAddress(block, -initialBlockAddress);
	return (addToAddress(initialBlock, address^(1<<block->kval)));
}

/**
* Sets the header for a block
* @param block Pointer to the block
* @param tag Reservation tag for the block
* @param kval Kval for the block
*/
void initializeReservedBlock(BlockPtr block, int tag, int kval)
{
	block->tag = tag;
	block->kval = kval;
}

/**
* Sets the header for a block, and links it to the previous free block
* @param block Pointer to the block
* @param tag Reservation tage for the block
* @param kval Kval for the block
* @param prevBlock Previous block in the list
*/
void initializeFreeBlock(BlockPtr block, int tag, int kval, BlockPtr prevBlock)
{
	initializeReservedBlock(block, tag, kval);
	block->prev = prevBlock;
}

/**
* Initializes the memory block for the buddy system
*/
void initial(){
	initialBlock = (BlockPtr) sbrk(MAX_SIZE);
	blockAddress[MAX_ARRAY_SIZE] = initialBlock;
	initializeFreeBlock(initialBlock, FREE, MAX_ARRAY_SIZE, NULL);
	initialBlockAddress = addToAddress(initialBlock, 0);
	initialized = 1;
}

/**
* Returns the size of the block based on it's kval
* @param block Pointer to the block
* @return Size of the block
*/
int buddy_getSize(BlockPtr block){
	return 1 << block->kval;
}

/**
* Returns whether or not the block is currently reserved
* @param block Pointer to the block
* @return True if reserved, false if free
*/
int isReserved(BlockPtr block){
	return block->kval == RESERVED;
}

/**
* Returns whether or not the next block is null
* @param block Pointer to the block
* @return True if null, false if defined
*/
int isNextBlockNull(BlockPtr block){
	return block->next == NULL;
}

/**
* Returns whether or not the index of the blockAddress array is null
* @param index Index of the array to check
* @return True if null, false if defined.
*/
int isBlockIndexNull(int index){
	return blockAddress[index] == NULL;
}

/**
* Returns the next block in the list
* @param block Point to the blocker
* @return The next block, unless there is no more blocks in the list. If there are no
* more blocks in the list, it returns NULL.
*/
BlockPtr nextFreeBlock(BlockPtr block){
	if(isNextBlockNull(block)) return NULL;
	return block->next;
}

/**
* Determines if the index of the block addresses is currently empty
* @param kval Kval to check
* @return True if there is no block at the current index of the block addresses array, false otherwise
*/
int isKvalEmpty(int kval){
	return blockAddress[kval] == NULL;
}

/**
* Searches the free list for the block and removes it
* @param block BlockPtr to search for
*/
void searchAndRemoveBlockFromFreeList(BlockPtr block){
	if(block == blockAddress[block->kval]) blockAddress[block->kval] = block->next;
	else {
		BlockPtr searchBlock = blockAddress[block->kval];
		while(searchBlock->next != block) searchBlock = searchBlock->next;
		searchBlock->next = NULL;
	}
}

/**
* Determines if the block is marked as a free block
* @param block BlockPtr to check
* @return True if free, false if otherwise
*/
int isBlockFree(BlockPtr block){ return block->tag == FREE; }

/**
* Determines if the kval is less than the space required for the header
* @param kval Kval to check
* @return True if less than the minimum, false otherwise.
*/
int isKvalLessThanMin(int kval){ return kval < 5; }

/**
* Splits a specified block and sets the buddy block in the free list
* @param block Block to split
* @return The left most block from the split
*/
BlockPtr splitBlock(BlockPtr block){
	int newSize = buddy_getSize(block)>>1;
	int newKval = block->kval - 1;
	if(newKval < 0) return NULL;
	else if(isKvalLessThanMin(newKval)) return block;
	if(isBlockFree(block)) searchAndRemoveBlockFromFreeList(block);
	size_t newAddress = addToAddress(block, newSize);
	BlockPtr leftBlock = (BlockPtr) block;
	BlockPtr rightBlock = (BlockPtr) (newAddress);
	initializeReservedBlock(leftBlock, RESERVED, newKval);
	initializeFreeBlock(rightBlock, FREE, newKval, blockAddress[newKval]);
	if(isKvalEmpty(newKval)) blockAddress[newKval] = rightBlock;
	return leftBlock;
}

/**
* Determines if the block's Kval is at or above the maximum kval allocated
* @param block The block to look at
* @return True if the block's kval is at or above the max kval allocated, false otherwise
*/
int isKvalAtMax(BlockPtr block) { return block->kval >= MAX_ARRAY_SIZE; }

/**
* Determines if the block address is valid
* @param blockAddress The block address to look at
* @return True if valid, false if invalid
*/
int isBlockAddressValid(size_t blockAddress) { return blockAddress >= initialBlockAddress; }

/**
* Determines if the block's buddy block needs to be merged in the free list
* @param block The BlockPtr of the buddyBlock's buddy.
* @param buddyBlock The BlockPtr of the block's buddy.
* @param buddyAddress The address of the buddyBlock
* @return True if the buddy block of block is ready to be merged, false if otherwise
*/
int isNextBuddyBlockReadyToJoin(BlockPtr block, BlockPtr buddyBlock, size_t buddyAddress){
	return !isKvalAtMax(block) && isBlockAddressValid(buddyAddress) && isBlockFree(buddyBlock);
}

/**
* Sets the node back into the free list.
* @param block Block to place back into the free list
*/
void setNodeInFreeList(BlockPtr block){
	BlockPtr nextBlock = blockAddress[block->kval];
	while(nextBlock->next != NULL) nextBlock = nextBlock->next;
	nextBlock->next = block;
}

/**
* Joins the block and all it's resultant buddy blocks. It puts the result back on the free list
* @param block BlockPtr to the block to free
* @param blocksAddress Address to the block to free
*/
void joinBlock(BlockPtr block, size_t blocksAddress){
	size_t buddyAddress = buddy(block);
	BlockPtr buddyBlock = (BlockPtr) buddyAddress;
	BlockPtr reallocBlock;
	
	if(isKvalAtMax(block)) return;
	if(isBlockFree(block)){
		reallocBlock = (buddyAddress < blocksAddress) ? buddyBlock : block;
			
		searchAndRemoveBlockFromFreeList(buddyBlock);
		reallocBlock->kval = block->kval+1;
	}
	else reallocBlock = block;
	
	reallocBlock->tag = FREE;
	reallocBlock->next = NULL;
	reallocBlock->prev = NULL;
	buddyAddress = buddy(reallocBlock);
	buddyBlock = (BlockPtr) buddyAddress;
	
	if(isNextBuddyBlockReadyToJoin(reallocBlock, buddyBlock, buddyAddress)){
		size_t reallocBlocksAddress = addToAddress(reallocBlock, 0);
		joinBlock(reallocBlock, reallocBlocksAddress);
	}
	else {
		if(isKvalEmpty(reallocBlock->kval)) blockAddress[reallocBlock->kval] = reallocBlock;
		else setNodeInFreeList(reallocBlock);
	}
}

/**
* Frees a block and puts it back on the free list
* @param address Address to the block to free
*/
void buddy_free(void *address){
	size_t blocksAddress = addToAddress((BlockPtr)address, -SIZE_OF_BLOCK_HEADER);
	BlockPtr block = (BlockPtr) blocksAddress;
	joinBlock(block, blocksAddress);
}

/**
* Prints out a block and any other blocks that it is linked to at the same kval
* @param block Block to print out.
*/
void printBlock(BlockPtr block){
	if(block == NULL) return;
	printf("Address: %p kval:%d tag:%d blockStartingAddress:%p next:%p prev:%p -->", block, block->kval, block->tag, block->blockStartingAddress, block->next, block->prev);
	if(!isNextBlockNull(block)) printBlock(block->next);
}

/**
* Prints out a block from the address of the end of the Block Header
* @param block BlockPtr that is adjusted to the end of the Block Header
*/
void printAddress(BlockPtr block){
	if(block == NULL) return;
	size_t blockHeaderAddress = addToAddress(block, -SIZE_OF_BLOCK_HEADER);
	BlockPtr blockToPrint = (BlockPtr) blockHeaderAddress;
	printBlock(blockToPrint);
}

/**
* Prints out the blockAddress array and all the free blocks it contains
*/
void printMemory(){
	int i = 0;
	for(; i <= MAX_ARRAY_SIZE; i++)
		if(!isBlockIndexNull(i)) {
			printf("\n\tBlock at index:%d ", i);
			printBlock(blockAddress[i]);
		}
}

/**
* Returns the ceiling of the log of 2 of a passed in number
* @param v Number to determine log of 2 from
* @return Ceiling of the log of 2 of the passed in number
* Credits go to Sean Eron Anderson for this Bit Twiddling hack
* The original page this was found on can be found at the following url:
* http://graphics.stanford.edu/~seander/bithacks.html
*/
int upper_power_of_two(uint v)
{
	int o = v;
	int i;

	register uint r = 0; // result of log2(v) will go here
	for (i = 4; i >= 0; i--) // unroll for speed...
	{
	  if (v & b[i])
	  {
		v >>= S[i];
		r |= S[i];
	  } 
	}
	if(o > (1<<r)) r++;
	return r;
}

/**
* Reserves a chunk of memory and returns the address of the beginning of the free space in the chunk.
* @param bytes Size of the chunk of memory to request
* @return Pointer to the beginning of the free space
*/
void *buddy_malloc(size_t bytes){
	if(initialized == 0) initial();
	size_t requestSize = (size_t) upper_power_of_two((uint) bytes + (1<<5));
	BlockPtr requestedBlock = NULL;
	int i = requestSize;
	
	while(requestedBlock == NULL)
	{
		requestedBlock = blockAddress[i];
		if(i > MAX_ARRAY_SIZE){
			errno = ENOMEM;
			return NULL;
		}
		i++;
	}
	
	if(requestedBlock->kval <= requestSize){
		initializeReservedBlock(requestedBlock, RESERVED, requestedBlock->kval);
	}
	while(requestedBlock->kval > requestSize)
	{
		requestedBlock = splitBlock(requestedBlock);
	}
	
	requestedBlock->tag = RESERVED;
	if(blockAddress[requestedBlock->kval] == requestedBlock) 
		blockAddress[requestedBlock->kval] = requestedBlock->next;
	
	
	return &requestedBlock->blockStartingAddress;
}

void **buddy_calloc(size_t n, size_t bytes){
	BlockPtr *addresses = buddy_malloc(bytes*n);
	int i = 0;
	if(n == 0) return NULL;
	for(; i<n; i++){
		addresses[i] = buddy_malloc(bytes);
	}
	return (void **)addresses;
}

/**
* Tests for malloc
*/
/*void testBalloc(){
	char *test = malloc(sizeof(char)*8);
	char *secondTest = malloc(sizeof(char)*13);
	char *thirdTest = malloc(sizeof(char)*8); 
	char *fourthTest = malloc(sizeof(char)*8);
	char *fifthTest = malloc(sizeof(char)*8);
	char *sixthTest = malloc(sizeof(char)*480);
	char *loopTest = malloc(sizeof(char)*10);
	char *testString = "Oh, hi.";
	char *secondTestString = "Hello World.";
	char *thirdtestString = "Oh, li.";
	char *fourthtestString = "Oh, ki.";
	char *fifthtestString = "Oh, ji.";
	char *sixthString = "Oh, gi.";
	int i = 0;
	memcpy(test, testString, sizeof(char)*8);
	memcpy(secondTest, secondTestString, sizeof(char)*13);
	memcpy(thirdTest, thirdtestString, sizeof(char)*8);
	memcpy(fourthTest, fourthtestString, sizeof(char)*8);
	memcpy(fifthTest, fifthtestString, sizeof(char)*8);
	memcpy(sixthTest, sixthString, sizeof(char)*8);
	printf(test);
	printf(secondTest);
	printf(thirdTest);
	printf(fourthTest);
	printf(fifthTest);
	printf(sixthTest);
	printMemory();
	while(loopTest != NULL)
	{
		sprintf(loopTest, "Oh, hi.%d\n", i);
		printf(loopTest);
		loopTest = malloc(sizeof(char)*10);
		i++;
	}
	printMemory();
}*/

/**
* Tests for block splitting
*/
/*void testSplitBlock(){
	BlockPtr leftBlock;
	int i = 1;
	int j = MAX_ARRAY_SIZE;
	size_t *pba;
	size_t prevba;
	size_t *cba;
	size_t currba;
	initialize();
	initializeFreeBlock(initialBlock, FREE, MAX_ARRAY_SIZE, NULL);
	while(leftBlock != NULL && blockAddress[j] != NULL)
	{
		printf("\n===========Split %d============\n", i);
		leftBlock = splitBlock(blockAddress[j]);
		cba = (size_t *) &leftBlock;
		currba = *cba;
		if(j < MAX_ARRAY_SIZE){
			if(currba == prevba) printf("\n\t\tAddress: Correct\n");
			else printf("\n\t\tAddress: Incorrect\n");
		}
		printMemory();
		printBlock(leftBlock);
		i++;
		j--;
		if(leftBlock->kval == j) printf("\n\t\tKval: Correct\n");
		else printf("\n\t\tKval: Incorrect\n");
		pba = (size_t *) &blockAddress[j];
		prevba = *pba;
	}
}*/

/**
* Tests for joinBlock
*/
/*
void testJoinBlock(int trials){
	//BlockPtr test = malloc(sizeof(char)*128);
	//joinBlock(test);
	//printMemory();
	BlockPtr testBlocks[trials];
	int i = 0;
	
	for(; i<trials; i++)
	{
		testBlocks[i] = malloc(sizeof(char)*128);
		//printMemory();
		//getchar();
	}
	for(i=0; i<trials; i++){
		printf("\ns%d\n", i);
		printAddress(testBlocks[i]);
		joinBlock(testBlocks[i]);
		printMemory();
		getchar();
	}
	
	printMemory();
}
*/

/**
* Tests for free
*/
/*void testFree(int trials){
	BlockPtr testBlocks[trials];
	int i = 0;
	
	for(; i<trials; i++)
	{
		testBlocks[i] = malloc(sizeof(char)*128);
		//printMemory();
		//getchar();
	}
	BlockPtr *testBlocks = (BlockPtr *)calloc(trials, sizeof(char)*128);
	int i = 0;
	for(i=0; i<trials; i++){
		printf("\ns%d\n", i);
		printAddress(testBlocks[i]);
		free(testBlocks[i]);
		printMemory();
		getchar();
	}
	printAddress((BlockPtr)testBlocks);
	free(testBlocks);
	
	printMemory();
}*/

/**
* Tests for upper_power_of_two
*/
/*void testForPowerOfTwo(){
	printf("\n 2, power of 2 = %d\n", upper_power_of_two(2));
	printf("\n 15, power of 2 = %d\n", upper_power_of_two(15));
	printf("\n 131, power of 2 = %d\n", upper_power_of_two(131));
}*/

/*int main(int argc, char *argv[])
{
	if(strcmp(argv[1], "s") == 0)
		testSplitBlock();
	else if(strcmp(argv[1], "2") == 0)
		testForPowerOfTwo();
	else if(strcmp(argv[1], "b") == 0)
		testmalloc();
	else if(strcmp(argv[1], "j") == 0)
		testJoinBlock(atoi(argv[2]));
	else if(strcmp(argv[1], "f") == 0)
		testFree(atoi(argv[2]));
		
	exit(0);
}*/
