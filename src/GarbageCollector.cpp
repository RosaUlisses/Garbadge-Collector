#include "GarbageCollector.hpp"

Hash_set<void*> allocated_chunks;
Hash_map<void*, size_t> chunk_sizes;

unsigned char* stack_base = nullptr;


void initCollector(unsigned char* pointer)
{
	stack_base = pointer;
}

void* Malloc(size_t size)
{
	void* pointer = std::malloc(size);
	allocated_chunks.insert(pointer);
	chunk_sizes.insert(pointer, size);
	return pointer;
}

void* Calloc(size_t num, size_t size)
{
	void* pointer = std::calloc(num, size);
	allocated_chunks.insert(pointer);
	chunk_sizes.insert(pointer, num * size);
	return pointer;
}

void Free(void* ptr)
{
	allocated_chunks.erase(ptr);
	chunk_sizes.erase(ptr);
	std::free(ptr);
}


void free_memory(Hash_map<void*, bool>& is_allocated)
{
	Hash_map<void*, bool>::Map_iterator<void*, bool> it = is_allocated.begin();
	Hash_map<void*, bool>::Map_iterator<void*, bool> end = is_allocated.end();

	while (it != end)
	{
		if(!it.getValue())
		{
			allocated_chunks.erase(it.getKey());
			chunk_sizes.erase(it.getKey());
			std::free(it.getKey());
		}
		it++;
	}
}

void find_chunks(void* pointer, Hash_map<void*, bool>& is_pointed)
{
	if (pointer == nullptr) return;

	unsigned char* ptr = (unsigned char*)pointer;
	size_t size = chunk_sizes.get(ptr);

	for (int i = 0; i < size; i++)
	{
		void* val = *((void**)ptr);

		if (allocated_chunks.contains(val))
		{
			find_chunks(val, is_pointed);
			is_pointed.set(val, true);
		}

		ptr++;
	}
}

void heap_collect()
{
	if (allocated_chunks.get_Element_count() == 0) return;

	unsigned char* current = (unsigned char*)&current;

	List<void*> referenced_chunks;
	Hash_map<void*, bool> is_pointed;

	for (; current <= stack_base; current++)
	{
		void* ptr = *((void**)current);
		if(allocated_chunks.contains(ptr))
		{
			is_pointed.insert(ptr, true);
			referenced_chunks.push_front(ptr);
		}
	}

	{
		Hash_set<void*>::Set_iterator<void*> it = allocated_chunks.begin();
		Hash_set<void*>::Set_iterator<void*> end = allocated_chunks.end();

		while (it != end)
		{
			if (!is_pointed.contains(it.getValue()))
			{
				is_pointed.insert(it.getValue(), false);
			}
			it++;
		}
	}

	List<void*>::List_iterator<void*> it = referenced_chunks.begin();
	List<void*>::List_iterator<void*> end = referenced_chunks.end();

	while(it != end)
	{
		find_chunks(it.getValue(), is_pointed);
		it++;
	}

	free_memory(is_pointed);
}
