#include "WAVio.h"
#include "magicMap.h"
#include "mapMarkov.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define MODEL_FILE "SavedMagicModel"


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

MagicMap buildModel(vector<int> input, int order, int inputSizeLimit, int fuzzMultiple, const char *inputName) {
    MagicMap model(0,fuzzMultiple);
    vector<int> history;

    int inputSize = input.size();
    if (inputSize > inputSizeLimit) inputSize = inputSizeLimit;

    if (!model.ReadFromFile(MODEL_FILE, inputName)) {
        cout << "Building Model of size " << inputSize << ": " << endl;
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
        model.SaveToFile(MODEL_FILE, inputName);
    }
    return model;
}

void markovGeneration(vector< vector<int> > &inputs, vector<char *> &inputNames, char* outFile, int order, int inputSizeLimit, int outputSize, int fuzzMultiple, int beatLength, int beatModifierStrength, bool unify) {

    vector<MagicMap> models;
    int modelIndex = 0;

    MagicMap model(0,fuzzMultiple);

    if (!unify) {
        for (int i = 0; i < inputs.size(); i++) {
            models.push_back(buildModel(inputs[i],order,inputSizeLimit,fuzzMultiple,inputNames[i]));
        }

        model = models[modelIndex];
    }
    else {
        vector<int> unified;
        for (int i = 0; i < inputs.size(); i++) {
            int inputSize = inputs[i].size();
            if (inputSize > inputSizeLimit) inputSize = inputSizeLimit;
            for (int j = 0; j < inputSize; j++) {
                unified.push_back(inputs[i][j]);
            }
        }
        int unifiedLen = 0;
        for (size_t i = 0; i < inputNames.size(); i++) {
            unifiedLen += strlen(inputNames[i]);
        }
        unifiedLen++;
        char *unifiedStr = (char *)malloc(unifiedLen);
        unifiedStr[0] = 0;
        for (size_t i = 0; i < inputNames.size(); i++) {
            strcat(unifiedStr, inputNames[i]);
        }
        model = buildModel(unified,order,unified.size(),fuzzMultiple,unifiedStr);
        free(unifiedStr);
    }
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

    int metronome = 0;
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
            if (!unify) {
                modelIndex = rand() % models.size();
                printf("\nRAN OUT OF POSSIBILITIES, new song %i\n",modelIndex);
                model = models[modelIndex];
                startFlag = model.getMostSimilarKey(seed,order);
            }
            else {
                printf("\nRAN OUT OF POSSIBILITIES\n");
                startFlag = model.getMostSimilarKey(seed,order);
            }
            for (int i = 0; i < startFlag.size(); i++) {
                seed.push_back(startFlag[i]);
            }
        }
    }
    printf("\nPOST PROCESSING\n");
    for (int i = 0; i < outputSize; i++) {
        if (i % 1000 == 0) {
            //printf("\r History size %i, iteration %i",history.size(), i);
            printf("\r %i done (%i percent)",i,(int)(((double)i/(double)outputSize)*100));
            fflush(stdout);
        }
        metronome++;
        int beatModifier = metronome;
        if (metronome > beatLength) metronome = beatLength - metronome;
        if (metronome > beatLength * 2) metronome = 0;
        seed[i] += beatModifier;
    }
    printf("DONE POST PROCESSING\n");
    avgSize /= (double)nonZeroVectors;
    printf("\n avg vec size %f, non-zero vectors %i, zero-vectors %i",avgSize,nonZeroVectors,zeroVectors);
    cout << endl;

    writeSamplesToWAV(&seed,outFile);
}

