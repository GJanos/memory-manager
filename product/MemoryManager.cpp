#include "MemoryManager.h"
#include "MList.h"
#include <iostream>

using namespace std;

#define log(x) std::cout << x << " " << endl;

/* Starting point for MemoryManager constructor */
MemoryManager::MemoryManager()
{
	allocList = new AllocList(memStart, maxSize);
	freeList = new FreeList(memStart, maxSize);
}

/* Code correct destructor to cleanup all memory */
MemoryManager::~MemoryManager()
{
	delete allocList;
	delete freeList;
}

/* pre : size > 0
 * post: If no memory block of size "size" available return -1
 *       Otherwise claimMemory() returns the first
 *       address in the freeList where a memory block of at least size is present.
 *       This memory is allocated and not any more available.
 *       The newly allocated block is added to the allocList and addr of the newly
 *       allocated block is returned
 */

int MemoryManager::claimMemory(int size)
{
	int addrOfNextChunk = freeList->enoughMemory(size);
	if (addrOfNextChunk == -1)
		return -1;
	freeList->reduceMemory(addrOfNextChunk, size);
	allocList->allocMemory(addrOfNextChunk, size);
	return addrOfNextChunk;
}

/* pre:  Parameter addr must match the start of an allocatd memory block. Otherwhise return -1.
 *
 * post: If addr is a part of allocated memory of allocList then the memory from this address
 * 	     with registered size should be removed from the allocList and added to the freeList
 *
 *       freeMemory(addr) returns number of bytes (size) belongig to the address addr
 *       In all other cases freeMemory(freeList,allocList,addr) returns -1.
 */

int MemoryManager::freeMemory(int addr)
{

	int isValid = allocList->valDeAlloc(addr);
	if (isValid == -1)
		return -1;
	return freeList->insertMemory(allocList->cutOutMemory(addr));
}

void MemoryManager::print()
{
	cout << "freeList: ";
	if (freeList->size() > 0)
		freeList->print();
	else
		cout << "NULL\n";

	cout << "allocList: ";
	if (allocList->size() > 0)
		allocList->print();
	else
		cout << "NULL\n";
	cout << "-----------\n";
}

////////////
// AllocList
////////////
AllocList::AllocList(int memstr, int maxs) : MList(memstr, maxs)
{
}

int AllocList::valDeAlloc(int addr)
{
	if (!head)
		return -1;
	ITEM *iter = head;

	do
	{
		if (iter->_addr == addr)
			return 0;

		iter = iter->after;
	} while (iter);
	return -1;
}

ITEM *AllocList::cutOutMemory(int addr)
{
	ITEM *iter = head;
	while (iter->_addr != addr)
		iter = iter->after;

	if (iter == head)
	{
		head = iter->after;
		// if the list is still not empty
		if (head)
			head->before = nullptr;
	}
	else if (!iter->after)
	{
		iter->before->after = nullptr;
	}
	else
	{
		iter->before->after = iter->after;
		iter->after->before = iter->before;
	}
	iter->after = nullptr;
	iter->before = nullptr;
	return iter;
}

void AllocList::allocMemory(int addr, int size)
{
	ITEM *newAlloc = new ITEM(addr, size);
	if (!head)
	{
		head = newAlloc;
		return;
	}

	ITEM *iter = head;
	while (iter->_addr < addr && iter->after)
		iter = iter->after;
	// here you had to check that wether the iter element's
	//(last element in this case)
	// addr > & < than the parameter addr. than act accordingly

	if (iter == head)
	{
		if (iter->_addr > newAlloc->_addr)
		{
			newAlloc->after = head;
			newAlloc->before = nullptr;
			head->before = newAlloc;
			head = newAlloc;
		}
		else
		{
			iter->after = newAlloc;
			newAlloc->before = iter;
			newAlloc->after = nullptr;
		}
	}
	else if (iter->_addr > newAlloc->_addr)
	{

		newAlloc->before = iter->before;
		newAlloc->after = iter;
		iter->before->after = newAlloc;
		iter->before = newAlloc;
	}
	else
	{
		iter->after = newAlloc;
		newAlloc->before = iter;
		newAlloc->after = nullptr;
	}
}

////////////
// FreeList
////////////
FreeList::FreeList(int memstr, int maxs) : MList(memstr, maxs)
{
	head = new ITEM(memstr, maxSize);
}

int FreeList::enoughMemory(int memorySize)
{
	if (!head)
		return memStart;

	ITEM *iter = head;
	do
	{
		if (iter->_size >= memorySize)
			return iter->_addr;

		iter = iter->after;
	} while (iter);

	return -1;
}

void FreeList::reduceMemory(int addr, int memorySize)
{
	if (!head)
	{
		ITEM *newAlloc = new ITEM(addr, memorySize);
		head = newAlloc;
	}
	ITEM *reducable = findByAddr(addr);

	if (memorySize < reducable->_size)
	{
		reducable->_addr += memorySize;
		reducable->_size -= memorySize;
	}
	else
	{
		if (reducable == head)
		{
			head = head->after;
		}
		else if (!reducable->after)
		{
			reducable->before->after = nullptr;
		}
		else
		{
			reducable->before->after = reducable->after;
			reducable->after->before = reducable->before;
		}
		delete reducable;
	}
}

// should return the size of the allocated chunk
int FreeList::insertMemory(ITEM *chunk)
{
	int retSize = chunk->_size;
	// if currently the freeList is empty
	if (!head)
	{

		chunk->before = nullptr;
		chunk->after = nullptr;
		head = chunk;
		return retSize;
	}
	ITEM *iter = head;
	while (iter->_addr < chunk->_addr && iter->after)
		iter = iter->after;

	if (iter == head)
	{
		if (iter->_addr > chunk->_addr)
		{
			chunk->after = head;
			chunk->before = nullptr;
			head->before = chunk;
			head = chunk;
		}
		else
		{
			iter->after = chunk;
			chunk->before = iter;
			chunk->after = nullptr;
		}
	}
	else if (iter->_addr > chunk->_addr)
	{
		chunk->before = iter->before;
		chunk->after = iter;
		iter->before->after = chunk;
		iter->before = chunk;
	}
	else
	{
		iter->after = chunk;
		chunk->before = iter;
		chunk->after = nullptr;
	}
	mergeMemory();
	return retSize;
}

void FreeList::mergeMemory()
{
	ITEM *iter = head;
	int cnt = 1;

	while (size() > cnt)
	{
		ITEM *temp = iter->after;
		if (iter->_addr + iter->_size == iter->after->_addr)
		{
			ITEM *left = iter;
			ITEM *right = iter->after;
			right->_addr = left->_addr;
			right->_size += left->_size;
			right->before = left->before;

			if (left == head)
				head = right;
			else
				left->before->after = right;

			cnt--;
			delete left;
		}
		iter = temp;
		cnt++;
	}
}
