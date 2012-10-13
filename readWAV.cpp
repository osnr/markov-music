#include <sndfile.h>
#include "readWAV.h"


#define SAMPLES 8192 // how many samples to read at a time from the file


std::vector<int> *readSamplesFromWAV(const char *fname) {
    std::vector<int> *samples = new std::vector<int>;
    
    SF_INFO fileInfo;
    fileInfo.format = 0;
    SNDFILE *fr = sf_open(fname, SFM_READ, &fileInfo);
    if (!fr) {
        printf("can't open %s\n", fname);
        throw;
    }

    float *input = (float *)malloc(sizeof(float) * fileInfo.channels * SAMPLES);
    int samplesRead = 0;
    while ((samplesRead = sf_readf_float(fr, input, SAMPLES)) != 0) {
        for (int i = 0; i < samplesRead; i++) {
            if (fileInfo.channels == 1) {
                samples->push_back(input[i]);
            } else if (fileInfo.channels == 2) {
                // push back an average of left/right channels (ie mono)
                samples->push_back((input[2*i] + input[2*i+1]) / 2.0);
            } else {
                printf("File has more than 2 audio channels\n");
                throw;
            }
        }
    }

    free(input);
    return samples;
}
