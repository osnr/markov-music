#ifndef MAGICMAP_H
#define MAGICMAP_H

#include <vector>

using namespace std;

class MagicMap {
    vector<vector<vector<int> > > hash;
    vector< vector<int> > values;
    float tolerance;
    bool calculateDeviation(vector<int> &a, vector<int> &b);

public:
    MagicMap(float atolerance) {
        tolerance = atolerance;
        hash.reserve(10000);
        for (int i = 0; i < 10000; i++) {
            vector<vector<int> > keys;
            keys.reserve(1000);
            hash.push_back(keys);
        }
        values.reserve(10000000);
    };
    ~MagicMap() {};
    vector<int> &operator[] (vector<int> &key);
    vector<int> &get(vector<int> &seed, int order);
    vector<int> getLargestKey();
};

#endif
