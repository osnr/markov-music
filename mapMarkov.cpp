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

MagicMap buildModel(vector<slice> input, int order, int inputSizeLimit, int fuzzMultiple, const char *inputName, int index) {
    
    int inputSize = input.size();
    if (inputSize > inputSizeLimit) inputSize = inputSizeLimit;

    //Do beat calculations
    vector<double> beathistory;
    int onBeat = 0;
    int lastBeatLocation = 44100/1024;
    vector<int> beatLength;
    vector<int> beatPosition;
    for (int s = 0; s < inputSize/1024; s++) {
        double sampleEnergy = 0;
        for (int i = s*1024; i < (s+1)*1024; i++) {
            sampleEnergy += (double)input[i].value * (double)input[i].value;
        }
        beathistory.push_back(sampleEnergy);
        //if we have a full history buffer
        if (beathistory.size() > 44100/1024) {
            double avgLocalEnergy = 0;
            for (int i = 0; i < beathistory.size(); i++) {
                avgLocalEnergy += beathistory[i];
            }
            avgLocalEnergy /= beathistory.size();
            if (sampleEnergy > avgLocalEnergy * 1.3) {
                //We found a beat
                onBeat++;
            }
            else if (onBeat > 0) {
                if (onBeat > 1) {
                    int length = s - lastBeatLocation - onBeat;
                    //if (length > 4) {
                        //printf("BEAT OF SIZE %i, %i since last, @%i\n",onBeat,length,(s-onBeat)*1024);
                        lastBeatLocation = s - onBeat;
                        beatLength.push_back(length);
                        beatPosition.push_back((s-onBeat)*1024);
                    //}
                }
                onBeat = 0;
            }
            /*printf("BEAT SCORE AT 1024*%i: ",s);
            for (int i = 0; i < onBeat; i++) {
                printf(" - ");
            }
            printf("*\n");
            for (int i = s*1024; i < (s+1)*1024; i++) {
                
            }*/
            beathistory.erase(beathistory.begin());
        }
    }
    double beatLengthAvg = 0;
    for (int i = 0; i < beatLength.size(); i++) {
        beatLengthAvg += beatLength[i];
    }
    beatLengthAvg /= beatLength.size();
    int beatLengthAvgInt = beatLengthAvg*1024;
    printf("The Average BEATLENGTH is %i\n",beatLengthAvgInt);

    MagicMap model(0,fuzzMultiple);
    vector<slice> history;


    //if (!model.ReadFromFile(MODEL_FILE, inputName)) {
        cout << "Building Model of size " << inputSize << ": " << endl;
        //Build the model
        int beatPositionIndex = 0;
        int beatBumpSize = 0;
        for (int i = 0; i < inputSize; i++) {
            if (beatPosition[beatPositionIndex] == i) {
                //printf("Writing over a beat @%i\n",i);
                beatPositionIndex++;
                beatBumpSize = beatLengthAvgInt;
            }
            beatBumpSize--;
            if (i % 1000 == 0) {
                printf("\r %i done (%i percent) beat: ",i,(int)(((double)i/(double)inputSize)*100));
                for (int i = 0; i < beatLengthAvgInt / 500; i++) {
                    if (i < beatBumpSize / 500) {
                        printf("+");
                    }
                    else {
                        printf("-");
                    }
                }
                fflush(stdout);
            }
            input.at(i).beatFalloff = beatBumpSize;
            if (history.size() >= order) {
                model[history].push_back(input.at(i));
                
                //Shift the history frame over 1 element
                history.erase(history.begin());
            }
            //Add to the history
            history.push_back(input.at(i));
        }
        model.SaveToFile(MODEL_FILE, inputName);
    //}
    return model;
}

void markovGeneration(vector< vector<slice> > &inputs, char* outFile, int order, int inputSizeLimit, int outputSize, int fuzzMultiple, int beatLength, int beatModifierStrength, bool unify) {

    vector<MagicMap> models;
    int modelIndex = 0;

    MagicMap model(0,fuzzMultiple);

    if (!unify) {
        for (int i = 0; i < inputs.size(); i++) {
            models.push_back(buildModel(inputs[i],order,inputSizeLimit,fuzzMultiple,i));
        }

        model = models[modelIndex];
    }
    else {
        vector<slice> unified;
        for (int i = 0; i < inputs.size(); i++) {
            int inputSize = inputs[i].size();
            if (inputSize > inputSizeLimit) inputSize = inputSizeLimit;
            for (int j = 0; j < inputSize; j++) {
                unified.push_back(inputs[i][j]);
            }
        }
        model = buildModel(unified,order,unified.size(),fuzzMultiple,0);
    }
    //Get starting seed
    vector<slice> startFlag = model.getLargestKey();
    vector<slice> seed = startFlag;
    printf("\nStarting with set: ");
    for (int i = 0; i < seed.size(); i++) {
        printf("%i,",seed[i].value);
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
        slice addSlice;
        vector<slice> *possibleFollowers = &model.get(seed,order);
        if (possibleFollowers->size() > 0) {
            addSlice = possibleFollowers->at(rand() % possibleFollowers->size());
            avgSize += possibleFollowers->size();
            nonZeroVectors++;
            //Add our result
            seed.push_back(addSlice);
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
        seed[i].value += beatModifier;
    }
    printf("DONE POST PROCESSING\n");
    avgSize /= (double)nonZeroVectors;
    printf("\n avg vec size %f, non-zero vectors %i, zero-vectors %i",avgSize,nonZeroVectors,zeroVectors);
    cout << endl;

    vector<int> seedJustInts;

    for (int i = 0; i < outputSize; i++) {
        seedJustInts.push_back(seed[i].value);
    }

    writeSamplesToWAV(&seedJustInts,outFile);
}

