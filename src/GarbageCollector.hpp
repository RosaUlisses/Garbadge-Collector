#pragma once
#ifndef GARBADGECOLLECTOR_H
#define GARBADGECOLLECTOR_H

#include "HashMap.h"
#include "HashSet.h"
#include "List.h"
#include <cstdlib>
#include <vector>



void* Malloc(size_t size);
void* Calloc(size_t num, size_t size);
void Free(void* ptr);


void heap_collect();

void initCollector(unsigned char* pointer);

#endif
