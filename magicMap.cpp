#include "magicMap.h"

#define FUZZ_MULTIPLE 1000000

int inline MagicMap::vectorHashNumber (vector<int> &key) {
    int hashNumber = 1;
    for (size_t i = 0; i < key.size(); i++) {
        hashNumber += key[i];
        hashNumber *= key[i];
    }
    return hashNumber % hash.size();
}

void fuzzVector(vector<int> &key,int fuzzMultiple) {
    for (size_t i = 0; i < key.size(); i++) {
        key[i] = ((int)((double)key[i]/fuzzMultiple))*fuzzMultiple;
    }
}

vector<int> &MagicMap::operator[] (vector<int> &key) {
    fuzzVector(key,FUZZ_MULTIPLE);
    int hashNumber = vectorHashNumber(key);
    vector<vector<int> > *keys = &hash[hashNumber];
    vector<vector<int> > *values = &hashValues[hashNumber];
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key))
            return values->at(i);
    }

    // If we got here, nothing was found, add key to keys
    vector<int> v;
    v.reserve(100);
    keys->push_back(key);
    values->push_back(v);
    return values->back();
}

vector<int> &MagicMap::get(vector<int> &seed, int order) {
    vector<int> key;
    key.reserve(order);
    for (size_t i = seed.size()-order; i < seed.size(); i++) {
        key.push_back(seed[i]);
    }
    fuzzVector(key,FUZZ_MULTIPLE);
    int hashNumber = vectorHashNumber(key);
    vector<vector<int> > *keys = &hash[hashNumber];
    vector<vector<int> > *values = &hashValues[hashNumber];
    //printf("Bucket Size %i\n",keys->size());
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key))
            return values->at(i);
    }

    printf("\nRAN OUT OF POSSIBILITIES\n");
    fflush(stdout);

    vector<int> v;
    return v;
}

void MagicMap::debugModel() {
    for (size_t h = 0; h < hash.size(); h++) {
        vector<vector<int> > keys = hash[h];
        vector<vector<int> > values = hashValues[h];
        for (size_t i = 0; i < keys.size(); i++) {
            printf("Key: ");
            for (size_t k = 0; k < keys[i].size(); k++) {
                printf("%i",keys[i][k]);
            }
            printf(": ");
            for (size_t n = 0; n < values[i].size(); n++) {
                printf("%i,",values[i][n]);
            }
            printf("\n");
        }
    }
}

vector<int> MagicMap::getLargestKey() {
    vector<int> result;
    int largestResult = 0;
    int largestResultHash = 0;
    int largestResultIndex = 0;
    for (size_t h = 0; h < hash.size(); h++) {
        vector<vector<int> > keys = hash[h];
        vector<vector<int> > values = hashValues[h];
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
    /*printf("\nDeviation for sets size %i: ",a.size());
    for (int i = 0; i < a.size(); i++) {
        printf("%i,",a[i]);
    }
    printf("\n");
    for (int i = 0; i < a.size(); i++) {
        printf("%i,",b[i]);
    }
    printf("\n");*/
    for (size_t i = 0; i < a.size(); i++) {
        //Special case for zero
        if (a[i] != b[i]) return false;
    }
    return true;

}


bool MagicMap::ReadFromFile(const char *fname) {
    return false; // TODO
}


void MagicMap::SaveToFile(const char *fname) {
    return; // TODO
}
