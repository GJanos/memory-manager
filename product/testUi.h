#include "MemoryManager.h"
#include "MList.h"
#include <iostream>

using namespace std;

class testUi {
        int getNumber(string message);
        bool withMenu = true;
    public:
        void startUi(MemoryManager *mm);
        void showList(MList *list);
        void showLists(MList *freeList, MList *allocList);
};