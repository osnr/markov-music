#ifndef MAGICMAP_H
#define MAGICMAP_H

#include <vector>

using namespace std;

class MagicMap {
    vector<vector<vector<int> > > hash;
    vector< vector<int> > values;
    float tolerance;
    float calculateDeviation(vector<int> &a, vector<int> &b);

public:
    MagicMap(float atolerance) {
        tolerance = atolerance;
        hash.reserve(100);
        for (int i = 0; i < 10000; i++) {
            vector<vector<int> > keys;
            keys.reserve(1000);
            hash.push_back(keys);
        }
        values.reserve(100000);
    };
    ~MagicMap() {};
    vector<int> &operator[] (vector<int> &key);
    vector<int> getLargestKey();
};

#endif
