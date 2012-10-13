#ifndef WAVIO_H
#define WAVIO_H

#include <vector>

// free it at the end
extern std::vector<int> *readSamplesFromWAV(const char *fname);

// returns true unless there was a problem writing to the file
extern bool writeSamplesToWAV(std::vector<int> *samples, const char *fname);

#endif