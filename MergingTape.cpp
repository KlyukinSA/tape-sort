#include "MergingTape.hpp"

#include <limits>

MergingTape::MergingTape(TapeInterface& tape)
    : tape(tape)
    , cur(std::numeric_limits<int>::min())
{
    tape.rewind();
    step();
}

bool MergingTape::isIncreasing() {
    return !tape.isFinished() && cur >= prev;
}

int MergingTape::getCurrentValue() {
    return cur;
}

void MergingTape::step() {
    int val = 0;
    tape.read(val);
    tape.shift(1);
    prev = cur;
    cur = val;
}

void MergingTape::startNewIncrease() {
    prev = std::numeric_limits<int>::min();
}
