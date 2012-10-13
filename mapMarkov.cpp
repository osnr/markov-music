#include "WAVio.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

void markovGeneration(vector<int> input) {
    MagicMap model = new MagicMap(0);
    vector<int> history;

    //Get starting seed
    int mostFrequentKeyCount = 0;
    vector<int> seed;

    int order = 10;

    //Build the model
    for (int i = 0; i < input.size(); i++) {
        if (history.size() >= order) {
            model[history].push_back(input[i]);
            //Check for the seed value
            //TODO: This is slightly slower than the most frequent at the end, b/c of redundancies
            if (model[history].size() > mostFrequentKeyCount) {
                mostFrequentKeyCount = model[history].size();
                seed = model[history];
            }
            //Shift the history frame over 1 element
            history.erase(history.begin());
        }
        //Add to the history
        history.push_back(input[i]);
    }

    int outputSize = 100;
    
    //We're repurposing the history to do generation
    history = seed;

    //Seed the random number generator
    srand( time(NULL));

    //Do the generation
    for (int i = 0; i < outputSize; i++) {
        //Get a random int from the options to follow this history set
        int addInt = model[history][rand() % model[history].size()];
        //Add it to the result
        seed.push_back(addInt);
        //Shift the history frame
        history.push_back(addInt);
        history.erase(history.begin());
    }
}

