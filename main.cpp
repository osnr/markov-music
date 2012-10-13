#include "WAVio.h"
#include "markov.h"
#include "mapMarkov.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Usage: %s WAV1 ... WAVN\n\tWAVX are WAV files to read from.\n", argv[0]);
        return 1;
    } else {
        vector<vector<int> > inputs;
        for (int i = 1; i < argc; i++) {
            vector<int> v;
            readSamplesFromWAV(v, argv[i]);
            inputs.push_back(v);
        }
        markovGeneration(inputs,"out.wav",3,250000,10000000);
    }

}
