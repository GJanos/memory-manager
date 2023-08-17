#include <iostream>
#include "MList.h"

using namespace std;

ITEM::ITEM(int addr, int size) : after(nullptr), before(nullptr), _addr(addr), _size(size)
{
}

MList::MList(int memstr, int maxs) : head(nullptr), memStart(memstr), maxSize(maxs)
{
}

MList::~MList()
{
    ITEM *iter = head;
    while (iter != NULL)
    {
        ITEM *next = iter->after;
        delete iter;
        iter = next;
    }
    head = iter;
}

int MList::size()
{
    if (!head)
        return 0;
    int counter = 1;
    ITEM *iter = head;
    while (iter->after)
    {
        iter = iter->after;
        counter++;
    }
    return counter;
}

void MList::print()
{
    if (!head)
        return;

    int cnt = 1;
    ITEM *iter = head;
    while (iter)
    {
        cout << cnt++ << ".(" << iter->_addr << "," << iter->_size << ")->";
        iter = iter->after;
    }
    cout << "NULL";
    cout << endl;
}

ITEM *MList::findByAddr(int addr)
{
    ITEM *iter = head;
    while (iter->_addr != addr && iter)
        iter = iter->after;

    if (!iter)
        return nullptr;

    return iter;
}
