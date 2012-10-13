#ifndef MAGICMAP_H
#define MAGICMAP_H

#include <vector>

using namespace std;

typedef struct slice {
    int value;
    int beatFalloff;
} slice;

class MagicMap {
    vector<vector<vector<slice> > > hash;
    vector<vector<vector<slice> > > hashValues;
    float tolerance;
    int fuzzMultiple;
    bool calculateDeviation(vector<slice> &a, vector<slice> &b);

public:
    MagicMap(float atolerance,int fuzz) {
        fuzzMultiple = fuzz;
        tolerance = atolerance;
        hash.reserve(10000);
        for (int i = 0; i < 10000; i++) {
            vector<vector<slice> > keys;
            keys.reserve(1000);
            hash.push_back(keys);
        }
        hashValues.reserve(10000);
        for (int i = 0; i < 10000; i++) {
            vector<vector<slice> > values;
            values.reserve(1000);
            hashValues.push_back(values);
        }
    };
    ~MagicMap() {};
    vector<slice> &operator[] (vector<slice> &key);
    vector<slice> &get(vector<slice> &seed, int order);
    vector<slice> getLargestKey();
    double calculateScore(vector<slice> &a, vector<slice> &b);
    vector<slice> getMostSimilarKey(vector<slice> &check, int order);
    void debugModel();
    int inline vectorHashNumber (vector<slice> &key);

    bool ReadFromFile(const char *fname, const char *name);
    void SaveToFile(const char *fname, const char *name);
};

#endif
