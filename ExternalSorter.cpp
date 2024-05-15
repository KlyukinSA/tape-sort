#include "ExternalSorter.hpp"

#include "TempTape.hpp"
#include "MergingTape.hpp"

#include <algorithm>
#include <vector>
#include <fstream>

ExternalSorter::ExternalSorter(int chunkSize):
    chunkSize(chunkSize) {
}

static std::string getTempTapeFileName(int id) {
    return "tmp/" + std::to_string(id) + ".txt";
}

std::vector<int> ExternalSorter::readChunk(TapeInterface& inputTape) {
    std::vector<int> res;
    for (int i = 0; i < chunkSize; i++) {
        int val = 0;
        inputTape.read(val);
        inputTape.shift(1);
        if (inputTape.isFinished()) {
            break;
        }
        res.push_back(val);
    }
    return res;
}

int mergeGroupToTape(std::vector<MergingTape>& group, TapeInterface& tape) {
    int remain = 0;
    for (std::size_t i = 0; i < group.size(); i++) {
        if (group[i].isIncreasing()) {
            remain++;
        }
    }
    while (true) {
        int min = 0;
        std::size_t pos = 0;
        bool wasIncrease = false;
        for (std::size_t i = 0; i < group.size(); i++) {
            if (group[i].isIncreasing()) {
                wasIncrease = true;
                min = group[i].getCurrentValue();
                pos = i;
                break;
            }
        }
        if (!wasIncrease) {
            break;
        }

        for (std::size_t i = 0; i < group.size(); i++) {
            int val = group[i].getCurrentValue();
            // if (i == 2) std::cout << val << ' ';
            if (group[i].isIncreasing() && val < min) {
                min = val;
                pos = i;
            }
        }

        MergingTape& winner = group[pos];
        tape.write(winner.getCurrentValue());
        tape.shift(1);

        winner.step();
    }
    // std::cout << "END GROUP\n";
    for (std::size_t i = 0; i < group.size(); i++) {
        group[i].startNewIncrease();
    }
    return remain;
}

void ExternalSorter::sort(TapeInterface& inputTape, TapeInterface& outputTape, int tapeGroupSize, const FileTapeConfig& config) {
    std::vector<TempTape> tempTapes;
    for (int i = 0; i < 2 * tapeGroupSize; i++) {
        tempTapes.push_back(TempTape{getTempTapeFileName(i), config, std::ios::trunc});
    }
    int i = 0;
    int j = 0;
    while (!inputTape.isFinished()) {
        std::vector<int> chunk = readChunk(inputTape);
        if (chunk.size() > 0) {
            std::sort(chunk.begin(), chunk.end());
            for (int val : chunk) {
                tempTapes[i].write(val);
                tempTapes[i].shift(1);
            }
            i++;
            if (i == tapeGroupSize) {
                i = 0;
            }
            j++;
        }
    }
    if (j == 0) {
        return;
    }

    bool forward = true;
    while (true) {
        std::vector<MergingTape> group;
        int srcShift = 0;
        if (!forward) {
            srcShift = tapeGroupSize;
        }
        for (int i = 0; i < tapeGroupSize; i++) {
            int srcTapeNumber = srcShift + i;
            // std::cout << "merge from " << srcTapeNumber << '\n';
            group.push_back(MergingTape{tempTapes[srcTapeNumber]});
        }

        i = 0;
        int j = 0;
        bool clean = true;
        while (true) {
            int shift = tapeGroupSize;
            if (!forward) {
                shift = 0;
            }
            int destTapeNumber = shift + i;
            // std::cout << "all ";
            if (clean) {
            // std::cout << "clean" << '\n';
                tempTapes[destTapeNumber].clean();
            }
            // std::cout << "merge to " << destTapeNumber << '\n';

            int remain = mergeGroupToTape(group, tempTapes[destTapeNumber]);

            if (j == 1 && remain == 0) {
                if (i == 0) {
                    i = tapeGroupSize;
                }
                int resultTapeNumber = shift + i - 1;
                tempTapes[resultTapeNumber].rewind();
                for (;;) {
                    int val = 0;
                    tempTapes[resultTapeNumber].read(val);
                    tempTapes[resultTapeNumber].shift(1);
                    if (tempTapes[resultTapeNumber].isFinished()) {
                        break;
                    }
                    outputTape.write(val);
                    outputTape.shift(1);
                }
                return;
            }
            j++;
            if (remain == 0) {
                break;
            }

            i++;
            if (i == tapeGroupSize) {
                i = 0;
                clean = false;
            }
        }
        // std::cout << j << '\n';
        forward = !forward;
    }
}
