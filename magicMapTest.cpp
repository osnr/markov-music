/* Test using g++ magicMapTest.cpp; ./a.out */
#include "magicMap.h"
#include "magicMap.cpp"


int main() {
    MagicMap model(1);
    vector<int> testkey;
    testkey.push_back(1);
    testkey.push_back(2);
    testkey.push_back(3);

    model[testkey].push_back(42);
    
    printf("size (should be 1) : %i\n", (int)model[testkey].size());
}