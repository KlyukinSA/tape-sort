#ifndef EXTERNAL_SORTER_H
#define EXTERNAL_SORTER_H

#include "TapeInterface.hpp"

#include <vector>
#include <algorithm>

#include <iostream>

class ExternalSorter {
public:
    void sort(TapeInterface& inputTape, TapeInterface& outputTape, int chunkSize, int memoryLimit);
};

#endif
