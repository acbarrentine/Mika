#pragma once

class MemoryManager
{
protected:
	const static int SBlockSize = 10 * 1024 * 1024;

	std::vector<char*> mBlocks;
	size_t mBlockOffset;
	void AllocateBlock();

public:
	MemoryManager();
	~MemoryManager();

	void Reset();
	void* GetMem(size_t bytes);
};

extern MemoryManager GMemoryManager;

class ManagedEntity
{
public:
	void* operator new(size_t num_bytes) { return GMemoryManager.GetMem(num_bytes); }
	void operator delete(void*) { }
};

