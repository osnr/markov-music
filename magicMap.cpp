#include "magicMap.h"

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
    fuzzVector(key,fuzzMultiple);
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
    fuzzVector(key,fuzzMultiple);
    int hashNumber = vectorHashNumber(key);
    vector<vector<int> > *keys = &hash[hashNumber];
    vector<vector<int> > *values = &hashValues[hashNumber];
    //printf("Bucket Size %i\n",keys->size());
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key))
            return values->at(i);
    }


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

double MagicMap::calculateScore(vector<int> &a, vector<int> &b) {
    double avgScore = 0;
    for (size_t i = 0; i < a.size(); i++) {
        avgScore += abs(a[i]-b[i]);
    }
    return avgScore / a.size();
}

vector<int> MagicMap::getMostSimilarKey(vector<int> &check, int order) {
    vector<int> key;
    for (int i = check.size() - order; i < check.size(); i++) {
        key.push_back(check[i]);
    }
    double closestResult = 1000000;
    int closestResultHash = 0;
    int closestResultIndex = 0;
    for (size_t h = 0; h < hash.size(); h++) {
        vector<vector<int> > keys = hash[h];
        vector<vector<int> > values = hashValues[h];
        for (size_t i = 0; i < keys.size(); i++) {
            int valueSize = calculateScore(values[i],key);
            if (valueSize < closestResult) {
                closestResult = valueSize;
                closestResultHash = h;
                closestResultIndex = i;
            }
        }
    }
    return hash[closestResultHash][closestResultIndex];
}


bool MagicMap::calculateDeviation(vector<int> &a, vector<int> &b) {
    for (size_t i = 0; i < a.size(); i++) {
        //Special case for zero
        if (a[i] != b[i]) return false;
    }
    return true;

}


bool MagicMap::ReadFromFile(const char *dir, int i) {
    char buf[512];// todo this could overflow if i is really high
    sprintf(buf, "%s/%i.magic", dir, i);
    FILE *fr = fopen(buf, "r");
    if (!fr) {
        return false;
    }

    hash.clear();
    hashValues.clear();

    char line[512];
    while (fgets(line, 512, fr)) {
        if (line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0';

        if (strcmp(line, "1")==0) {
            vector<vector<int > > v;
            hash.push_back(v);
        } else if (strcmp(line, "2")==0) {
            vector<int> v;
            hash.back().push_back(v);
        } else if (strstr(line, "3 ") == line) {
            hash.back().back().push_back(atoi(line + strlen("3 ")));
        } else if (strcmp(line, "4")==0) {
            vector<vector<int > > v;
            hashValues.push_back(v);
        } else if (strcmp(line, "5")==0) {
            vector<int> v;
            hashValues.back().push_back(v);
        } else if (strstr(line, "6 ") == line) {
            hashValues.back().back().push_back(atoi(line + strlen("6 ")));
        }
    }
    
    fclose(fr);
    printf("Read MagicMap from %s\n", buf);
    return true;
}


void MagicMap::SaveToFile(const char *dir, int i) {
    char buf[512];// todo this could overflow if i is really high
    sprintf(buf, "%s/%i.magic", dir, i);
    FILE *fw = fopen(buf, "w");
    if (!fw) {
        printf("error: can't open %s for writing\n", buf);
        throw;
    }

    printf("hash size %i\n", hash.size());

    for (size_t i = 0; i < hash.size(); i++) {
        fprintf(fw, "1\n");
        for (size_t j = 0; j < hash[i].size(); j++) {
            fprintf(fw, "2\n");
            for (size_t k = 0; k < hash[i][j].size(); k++) {
                fprintf(fw, "3 %i\n", hash[i][j][k]);
            }
        }
    }

    for (size_t i = 0; i < hashValues.size(); i++) {
        fprintf(fw, "4\n");
        for (size_t j = 0; j < hashValues[i].size(); j++) {
            fprintf(fw, "5\n");
            for (size_t k = 0; k < hashValues[i][j].size(); k++) {
                fprintf(fw, "6 %i\n", hashValues[i][j][k]);
            }
        }
    }

    fclose(fw);
    printf("Saved MagicMap to %s\n", buf);
}
