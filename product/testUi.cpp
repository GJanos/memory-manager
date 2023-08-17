#include "testUi.h"
#include <iostream>

using namespace std;

int testUi::getNumber(string message)
{
    int choice = 0;

    if (withMenu == true)
    {
        cout << message;
    }
    cin >> choice;
    return choice;
}

void testUi::showList(MList *list)
{
    ITEM *item = list->head;
    if (!list->head)
    {
        cout << "  <empty>" << endl;
    }
    int i = 0;
    while (item)
    {
        char s[100];
        sprintf(s, "%3d:  addr:%4d  size:%4d", i, item->_addr, item->_size);
        cout << s << endl;
        item = item->after;
        i++;
    }
}

void testUi::showLists(MList *allocList, MList *freeList)
{
    cout << "FreeList:" << endl;
    cout << "---------" << endl;
    showList(freeList);
    cout << "AllocList:" << endl;
    cout << "----------" << endl;
    showList(allocList);
}

void testUi::startUi(MemoryManager *mm)
{
    bool quit = false;

    cout << endl
         << "SD 'MemoryC++' (version 1)" << endl;
    cout << "-------------------------" << endl;
    while (quit == false)
    {
        if (withMenu == true)
        {
            cout << endl;
            cout << "MENU" << endl;
            cout << "===================" << endl;
            cout << "[1] allocate memory" << endl;
            cout << "[2] free memory" << endl;
            cout << "[3] show lists" << endl;
            cout << "[8] show/hide menu" << endl;
            cout << "[9] quit" << endl
                 << endl;
        }
        int choice = getNumber("choice: ");
        char string[100];

        switch (choice)
        {
        case 1:
        {
            int nrOfBytes = getNumber("Give nr of bytes: ");
            int addr = mm->claimMemory(nrOfBytes);
            if (addr == -1)
            {
                cout << "[ALLOC] not enough memory for " << nrOfBytes << " byte" << endl;
            }
            else
            {
                cout << "[ALLOC] address: " << addr << " (" << nrOfBytes << " byte)" << endl;
            }
        }
        break;
        case 2:
        {
            int addr = getNumber("Give address to be freed: ");
            int nrOfBytes = mm->freeMemory(addr);
            if (nrOfBytes == -1)
            {
                sprintf(string, "[FREE]  address: %4d was not allocated", addr);
            }
            else
            {
                sprintf(string, "[FREE]  address: %4d (%d byte)", addr, nrOfBytes);
            }
            cout << string << endl;
        }
        break;
        case 3:
            showLists(mm->allocList, mm->freeList);
            break;
        case 8:
            if (withMenu == true)
            {
                withMenu = false;
                cout << "printing of MENU is diabled" << endl;
            }
            else
            {
                // printing enabled
                withMenu = true;
            }
            break;

        case 9:
            quit = true;
            break;

        default:
            cout << "invalid choice: " << choice << endl;
            break;
        }
    }
}