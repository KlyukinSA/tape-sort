#ifndef MERGING_TAPE_H
#define MERGING_TAPE_H

#include "TapeInterface.hpp"

class MergingTape {
public:
    MergingTape(TapeInterface& tape);
    bool isIncreasing();
    int getCurrentValue();
    void step();
    void startNewIncrease();
private:
    TapeInterface& tape;
    int prev;
    int cur;
};

#endif
