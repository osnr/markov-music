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

void markovGeneration(char* inFile, char* outFile, int order, int inputSizeLimit, int outputSize) {
    vector<int> *input = readSamplesFromWAV(inFile);
    //vector<int> *input = getTestVector();
        
    MagicMap model(0);
    vector<int> history;

    int inputSize = input->size();
    if (inputSize > inputSizeLimit) inputSize = inputSizeLimit;

    cout << "Building Model of size " << inputSize << ": "<<endl;
    //Build the model
    for (int i = 0; i < inputSize; i++) {
        if (i % 1000 == 0) {
            printf("\r %i done (%i percent)",i,(int)(((double)i/(double)inputSize)*100));
            fflush(stdout);
        }
        if (history.size() >= order) {
            model[history].push_back(input->at(i));
            //Check for the seed value
            //TODO: This is slightly slower than the most frequent at the end, b/c of redundancies

            //Shift the history frame over 1 element
            history.erase(history.begin());
        }
        //Add to the history
        history.push_back(input->at(i));
    }


    //Get starting seed
    vector<int> seed = model.getLargestKey();

    seed.reserve(outputSize);

    //free the input memory
    delete input;

    //We're repurposing the history to do generation
    history = seed;

    //Seed the random number generator
    srand( time(NULL));

    cout << endl << endl << "Generating Final Output of size " << outputSize << endl;

    //Do the generation
    int zeroVectors = 0;
    int nonZeroVectors = 0;
    double avgSize = 0;
    for (int i = 0; i < outputSize; i++) {
        if (i % 1000 == 0) {
            //printf("\r History size %i, iteration %i",history.size(), i);
            printf("\r %i done (%i percent)",i,(int)(((double)i/(double)outputSize)*100));
            fflush(stdout);
        }
        //Get a random int from the options to follow this history set
        int addInt = 0;
        vector<int> *possibleFollowers = &model.get(seed,order);
        if (possibleFollowers->size() > 0) {
            addInt = possibleFollowers->at(rand() % possibleFollowers->size());
            avgSize += possibleFollowers->size();
            nonZeroVectors++;
        }
        else {
            zeroVectors++;
        }
        //Add it to the result
        seed.push_back(addInt);
        //Shift the history frame
    }
    avgSize /= (double)nonZeroVectors;
    printf("\n avg vec size %f, non-zero vectors %i, zero-vectors %i",avgSize,nonZeroVectors,zeroVectors);
    cout << endl;

    writeSamplesToWAV(&seed,outFile);
}

