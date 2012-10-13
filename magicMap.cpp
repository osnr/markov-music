#include "magicMap.h"


vector<int> &MagicMap::operator[] (vector<int> &key) {
    int hashNumber = 1;
    for (size_t i = 0; i < key.size(); i++) {
        hashNumber += key[i];
        if (i % 3 == 0) {
            hashNumber = (hashNumber << 1);
        }
    }
    hashNumber = hashNumber % hash.size();
    vector<vector<int> > *keys = &hash[hashNumber];
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key) <= tolerance)
            return values[i];
    }

    // If we got here, nothing was found, add key to keys
    vector<int> v;
    v.reserve(100);
    keys->push_back(key);
    values.push_back(v);
    return values.back();
}

vector<int> MagicMap::getLargestKey() {
    vector<int> result;
    int largestResult = 0;
    int largestResultHash = 0;
    int largestResultIndex = 0;
    printf("Getting largest in hash size %i:\n",hash.size());
    for (size_t h = 0; h < hash.size(); h++) {
        printf("Hash Bucket %i, size %i\n",h,hash[h].size());
        vector<vector<int> > keys = hash[h];
        for (size_t i = 0; i < keys.size(); i++) {
            printf("Key %i\n",i);
            int valueSize = values[i].size();
            if (valueSize > largestResult) {
                largestResult = valueSize;
                largestResultHash = h;
                largestResultIndex = i;
            }
        }
    }
    return hash[largestResultHash][largestResultIndex];
}


float MagicMap::calculateDeviation(vector<int> &a, vector<int> &b) {
    if (a.size() != b.size()) {
        printf("keys don't have the same size (%i vs %i)\n", (int)a.size(), (int)b.size());
        throw;
    }

    int totalDev = 0;
    for (size_t i = 0; i < a.size(); i++) {
        totalDev += abs(a[i] - b[i]);
        //Special case for zero
        if (totalDev > 0) break;
    }

    return (float)totalDev / (float)a.size();
}
