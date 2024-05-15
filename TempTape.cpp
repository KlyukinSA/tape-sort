#include "TempTape.hpp"

#include <utility>

TempTape::TempTape(FileTape&& tape)
    : tape(std::move(tape))
    , pos(0)
    , size(0)
    , finished(false) 
{}

bool TempTape::read(int& value) {
    if (isFull()) {
        value = 0;
        finished = true;
        return false;
    } else {
        // std::cout << "read temp "<< tape.isFinished() << ' ' << value << '\n';
        bool res = tape.read(value);
        // std::cout << "read temp "<< tape.isFinished() << ' ' << value << '\n';
        return res;
    }
}

bool TempTape::write(int value) {
    if (isFull()) {
        size++;
    }
    finished = false;
    return tape.write(value);
}

bool TempTape::rewind() {
    pos = 0;
    finished = false;
    return tape.rewind();
}

bool TempTape::shift(int n) {
    // std::cout << "(shift " << pos << ' ' << size << ')';
    if (n < 0) {
        finished = false;
    } else if (n > 0) {
        pos += n;
        if (pos > size) {
            n = size - (pos - n);
            finished = true;
            pos = size;
        }
    }
    return tape.shift(n);
}

bool TempTape::isFinished() {
    return finished;
}

bool TempTape::clean() {
    pos = 0;
    size = 0;
    finished = false;
    return tape.rewind();
}
