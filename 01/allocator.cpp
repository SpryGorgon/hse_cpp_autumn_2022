#include <iostream>
#include "allocator.hpp"

Allocator::Allocator()
{
	offset = 0;
	size = 0;
	arr = nullptr;
}

void Allocator::makeAllocator(size_t maxSize)
{
	if (arr != nullptr) delete[] arr;
	arr = new char[maxSize];
	offset = 0;
	size = maxSize;
}

char* Allocator::alloc(size_t size)
{
	if (size + offset > this->size) return nullptr;
	if (size == 0) return nullptr;
	char* ptr = (arr + offset);
	offset += size;
	return ptr;
}
void Allocator::reset()
{
	offset = 0;
}
// Это деструктор!
Allocator::~Allocator()
{
	delete[] arr;
}
