#include "WAVio.h"
#include "magicMap.h"
#include "mapMarkov.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

vector<int> *getTestVector() {
    vector<int> *samples = new vector<int>;
    samples->push_back(1);
    samples->push_back(2);
    samples->push_back(3);
    samples->push_back(2);
    samples->push_back(1);
    samples->push_back(2);
    samples->push_back(3);
    samples->push_back(2);
    samples->push_back(2);
    samples->push_back(2);
    samples->push_back(2);
    samples->push_back(2);
    samples->push_back(1);
    samples->push_back(2);
    samples->push_back(3);
    samples->push_back(2);
    samples->push_back(1);
    samples->push_back(2);
    samples->push_back(3);
    samples->push_back(2);
    samples->push_back(1);
    samples->push_back(2);
    samples->push_back(3);
    samples->push_back(2);
    samples->push_back(1);
    samples->push_back(2);
    samples->push_back(3);
    samples->push_back(2);
    return samples;
}

void markovGeneration(char* inFile, char* outFile, int order, int outputSize) {
    vector<int> *input = readSamplesFromWAV(inFile);
    //vector<int> *input = getTestVector();
        
    MagicMap model(0);
    vector<int> history;

    //Get starting seed
    int mostFrequentKeyCount = 0;
    vector<int> seed;

    cout << "Building Model of size " << input->size() << " (one dot per 1000 inputs): "<<endl;
    //Build the model
    for (int i = 0; i < input->size(); i++) {
        if (i < 100) {
            cout << input->at(i) << ",";
        }
        if (i % 1000 == 0) cout << ".";
        if (history.size() >= order) {
            model[history].push_back(input->at(i));
            //Check for the seed value
            //TODO: This is slightly slower than the most frequent at the end, b/c of redundancies

            if (model[history].size() > mostFrequentKeyCount) {
                mostFrequentKeyCount = model[history].size();
                seed = history;
            }
            //Shift the history frame over 1 element
            history.erase(history.begin());
        }
        //Add to the history
        history.push_back(input->at(i));
    }

    //free the input memory
    delete input;

    //We're repurposing the history to do generation
    history = seed;

    //Seed the random number generator
    srand( time(NULL));

    cout << endl << endl << "Generating Final Output of size " << outputSize << " (one dot per 1000 outputs): " << endl;

    //Do the generation
    for (int i = 0; i < outputSize; i++) {
        if (i % 1000 == 0) cout << ".";
        //Get a random int from the options to follow this history set
        int addInt = model[history][rand() % model[history].size()];
        if (i < 100) {
            cout << addInt << ",";
        }
        //Add it to the result
        seed.push_back(addInt);
        //Shift the history frame
        history.push_back(addInt);
        history.erase(history.begin());
    }
    cout << endl;

    writeSamplesToWAV(&seed,outFile);
}

