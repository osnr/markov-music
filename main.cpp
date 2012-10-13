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
        vector<int> input;
        for (int i = 1; i < argc; i++) {
            readSamplesFromWAV(input, argv[i]);
        }
        markovGeneration(input,"out.wav",4,250000,1000000);
    }

}
