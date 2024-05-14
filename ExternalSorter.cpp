#include "ExternalSorter.hpp"

#include "FileTape.hpp"

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
    FileTape tape;
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

int mergeGroupToTape(std::vector<MergingTape>& group, TapeInterface& tape) {
    // int val = 0;
    // group[0].tape.read(val);
    // std::cout << val << '\n';
    int remain = 0;
    for (std::size_t i = 0; i < group.size(); i++) {
        if (!group[i].tape.isFinished()) {
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
            if (!group[i].tape.isFinished()) {
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
            if (!group[i].tape.isFinished() && val >= group[i].prev && val < min) {
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
        // std::cout << "winner " << pos << '\n';
        // std::cout << "winner1 " << winner.prev << ' ' << winner.cur << '\n';

        int val = 0;
        winner.tape.read(val);
        winner.tape.shift(1);
        winner.prev = winner.cur;
        winner.cur = val;
        // std::cout << "winner2 " << winner.prev << ' ' << winner.cur << '\n';
    }
    // std::cout << "END GROUP\n";
    for (std::size_t i = 0; i < group.size(); i++) {
        group[i].prev = 0;
    }
    return remain;
}

void ExternalSorter::sort(TapeInterface& inputTape, TapeInterface& outputTape, int tapeGroupSize) {
    int tempTapeNumber = 0;
    std::ios_base::openmode mode = std::ios::trunc;
    int i = 0;
    while (!inputTape.isFinished()) {
        std::vector<int> chunk = readChunk(inputTape);
        if (chunk.size() > 0) {
            FileTape tempTape(getTempTapeFileName(tempTapeNumber), mode);
            std::sort(chunk.begin(), chunk.end());
            for (int val : chunk) {
                tempTape.write(val);
                tempTape.shift(1);
            }
            tempTapeNumber++;
            if (tempTapeNumber == tapeGroupSize) {
                tempTapeNumber = 0;
                mode = std::ios::ate;
            }
        }
        i++;
        // std::cout << chunk.size() << ' ';
    }
    if (i == 1) {
        return;
    }

    bool forward = true;
    bool wasTurn = false;
    while (true) {
        std::vector<MergingTape> group;
        for (int i = 0; i < tapeGroupSize; i++) {
            int shift = 0;
            if (!forward) {
                shift = tapeGroupSize;
            }
            group.push_back(MergingTape{FileTape(getTempTapeFileName(shift + i)), 0, 0});
            // std::cout << "merge from " << shift + i << '\n';
            int val = 0;
            group.back().tape.read(val);
            group.back().tape.shift(1);
            group.back().cur = val;
        }

        tempTapeNumber = 0;
        mode = std::ios::trunc;
        bool firstTime = true;
        int afterTurn = 0;
        while (true) {
            int shift = tapeGroupSize;
            if (!forward) {
                shift = 0;
            }
            FileTape tempTape(getTempTapeFileName(shift + tempTapeNumber), mode); // TODO store in vector too
            // std::cout << "merge to " << shift + tempTapeNumber << '\n';

            int remain = mergeGroupToTape(group, tempTape);

            // std::cout << afterTurn << ' ' << remain << '\n';// << group[0].tape.isFinished() << group[1].tape.isFinished() << '\n';
            if (afterTurn == 1 && remain == 0) {
                if (tempTapeNumber == 0) {
                    tempTapeNumber = tapeGroupSize;
                }
                FileTape res(getTempTapeFileName(shift + tempTapeNumber - 1));
                for (;;) {
                    int val = 0;
                    res.read(val);
                    res.shift(1);
                    if (res.isFinished()) {
                        break;
                    }
                    outputTape.write(val);
                    outputTape.shift(1);
                }
                return;
            }
            if (remain == 0) {
                break;
            }

            tempTapeNumber++;
            if (tempTapeNumber == tapeGroupSize) {
                tempTapeNumber = 0;
                mode = std::ios::ate;
            }
            firstTime = false;
            afterTurn++;
        }
        forward = !forward;
        wasTurn = true;
    }
}
