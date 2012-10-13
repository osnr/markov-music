#ifndef MAGICMAP_H
#define MAGICMAP_H

#include <vector>

using namespace std;

class MagicMap {
    vector< vector<int> > keys;
    vector< vector<int> > values;
    float tolerance;
    float calculateDeviation(vector<int> &a, vector<int> &b);

public:
    MagicMap(float atolerance) {
        tolerance = atolerance;
    };
    ~MagicMap() {};
    vector<int> &operator[] (vector<int> &key);
    vector<int> getLargestKey();
};

#endif
