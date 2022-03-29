#include <iostream>
#include <new>
#include "GarbageCollector.hpp"

#define malloc(size) Malloc(size)
#define calloc(size) Calloc(size)
#define free(size) Free(size)

void* operator new(std::size_t sz) 
{
	if (sz == 0)
		++sz; 

	if (void* ptr = malloc(sz))
		return ptr;

	throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void operator delete(void* ptr) noexcept
{
	free(ptr);
}


int main(int argc, char** argv)
{
	unsigned char mark;
	initCollector(&mark);

	int* ptr1 = new int[40];
	ptr1 = nullptr; // Now, the 160 bytes are unused. The collector will collect them

	int* ptr2 = new int[60]; // That memory will not be freed, because is a pointer pointing to this segment

	heap_collect();

	return 0;
}