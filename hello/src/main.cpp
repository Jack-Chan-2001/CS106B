#include <iostream>
#include "collections/vector.h"
#include "collections/linkedlist.h"

using namespace std;

int main() {
    LinkedList<int> list;
    for (int i = 0; i < 8; i++) {
        list.add(i + 5);
    }
    cout << list << endl;

    return 0;
}
