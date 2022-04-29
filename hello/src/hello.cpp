#include <iostream>
#include "collections/vector.h"

using namespace std;

int main() {
    Vector<int> tt;
    tt.add(8);
    tt.add(8);
    std::cout << tt << std::endl;
    return 0;
}
