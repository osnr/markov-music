#ifndef READWAV_H
#define READWAV_H

#include <vector>

// free it at the end
extern std::vector<int> *readSamplesFromWAV(const char *fname);

#endif