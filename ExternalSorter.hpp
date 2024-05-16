#ifndef EXTERNAL_SORTER_H
#define EXTERNAL_SORTER_H

#include "FileTapeConfig.hpp"
#include "TapeFactory.hpp"
#include "MergingTape.hpp"
#include "TempTape.hpp"

#include <vector>
#include <algorithm>

template<class T>
class ExternalSorter {
public:
    ExternalSorter(int chunkSize);
    void sort(TapeInterface& inputTape, TapeInterface& outputTape, int tapeGroupSize, TapeFactory<T>& factory);
private:
    std::vector<int> readChunk(TapeInterface& inputTape);
    int mergeGroupToTape(std::vector<MergingTape>& group, TapeInterface& tape);
    void writeResult(TapeInterface& resultTape, TapeInterface& outputTape);

    int chunkSize;
};

template<class T>
ExternalSorter<T>::ExternalSorter(int chunkSize)
    : chunkSize(chunkSize)
{}

template<class T>
std::vector<int> ExternalSorter<T>::readChunk(TapeInterface& inputTape) {
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

template<class T>
int ExternalSorter<T>::mergeGroupToTape(std::vector<MergingTape>& group, TapeInterface& tape) {
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
    for (std::size_t i = 0; i < group.size(); i++) {
        group[i].startNewIncrease();
    }
    return remain;
}

template<class T>
void ExternalSorter<T>::writeResult(TapeInterface& resultTape, TapeInterface& outputTape) {
    resultTape.rewind();
    for (;;) {
        int val = 0;
        resultTape.read(val);
        resultTape.shift(1);
        if (resultTape.isFinished()) {
            break;
        }
        outputTape.write(val);
        outputTape.shift(1);
    }
}

template<class T>
void ExternalSorter<T>::sort(TapeInterface& inputTape, TapeInterface& outputTape, int tapeGroupSize, TapeFactory<T>& factory) {
    std::vector<TempTape<T>> tempTapes;
    for (int i = 0; i < 2 * tapeGroupSize; i++) {
        tempTapes.push_back(TempTape{factory.create()});
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
            if (clean) {
                tempTapes[destTapeNumber].clean();
            }

            int remain = mergeGroupToTape(group, tempTapes[destTapeNumber]);

            if (j == 1 && remain == 0) {
                if (i == 0) {
                    i = tapeGroupSize;
                }
                writeResult(tempTapes[shift + i - 1], outputTape);
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
        forward = !forward;
    }
}

#endif
