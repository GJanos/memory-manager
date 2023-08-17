#pragma once

class ITEM
{
public:
    ITEM *after;
    ITEM *before;
    int _addr;
    int _size;
    ITEM(int addr, int size);
};

class MList
{
public:
    ITEM *head;
    int memStart;
    int maxSize;

    MList(int memstr, int maxs);
    ~MList();
    void cleanList();

    int size();
    void print();
    ITEM *findByAddr(int addr);
};
