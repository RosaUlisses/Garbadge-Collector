### Garbage-Collector

-> An implementation of a garbage collector on C/C++

It is important to say that the collector works in the in the main thread, that said, when the function "heap_collect()" is called, the program "stop". Besides that, before using the "heap_collect()" it is necessary to call "initCollector()" and pass a unsigned char* that will be used as the begining of the stack.


# How it works ?

The garbage collector free the memory by iterating throught the stack and checking if the values stored in this segment of memory are pointers to chunks on the heap. The collector also confer if the allocated chunks are pointing to other chunks that were allocated on the heap. In that way, we avoid memory leaks that happen when we free the root of a tree, before deleting his children, for example.

