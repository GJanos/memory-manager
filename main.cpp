#include <iostream>
#include "MemoryManager.h"
#include "MList.h"
#include "testUi.h"

using namespace std;

int main()
{
    MemoryManager *mm = new MemoryManager();
    testUi *tui = new testUi();
    tui->startUi(mm);
    delete mm;
    delete tui;
    return 0;
}