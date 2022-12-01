#pragma once

class Allocator
{
private:
	size_t offset;
	size_t size;
	char* arr;
public:
	Allocator();
	void makeAllocator(size_t maxSize);
	char* alloc(size_t size);
	void reset();
	// Это деструктор!
	~Allocator();
};