//Markov Model Header File
#ifndef _MAPMARKOV
#define _MAPMARKOV

#include <iostream>
#include <vector>

using namespace std;

void markovGeneration(vector< vector<int> > &inputs, vector<char *> &inputNames, char* outFile, int order, int inputSizeLimit, int outputSize, int fuzzMultiple, int beatLength, int beatModifierStrength, bool unify);
#endif
