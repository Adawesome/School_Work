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
	printf("fresh malloc free_head at: %p\n", (void *)free_head);
	//(*free_head).block_size = size - overheadSize; <<< this created a "ghost" overhead of 8bytes
	(*free_head).block_size = size;
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
	mutate = size;
	while (multiple != 0) {
		mutate++;
		multiple = mutate % voidSize;
	}
	if (myBlock == NULL) {
		// can't do anything, return to end my_alloc
		return 0;
	}
	//start "singly linked list" traversal
	while((*myBlock).block_size < (mutate + overheadSize)){
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
	for (int i = 1; i < countToPrevious; i++) {
		//move prevBlock to be the previous Block that would point to myBlock.
		prevBlock = (*prevBlock).next_block;
	}
	//leaving this loop means the block size is good.
	//if yes AND can split block
	if (myBlock == NULL) {
		// can't do anything, return to end my_alloc
		return 0;
	}
	int checkSplit = (*myBlock).block_size - mutate;
	if (checkSplit >= (overheadSize + voidSize)) {
		//above logic: if checksplit is greater than compared number 
		//then it has remaining space to make a new block
		//now check if myBlock is same as free head or not
		if (myBlock == free_head)
		{
			//change free_head to point to the new splitted block.
			//
			free_head += overheadSize + mutate;
			//ensure free_head still points to NULL or open blocks
			(*free_head).next_block = (*myBlock).next_block;
			//fix block size of my block
			(*free_head).block_size = (*myBlock).block_size - overheadSize - mutate;
			(*myBlock).block_size = mutate;
			
		}
		else {
			//unlock above, myBlock will need to point to the newly split block to properly
			//rearrange pointers
			//this means that we are not same as free_head, but traversed the "list"
			struct Block* tempBlockTest = myBlock;
			int splitSize = (*tempBlockTest).block_size - overheadSize - mutate;
			(*prevBlock).next_block = (*myBlock).next_block;
			// \/create split below \/
			myBlock += overheadSize + mutate;
			//^^create split above^^
			//myBlock is now the newly made open spaced block
			//now have the newly made open space point to 
			// the next open-block in the list (or NULL)
			(*myBlock).next_block = (*prevBlock).next_block;
			//finally have prevBlock point to the newly opened block
			(*prevBlock).next_block = myBlock;
			(*myBlock).block_size = splitSize;
			return tempBlockTest;
		}
	}//if yes but cannot split block
	else {

		//implement allocation
		if (myBlock == free_head) {
			//no need to move pointers other than head
			//(*myBlock).block_size = mutate;
			free_head = (*myBlock).next_block;
		}
		else {
			//will require pointer changing unlike above if
			(*prevBlock).next_block = (*myBlock).next_block;
			//(*myBlock).block_size = mutate;
			//(*myBlock).next_block = NULL;
		}
	}
	return myBlock;
}
void my_free(void *data) {
	// will deallocate the given Block pointer (passed via parameter)
	//note, a pointer is being passed as a parameter... this is pass by reference... not changing
	//a "copy" of the value but the actual value itself
	struct Block* blockLocation = data;
	(*blockLocation).block_size += overheadSize;
	//fix block size becuase we create overhead on my_alloc, so have to get rid of that overhead here
	//otherwise we would have 2 overheads...
	(*blockLocation).next_block = free_head;
	free_head = blockLocation;
	//1st : "data" points to current_free_head
	//2nd : free_head points to "data" which is pointing to old_free_head

}
void testCase1() {
	//my_initialize_heap(1000);
	printf("TESTCASE ONE\n");
	printf("______________________________________\n");
	void* ptr = my_alloc(sizeof(int));
	printf("CURRENT POINTER: %p\n", (void *)ptr);
	my_free(ptr);
	printf("AFTER FREE: %p\n", (void *)ptr);
	void* next = my_alloc(sizeof(int));
	printf("SAME ALLOC: %p\n", (void *)next);
	printf("______________________________________\n");
}
void testCase2() {
	//my_initialize_heap(1000);
	printf("TESTCASE TWO\n");
	printf("______________________________________\n");
	//overheadSize == 8
	//integer size == 4
	//total(in hex)== 0xC
	void* allo1 = my_alloc(sizeof(int));
	printf("_FIRST INT POINTER: %p\n", (void *)allo1);
	//printPointer(allo1);
	void* allo2 = my_alloc(sizeof(int));
	printf("SECOND INT POINTER: %p\n", (void *)allo2);
	//printPointer(allo2);
	printf("______________________________________\n");
}
void testCase3() {
	//my_initialize_heap(1000);
	int temp;

	printf("TESTCASE THREE\n");
	printf("______________________________________\n");
	printf("CURRENT FREE HEAD : ");
	temp = printPointer(free_head);
	void* allo1 = my_alloc(sizeof(int));
	printf("_FIRST INT POINTER: ");
	temp = printPointer(allo1);
	void* allo2 = my_alloc(sizeof(int));
	printf("SECOND INT POINTER: ");
	temp = printPointer(allo2);
	void* allo3 = my_alloc(sizeof(int));
	printf("_THIRD INT POINTER: ");
	temp = printPointer(allo3);
	my_free(allo2);
	void* allo4 = my_alloc(sizeof(double));
	printf("__A DOUBLE POINTER: ");
	temp = printPointer(allo4);
	void* sameAs2 = my_alloc(sizeof(int));
	printf("____________________________\n");
	printf("SECOND INT POINTER: ");
	temp = printPointer(allo2);
	printf("___SAME AS SECOND?: ");
	temp = printPointer(sameAs2);
	printf("CURRENT FREE HEAD : ");
	temp = printPointer(free_head);
	printf("______________________________________\n");
}
int printPointer(void *ptr) {
	struct Block* test = ptr;
	printf("%p\n",(void *)ptr);
	return 0;
}
void testCase4() {
	//my_initialize_heap(1000);
	printf("TESTCASE FOUR\n");
	printf("______________________________________\n");
	int temp;
	printf("FREE HEAD LOCATOIN: ");
	printPointer(free_head);
	void* charTest = my_alloc(sizeof(char));
	printf("CHAR LOCATION     : ");
	printPointer(charTest);
	void* intTest = my_alloc(sizeof(int));
	printf("INTEGER LOCATION  : ");
	printPointer(intTest);
	printf("FREE HEAD LOCATOIN: ");
	printPointer(free_head);
	printf("______________________________________\n");
}
void testCase5() {
	//my_initialize_heap(1000);
	//int arr[100];
	printf("TESTCASE FIVE\n");
	printf("______________________________________\n");
	printf("FREE HEAD  LOCATION: ");
	printPointer(free_head);
	void* bigArray = my_alloc(sizeof(int)*100);
	printf("BIG DADDY ARRAY LOC: ");
	printPointer(bigArray);
	void* allo1 = my_alloc(sizeof(int));
	printf("NORMAL_____ INTEGER: ");
	printPointer(allo1);
	my_free(bigArray);
	printf("POST ARRAY FREEING : ");
	printPointer(allo1);
	printf("FREE HEAD  LOCATION: ");
	printPointer(free_head);
	printf("______________________________________\n");
}
int main() {
	my_initialize_heap(1000);
	//testCase1();
	//testCase2();
	//testCase3();
	//testCase4();
	testCase5();
	getchar();
}