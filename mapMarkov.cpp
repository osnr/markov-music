#include "WAVio.h"
#include "magicMap.h"
#include "mapMarkov.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define MODEL_FILE "magicModel.txt"


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

void printResults(vector<int> *output) {
    for (int i = 0; i < output->size(); i++) {
        printf("%i,",output->at(i));
    }
    printf("\n");
}

MagicMap buildModel(vector<int> input, int order, int inputSizeLimit) {
    MagicMap model(0);
    vector<int> history;

    int inputSize = input.size();
    if (inputSize > inputSizeLimit) inputSize = inputSizeLimit;

    if (!model.ReadFromFile(MODEL_FILE)) {
        cout << "\nBuilding Model of size " << inputSize << ": " << endl;
        //Build the model
        for (int i = 0; i < inputSize; i++) {
            if (i % 1000 == 0) {
                printf("\r %i done (%i percent)",i,(int)(((double)i/(double)inputSize)*100));
                fflush(stdout);
            }
            if (history.size() >= order) {
                model[history].push_back(input.at(i));
                //Check for the seed value
                //TODO: This is slightly slower than the most frequent at the end, b/c of redundancies
                
                //Shift the history frame over 1 element
                history.erase(history.begin());
            }
            //Add to the history
            history.push_back(input.at(i));
        }
        model.SaveToFile(MODEL_FILE);
    }
    return model;
}

void markovGeneration(vector< vector<int> > &inputs, char* outFile, int order, int inputSizeLimit, int outputSize) {
    vector<MagicMap> models;
    
    for (int i = 0; i < inputs.size(); i++) {
        models.push_back(buildModel(inputs[i],order,inputSizeLimit));
    }

    int modelIndex = 0;
    MagicMap model = models[modelIndex];
    //Get starting seed
    vector<int> startFlag = model.getLargestKey();
    vector<int> seed = startFlag;
    printf("\nStarting with set: ");
    for (int i = 0; i < seed.size(); i++) {
        printf("%i,",seed[i]);
    }

    //Seed the random number generator
    srand( time(NULL));

    cout << endl << endl << "Generating Final Output of size " << outputSize << endl;

    //model.debugModel();

    //Do the generation
    int zeroVectors = 0;
    int nonZeroVectors = 0;
    double avgSize = 0;
    fflush(stdout);

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
            //Add our result
            seed.push_back(addInt);
        }
        else {
            //Start over if we hit a loop
            /*modelIndex++;
            if (modelIndex >= models.size()) {
                modelIndex = 0;
            }*/
            modelIndex = rand() % models.size();
            printf("\nRAN OUT OF POSSIBILITIES, new song %i\n",modelIndex);
            model = models[modelIndex];
            //startFlag = model.getLargestKey();
            startFlag = model.getMostSimilarKey(seed,order);
            for (int i = 0; i < startFlag.size(); i++) {
                seed.push_back(startFlag[i]);
            }
        }
    }
    avgSize /= (double)nonZeroVectors;
    printf("\n avg vec size %f, non-zero vectors %i, zero-vectors %i",avgSize,nonZeroVectors,zeroVectors);
    cout << endl;

    //printResults(&seed);
    writeSamplesToWAV(&seed,outFile);
}

