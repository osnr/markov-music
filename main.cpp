#include "WAVio.h"
#include "markov.h"
#include "mapMarkov.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Usage: %s WAV1 ... WAVN\n\tWAVX are WAV files to read from.\n", argv[0]);
        return 1;
    } else if (argc == 3) {
        markovGeneration(argv[1],argv[2],1000,50000,90000);
    } else {
        cout << "Hello Hackathon!" << endl;
        return 0;
    }

}
