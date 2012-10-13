#include "WAVio.h"

#include "mapMarkov.h"
#include "magicMap.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    if (argc == 1) {
        cout << "Usage: " << argv[0] << " insize outsize strictness order unify WAV1 ... WAVN" << endl;

        cout << "\tinsize is the number of words from the start of each file on which to train the model." << endl;
        cout << "\toutsize is the number of words to generate in the output." << endl;
        cout << "\tstrictness determines the strictness of sequence matching (higher -> stricter)." << endl;
        cout << "\torder is the number of words used to predict the next word in the model." << endl;
        cout << "\tunify (0 or 1) determines whether to build a single model or switch between per-song models." << endl;
        cout << "\tWAV1 ... WAVN are WAV files to read from." << endl;
        
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
