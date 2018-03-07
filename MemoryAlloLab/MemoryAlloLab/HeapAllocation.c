#include <stdio.h>
#include <stdlib.h>
struct Block {
	int block_size; //# of bytes in the data section
	struct Block *next_block; //in C, you have to use 'struct Block' as the type
};
const int overheadSize = sizeof(struct Block);
const int voidSize = sizeof(void*);
struct Block *free_head;
void my_initialize_heap(int size) {
	//4 bytes for pointer, 4 bytes for size
	//size - 8.
	free_head = malloc(size);
	(*free_head).block_size = size - overheadSize;
	(*free_head).next_block = NULL;
}
void* my_alloc(int size) {
	//alignment rules: struct is size 8, declare b4 ints
	struct Block* myBlock = free_head;
	struct Block* prevBlock = free_head;
	int multiple = size % voidSize;
	int countToPrevious = 0;
	//mutate: "True size to allocate"
	//int mutate = size + (size % voidSize);
	int mutate;
	// probably don't need this if/else statement
	if (multiple != 0) {
		mutate += size + multiple;
	}
	else
		mutate = size;
	//start "singly linked list" traversal
	while(!(*myBlock).block_size >= mutate){
		//this means that myBlocks size does not have enough room to
		//fit the desired allocation size (parameter size)
		if(myBlock == NULL) {
			// can't do anything, return to end my_alloc
			return 0;
		}
		else
		{
			//move to next block
			myBlock = (*myBlock).next_block;
			countToPrevious++;
		}
		
	}
	for (int i = 0; i < countToPrevious; i++) {
		//move prevBlock to be the previous Block that would point to myBlock.
		prevBlock = (*prevBlock).next_block;
	}
	//quick test to check prev Block/////////
	if((*prevBlock).next_block == myBlock)
		printf("We Gucci...\n");
	//////////////////end test/////////////

	//leaving this loop means the block size is good.
	//if yes AND can split block
	int checkSplit = (*myBlock).block_size - mutate;
	if (checkSplit > (overheadSize + voidSize)) {
		//above logic: if checksplit is greater than compared number 
		//then it has remaining space to make a new block
		//now check if myBlock is same as free head or not
		if (myBlock == free_head)
		{
			//change free_head to point to the new splitted block.
			free_head = overheadSize + mutate;
			//ensure free_head still points to NULL or open blocks
			(*free_head).next_block = (*myBlock).next_block;
			//fix block size of my block
			(*myBlock).block_size = mutate;
		}
		else {
			//unlock above, myBlock will need to point to the newly split block to properly
			//rearrange pointers
			//this means that we are not same as free_head, but traversed the "list"
			(*prevBlock).next_block = (*myBlock).next_block;
			(*myBlock).block_size = mutate;
			// \/create split below \/
			myBlock = overheadSize + mutate;
			//^^create split above^^
			(*myBlock).next_block = free_head;
			free_head = myBlock;
		}
	}//if yes but cannot split block
	else {
		//implement allocation
		if (myBlock == free_head) {
			//no need to move pointers other than head
			(*myBlock).block_size = mutate;
			free_head = (*myBlock).next_block;
		}
		else {
			//will require pointer changing unlike above if

		}
	}


}
int main() {
	//swag 
}