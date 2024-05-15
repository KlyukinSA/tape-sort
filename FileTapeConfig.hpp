#ifndef FILE_TAPE_CONFIG_H
#define FILE_TAPE_CONFIG_H

#include <fstream>

struct FileTapeConfig {
    int writeDelay;
    int readDelay;
    int shiftDelay;
    int rewindDelay;
};

FileTapeConfig readFileTapeConfigFromFile(std::ifstream& file);

#endif
