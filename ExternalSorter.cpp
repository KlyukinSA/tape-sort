#include "ExternalSorter.hpp"

#include "TempTape.hpp"

#include <algorithm>
#include <vector>
#include <fstream>

// class MergingTape {
// public:
//     MergingTape(std::string fileName):
//         tape(FileTape(fileName)) {}
//     FileTape tape;
// };
struct MergingTape {
    int prev;
    int cur;
};

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

int mergeGroupToTape(std::vector<TempTape>::iterator tempTapes, std::vector<MergingTape>& group, TapeInterface& tape) {
    // int val = 0;
    // group[0].tape.read(val);
    // std::cout << val << '\n';
    int remain = 0;
    for (std::size_t i = 0; i < group.size(); i++) {
        if (!tempTapes[i].isFinished()) {
            remain++;
        }
    }
    // if (remain == 0) {
    //     return true;
    // }
    while (true) {
        int min = 0;
        std::size_t pos = 0;
        bool wasIncrease = false;
        // remain = 0;
        for (std::size_t i = 0; i < group.size(); i++) {
            if (!tempTapes[i].isFinished()) {
                // remain++;
                // std::cout << "remain: " << remain << " i "  << i << '\n';
                if (group[i].cur >= group[i].prev) {
                    wasIncrease = true;
                    min = group[i].cur;
                    pos = i;
                    break;
                }
            }
        }
        // if (remain == 0) {
        //     return true;
        // }
        if (!wasIncrease) {
            break;
        }

        for (std::size_t i = 0; i < group.size(); i++) {
            int val = group[i].cur;
            // if (i == 2) std::cout << val << ' ';
            if (!tempTapes[i].isFinished() && val >= group[i].prev && val < min) {
                min = val;
                pos = i;
            }
        }

        MergingTape& winner = group[pos];
        tape.write(winner.cur);
        tape.shift(1);

        // std::cout << "group curs: ";
        // for (int i = 0; i < group.size(); i++) {
        //     std::cout << group[i].cur << ' ';
        // }

        // std::cout << "winner " << pos << ' ' << winner.prev << ' ' << winner.cur << ' ' << tempTapes[pos].isFinished() << ' ';

        // bool is = tempTapes[pos].isFinished();

        int val = 0;
        tempTapes[pos].read(val);
        tempTapes[pos].shift(1);
        winner.prev = winner.cur;
        winner.cur = val;

        // if (!is && tempTapes[pos].isFinished()) {
        //     std::cout << pos << ' ' << winner.prev << ' ' << winner.cur << '\n';
        // } 

        // std::cout << "winner " << pos << ' ' << winner.prev << ' ' << winner.cur << ' ' << tempTapes[pos].isFinished() << '\n';
    }
    // std::cout << "END GROUP\n";
    for (std::size_t i = 0; i < group.size(); i++) {
        group[i].prev = 0;
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
            tempTapes[srcTapeNumber].rewind();
            int val = 0;
            tempTapes[srcTapeNumber].read(val);
            // std::cout << srcTapeNumber << ' ' << val << ' ' << tempTapes[srcTapeNumber].isFinished() << '\n';
            tempTapes[srcTapeNumber].shift(1);
            group.push_back(MergingTape{0, val});
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

            // if (destTapeNumber == 1) {
            //     for (int i = 0; i < tapeGroupSize; i++) {
            //         int srcTapeNumber = srcShift + i;
            //         std::cout << group[srcTapeNumber].cur << group[srcTapeNumber].prev << srcTapeNumber << tempTapes[srcTapeNumber].isFinished() << '\n';
            //     }
            //     // return;
            // }
            int remain = mergeGroupToTape(tempTapes.begin() + srcShift, group, tempTapes[destTapeNumber]);

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
