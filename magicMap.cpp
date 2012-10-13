#include "magicMap.h"


vector<int> &MagicMap::operator[] (vector<int> &key) {
    int hashNumber = 1;
    int fuzzMultiple = 2000;
    //printf("Fuzzing Key: ");
    for (size_t i = 0; i < key.size(); i++) {
        key[i] = ((int)((double)key[i]/fuzzMultiple))*fuzzMultiple;
        //printf("%i-",key[i]);
        hashNumber += key[i];
        if (i % 3 == 0) {
            hashNumber = (hashNumber << 1);
        }
    }
    //printf("\n");
    hashNumber = hashNumber % hash.size();
    vector<vector<int> > *keys = &hash[hashNumber];
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key))
            return values[i];
    }

    // If we got here, nothing was found, add key to keys
    vector<int> v;
    v.reserve(100);
    keys->push_back(key);
    values.push_back(v);
    return values.back();
}

vector<int> &MagicMap::get(vector<int> &seed, int order) {
    int hashNumber = 1;
    int fuzzMultiple = 20000;
    //printf("Fuzzing Key: ");
    vector<int> key;
    key.reserve(seed.size());
    for (size_t i = seed.size()-order; i < seed.size(); i++) {
        key.push_back(((int)((double)seed[i]/fuzzMultiple))*fuzzMultiple);
        //printf("%i-",key[i]);
        hashNumber += key.back();
        if (i % 3 == 0) {
            hashNumber = (hashNumber << 1);
        }
    }
    //printf("\n");
    hashNumber = hashNumber % hash.size();
    vector<vector<int> > *keys = &hash[hashNumber];
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key))
            return values[i];
    }

    // If we got here, nothing was found
    vector<int> v;
    return v;
}

vector<int> MagicMap::getLargestKey() {
    vector<int> result;
    int largestResult = 0;
    int largestResultHash = 0;
    int largestResultIndex = 0;
    for (size_t h = 0; h < hash.size(); h++) {
        vector<vector<int> > keys = hash[h];
        for (size_t i = 0; i < keys.size(); i++) {
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


bool MagicMap::calculateDeviation(vector<int> &a, vector<int> &b) {
    for (size_t i = 0; i < a.size(); i++) {
        //Special case for zero
        if (a[i] != b[i]) return false;
    }
    return true;

}
