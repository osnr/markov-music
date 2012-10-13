#include <sndfile.h>
#include "WAVio.h"


#define SAMPLES 8192 // how many samples to read at a time from the file
#define SAMPLE_RATE 44100 // for output WAV


std::vector<int> *readSamplesFromWAV(const char *fname) {
    std::vector<int> *samples = new std::vector<int>;
    
    SF_INFO fileInfo;
    fileInfo.format = 0;
    SNDFILE *fr = sf_open(fname, SFM_READ, &fileInfo);
    if (!fr) {
        printf("can't open %s\n", fname);
        throw;
    }

    int *input = (int *)malloc(sizeof(int) * fileInfo.channels * SAMPLES);
    int samplesRead = 0;
    bool soundStarted = false;

    while ((samplesRead = sf_readf_int(fr, input, SAMPLES)) != 0) {
        for (int i = 0; i < samplesRead; i++) {
            if (fileInfo.channels == 1) {
                samples->push_back(input[i]);
            } else if (fileInfo.channels == 2) {
                // push back an average of left/right channels (ie mono)
                int sampleToAdd = (int)((input[2*i] + input[2*i+1]) / 2.0);
                if (sampleToAdd == 0 && soundStarted || sampleToAdd != 0) {
                    soundStarted = true;
                    samples->push_back(sampleToAdd);
                }
            } else {
                printf("File has more than 2 audio channels\n");
                throw;
            }
        }
    }

    /*for (size_t i = 0; i < 100; i++) {
        printf("%i,", samples->at(i));
    }*/

    free(input);
    sf_close(fr);
    return samples;
}


bool writeSamplesToWAV(std::vector<int> *samples, const char *fname) {
    SF_INFO fileInfo;
    fileInfo.samplerate = SAMPLE_RATE;
    fileInfo.channels = 1;
    fileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE *fw = sf_open(fname, SFM_WRITE, &fileInfo);
    if (!fw) {
        printf("can't open %s for writing\n", fname);
        throw;
    }

    // the &samples->at(0) is a trick to basically convert the C++-style vector to a C-style array pointer
    sf_writef_int(fw, &samples->at(0), samples->size());
    sf_close(fw);
}
