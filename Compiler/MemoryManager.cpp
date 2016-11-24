#include "stdafx.h"
#include "MemoryManager.h"

MemoryManager GMemoryManager;

void MemoryManager::AllocateBlock()
{
	char* block = (char*) malloc(SBlockSize);
	assert(block);
	mBlocks.push_back(block);
	mBlockOffset = 0;
}

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
	Reset();
}

void MemoryManager::Reset()
{
	for (void* block : mBlocks)
	{
		free(block);
	}
	mBlocks.clear();
}

void* MemoryManager::GetMem(size_t bytes)
{
	if (mBlocks.empty() || mBlockOffset + bytes > SBlockSize)
	{
		AllocateBlock();
	}

	char* block = mBlocks.back();
	void* retVal = block + mBlockOffset;
	mBlockOffset += bytes;
	return retVal;
}
