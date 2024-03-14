#include "SkipList.h"

using namespace std;

int main()
{
    Util::SkipList<int, int> skipList(8);
    skipList.insertElement(1, 1);
    skipList.insertElement(2, 1);
    skipList.insertElement(3, 1);
    skipList.insertElement(4, 1);
    skipList.insertElement(5, 1);
    skipList.insertElement(5, 1);
    skipList.displayList();

    skipList.deleteElement(3);
    skipList.displayList();

    skipList.deleteElement(5);
    skipList.displayList();

    skipList.deleteElement(1);
    skipList.displayList();

    skipList.deleteElement(2);
    skipList.displayList();

    skipList.deleteElement(4);
    skipList.displayList();

    skipList.deleteElement(4);
}