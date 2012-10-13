#include "magicMap.h"


vector<int> &MagicMap::operator[] (vector<int> &key) {
    for (size_t i = 0; i < keys.size(); i++) {
        if (calculateDeviation(keys[i], key) <= tolerance)
            return values[i];
    }

    // If we got here, nothing was found, add key to keys
    vector<int> v;
    keys.push_back(key);
    values.push_back(v);
    return values.back();
}

vector<int> MagicMap::getLargestKey() {
    vector<int> result;
    int largestResult = 0;
    int largestResultIndex = 0;
    for (size_t i = 0; i < keys.size(); i++) {
        int valueSize = values[i].size();
        if (valueSize > largestResult) {
            largestResult = valueSize;
            largestResultIndex = i;
        }
    }
    return keys[largestResultIndex];
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
