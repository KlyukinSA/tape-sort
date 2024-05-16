#ifndef TEMP_TAPE_H
#define TEMP_TAPE_H

#include "TapeInterface.hpp"

#include <utility>

template<class T>
class TempTape : public TapeInterface {
private:
    bool isFull();

    T tape;
    int pos;
    int size;
    bool finished;
public:
    TempTape(T&& tape);
    bool read(int& value);
    bool write(int value);
    bool rewind();
    bool shift(int n);
    bool isFinished();
    bool clean();
};

template<class T>
bool TempTape<T>::isFull() {
    return pos == size;
}

template<class T>
TempTape<T>::TempTape(T&& tape)
    : tape(std::move(tape))
    , pos(0)
    , size(0)
    , finished(false) 
{}

template<class T>
bool TempTape<T>::read(int& value) {
    if (isFull()) {
        value = 0;
        finished = true;
        return false;
    } else {
        bool res = tape.read(value);
        return res;
    }
}

template<class T>
bool TempTape<T>::write(int value) {
    if (isFull()) {
        size++;
    }
    finished = false;
    return tape.write(value);
}

template<class T>
bool TempTape<T>::rewind() {
    pos = 0;
    finished = false;
    return tape.rewind();
}

template<class T>
bool TempTape<T>::shift(int n) {
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

template<class T>
bool TempTape<T>::isFinished() {
    return finished;
}

template<class T>
bool TempTape<T>::clean() {
    pos = 0;
    size = 0;
    finished = false;
    return tape.rewind();
}

#endif
