#include "WAVio.h"
#include "markov.h"
#include "mapMarkov.h"
#include "magicMap.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Usage: %s WAV1 ... WAVN\n\tWAVX are WAV files to read from.\n", argv[0]);
        return 1;
    } else {
        vector<vector<slice> > inputs;
        vector<char *> inputNames;
        int inSize = atoi(argv[1]);
        int outSize = atoi(argv[2]);
        int fuzzFactor = atoi(argv[3]);
        int order = atoi(argv[4]);
        int unified = atoi(argv[5]);
        cout << argc << endl;
        for (int i = 6; i < argc; i++) {
            vector<slice> v;
            readSamplesFromWAV(v, argv[i]);
            inputs.push_back(v);
            inputNames.push_back(argv[i]);
        }

        //order, input file size, output file size, fuzz factor, unify inputs into single graph
        markovGeneration(inputs,inputNames,"out.wav",order,inSize,outSize,INT_MAX/fuzzFactor,22000,0,unified==1);
    }
}
