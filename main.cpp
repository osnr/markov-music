#include "WAVio.h"
#include "markov.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  markov();

    if (argc == 1) {
        printf("Usage: %s WAV1 ... WAVN\n\tWAVX are WAV files to read from.\n", argv[0]);
        return 1;
    } else {
        cout << "Hello Hackathon!" << endl;
        return 0;
    }

}
