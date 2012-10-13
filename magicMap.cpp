#include "magicMap.h"

int inline MagicMap::vectorHashNumber (vector<slice> &key) {
    int hashNumber = 1;
    for (size_t i = 0; i < key.size(); i++) {
        hashNumber += key[i].value;
        hashNumber *= key[i].value;
        hashNumber *= key[i].beatFalloff;
    }
    return hashNumber % hash.size();
}

void fuzzVector(vector<slice> &key,int fuzzMultiple) {
    for (size_t i = 0; i < key.size(); i++) {
        key[i].value = ((int)((double)key[i].value/fuzzMultiple))*fuzzMultiple;
    }
}

vector<slice> &MagicMap::operator[] (vector<slice> &key) {
    fuzzVector(key,fuzzMultiple);
    int hashNumber = vectorHashNumber(key);
    vector<vector<slice> > *keys = &hash[hashNumber];
    vector<vector<slice> > *values = &hashValues[hashNumber];
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key))
            return values->at(i);
    }

    // If we got here, nothing was found, add key to keys
    vector<slice> v;
    v.reserve(100);
    keys->push_back(key);
    values->push_back(v);
    return values->back();
}

vector<slice> &MagicMap::get(vector<slice> &seed, int order) {
    vector<slice> key;
    key.reserve(order);
    for (size_t i = seed.size()-order; i < seed.size(); i++) {
        key.push_back(seed[i]);
    }
    fuzzVector(key,fuzzMultiple);
    int hashNumber = vectorHashNumber(key);
    vector<vector<slice> > *keys = &hash[hashNumber];
    vector<vector<slice> > *values = &hashValues[hashNumber];
    //printf("Bucket Size %i\n",keys->size());
    for (size_t i = 0; i < keys->size(); i++) {
        if (calculateDeviation(keys->at(i), key))
            return values->at(i);
    }


    vector<slice> v;
    return v;
}

void MagicMap::debugModel() {
    for (size_t h = 0; h < hash.size(); h++) {
        vector<vector<slice> > keys = hash[h];
        vector<vector<slice> > values = hashValues[h];
        for (size_t i = 0; i < keys.size(); i++) {
            printf("Key: ");
            for (size_t k = 0; k < keys[i].size(); k++) {
                printf("%i",keys[i][k].value);
            }
            printf(": ");
            for (size_t n = 0; n < values[i].size(); n++) {
                printf("%i,",values[i][n].value);
            }
            printf("\n");
        }
    }
}

vector<slice> MagicMap::getLargestKey() {
    vector<slice> result;
    int largestResult = 0;
    int largestResultHash = 0;
    int largestResultIndex = 0;
    for (size_t h = 0; h < hash.size(); h++) {
        vector<vector<slice> > keys = hash[h];
        vector<vector<slice> > values = hashValues[h];
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

double MagicMap::calculateScore(vector<slice> &a, vector<slice> &b) {
    double avgScore = 0;
    for (size_t i = 0; i < a.size(); i++) {
        avgScore += abs(a[i].value-b[i].value);
    }
    return avgScore / a.size();
}

vector<slice> MagicMap::getMostSimilarKey(vector<slice> &check, int order) {
    vector<slice> key;
    for (int i = check.size() - order; i < check.size(); i++) {
        key.push_back(check[i]);
    }
    double closestResult = 1000000;
    int closestResultHash = 0;
    int closestResultIndex = 0;
    for (size_t h = 0; h < hash.size(); h++) {
        vector<vector<slice> > keys = hash[h];
        for (size_t i = 0; i < keys.size(); i++) {
            int valueSize = calculateScore(keys[i],key);
            if (valueSize < closestResult) {
                closestResult = valueSize;
                closestResultHash = h;
                closestResultIndex = i;
            }
        }
    }
    return hash[closestResultHash][closestResultIndex];
}


bool MagicMap::calculateDeviation(vector<slice> &a, vector<slice> &b) {
    for (size_t i = 0; i < a.size(); i++) {
        //Special case for zero
        if (a[i].value != b[i].value) return false;
        int falloff = a[i].beatFalloff - b[i].beatFalloff;
        if (falloff > 0 || falloff < -1000) return false;
    }
    return true;

}


void strchrreplace(char *buf, char o, char n) {
    bool first = true;
    while (*buf != 0) {
        if (*buf == o) {
            if (first)
                first = false;
            else
                *buf = n;
        }
        buf++;
    }
}


bool MagicMap::ReadFromFile(const char *dir, const char *name) {
    char *buf = (char *)malloc(strlen(".magic") + strlen(dir) + 1 + strlen(name) + 1);
    sprintf(buf, "%s/%s.magic", dir, name);
    strchrreplace(buf, '/', '-');
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
            vector<vector<slice > > v;
            hash.push_back(v);
        } else if (strcmp(line, "2")==0) {
            vector<slice> v;
            hash.back().push_back(v);
        } else if (strstr(line, "3 ") == line) {
            slice k;
            char *endptr = NULL;
            k.value = strtod(line + strlen("3 "), &endptr);
            k.beatFalloff = strtod(endptr, NULL);
            hash.back().back().push_back(k);
        } else if (strcmp(line, "4")==0) {
            vector<vector<slice > > v;
            hashValues.push_back(v);
        } else if (strcmp(line, "5")==0) {
            vector<slice> v;
            hashValues.back().push_back(v);
        } else if (strstr(line, "6 ") == line) {
            slice k;
            char *endptr = NULL;
            k.value = strtod(line + strlen("6 "), &endptr);
            k.beatFalloff = strtod(endptr, NULL);
            hashValues.back().back().push_back(k);
        }
    }
    
    fclose(fr);
    printf("Read MagicMap from %s\n", buf);
    free(buf);
    return true;
}


void MagicMap::SaveToFile(const char *dir, const char *name) {
    char *buf = (char *)malloc(strlen(".magic") + strlen(dir) + 1 + strlen(name) + 1);
    sprintf(buf, "%s/%s.magic", dir, name);
    strchrreplace(buf, '/', '-');
    FILE *fw = fopen(buf, "w");
    if (!fw) {
        printf("error: can't open %s for writing\n", buf);
        throw;
    }

    printf("\nhash size %i\n", hash.size());

    for (size_t i = 0; i < hash.size(); i++) {
        fprintf(fw, "1\n");
        for (size_t j = 0; j < hash[i].size(); j++) {
            fprintf(fw, "2\n");
            for (size_t k = 0; k < hash[i][j].size(); k++) {
                fprintf(fw, "3 %i %i\n", hash[i][j][k].value, hash[i][j][k].beatFalloff);
            }
        }
    }

    for (size_t i = 0; i < hashValues.size(); i++) {
        fprintf(fw, "4\n");
        for (size_t j = 0; j < hashValues[i].size(); j++) {
            fprintf(fw, "5\n");
            for (size_t k = 0; k < hashValues[i][j].size(); k++) {
                /////TODO: Save the rest of the elements of slice
                fprintf(fw, "6 %i %i\n", hashValues[i][j][k].value, hashValues[i][j][k].beatFalloff);
            }
        }
    }

    fclose(fw);
    printf("Saved MagicMap to %s\n", buf);
    free(buf);
}
