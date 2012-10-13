#ifndef MAGICMAP_H
#define MAGICMAP_H

#include <vector>

using namespace std;

class MagicMap {
    vector< vector<int> > keys;
    vector< vector<int> > values;
    int tolerance;

public:
    MagicMap(int atolerance) {
        tolerance = atolerance;
    };
    ~MagicMap() {};
    vector<int> &operator[] (vector<int> &key);
};

#endif
