#ifndef WAVIO_H
#define WAVIO_H

#include <vector>
#include "magicMap.h"

// free it at the end
extern void readSamplesFromWAV(std::vector<slice> &samples, const char *fname);

// returns true unless there was a problem writing to the file
extern bool writeSamplesToWAV(std::vector<int> *samples, const char *fname);

#endif
