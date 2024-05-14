#ifndef EXTERNAL_SORTER_H
#define EXTERNAL_SORTER_H

#include "TapeInterface.hpp"

#include <vector>
#include <algorithm>

#include <iostream>

class ExternalSorter {
public:
    ExternalSorter(int chunkSize);
    void sort(TapeInterface& inputTape, TapeInterface& outputTape, int tapeGroupSize);
private:
    std::vector<int> readChunk(TapeInterface& inputTape);
    int chunkSize;
};

#endif
