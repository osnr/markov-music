#ifndef WAVIO_H
#define WAVIO_H

#include <vector>

// free it at the end
extern std::vector<int> *readSamplesFromWAV(const char *fname);

#endif