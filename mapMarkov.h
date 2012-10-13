//Markov Model Header File
#ifndef _MAPMARKOV
#define _MAPMARKOV

#include <iostream>
#include <vector>

using namespace std;

void markovGeneration(vector< vector<int> > &inputs, char* outFile, int order, int inputSizeLimit, int outputSize);

#endif
