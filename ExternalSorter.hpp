#ifndef EXTERNAL_SORTER_H
#define EXTERNAL_SORTER_H

#include "TapeInterface.hpp"
#include "FileTapeConfig.hpp"

#include <vector>
#include <algorithm>

class ExternalSorter {
public:
    ExternalSorter(int chunkSize);
    void sort(TapeInterface& inputTape, TapeInterface& outputTape, int tapeGroupSize, const FileTapeConfig& config);
private:
    std::vector<int> readChunk(TapeInterface& inputTape);
    int chunkSize;
};

#endif
