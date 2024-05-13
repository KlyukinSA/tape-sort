#include "ExternalSorter.hpp"

#include <algorithm>
#include <vector>

void ExternalSorter::sort(TapeInterface& inputTape, TapeInterface& outputTape, int chunkSize, int memoryLimit) {
    std::vector<int> input;
    while (true) {
        int i = 0;
        inputTape.read(i);
        inputTape.shift(1);
        if (inputTape.isFinished()) {
            break;
        }
        input.push_back(i);
    }

    std::sort(input.begin(), input.end()); 

    for (int i : input) {
        // std::cout << i << ' ';
        outputTape.write(i); 
        outputTape.shift(1); 
    }
}
