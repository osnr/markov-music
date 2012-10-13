//Markov Model Header File
#ifndef _MARKOV
#define _MARKOV

#include <iostream>
#include <vector>

#define ORDER 100
#define FOLLOW 10
#define LENIENCY 100

using namespace std;

typedef struct progression {
  vector<int> *preceding;
  vector<vector<int>* > *following;
} progression;

bool precedingMatch(vector<int>* preceding1, vector<int>* preceding2);
void initMarkovModel(vector<int>* samples, vector<progression>* progs);
int markov();

#endif
