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
	(*free_head).block_size = size - 8;
	(*free_head).next_block = 0;
}
void* my_alloc(int size) {
	//size must be positive
	//block must be multiple of void*
	//walk freelist starting with head w/ first fit heuristic
	//on block found, need to split(can fit wanted size AND excess to fit a new block with overhead)?
	//if no split, redirect pointers (remove block to fill from "singly linked list")
	//account for if it is head or not (removing is different)
	//byte location of new block = current block + allocation request
	//init new block
	//return pointer to data region which is overhead size

	if (size > 0 && (overheadSize % voidSize == 0)) {
		//walk freelist starting with head

	}
}
int main() {
	//swag 
}