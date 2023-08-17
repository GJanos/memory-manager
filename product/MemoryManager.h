#pragma once

#include "MList.h"

/* Code functionality for the AllocList, think well which functionality is specific for AllocList and which is generic to MList */

class AllocList : public MList
{
public:
    // do a copy constructor
    AllocList(int memstr, int maxs);
    int valDeAlloc(int addr);
    ITEM *cutOutMemory(int addr);
    void allocMemory(int addr, int size);
};

/* Code functionality for the FreeList, think well which functionality is specific for FreeList and which is generic to MList */

class FreeList : public MList
{
public:
    // do a copy constructor
    FreeList(int memstr, int maxs);
    int enoughMemory(int memorySize);
    void reduceMemory(int addr, int memorySize);
    int insertMemory(ITEM *chunk);
    void mergeMemory();
};

class MemoryManager
{
public:
    const int memStart = 1000;
    const int maxSize = 100;
    AllocList *allocList;
    FreeList *freeList;

    MemoryManager();
    ~MemoryManager();
    int claimMemory(int size);
    int freeMemory(int addr);
    void print();
};
