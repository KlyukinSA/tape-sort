#ifndef TEMP_TAPE_H
#define TEMP_TAPE_H

#include "FileTape.hpp"

class TempTape : public TapeInterface {
private:
    FileTape tape;
    int pos;
    int size;
    bool finished;
    bool isFull() {
        return pos == size;
    }
public:
    TempTape(std::string fileName, std::ios_base::openmode mode = std::ios::out)
        : tape(fileName, mode)
        , pos(0)
        , size(0)
        , finished(false) 
    {}
    bool read(int& value) {
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
    bool write(int value) {
        if (isFull()) {
            size++;
        }
        finished = false;
        return tape.write(value);
    }
    bool rewind() {
        pos = 0;
        finished = false;
        return tape.rewind();
    }
    bool shift(int n) {
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
    bool isFinished() {
        return finished;
    }
    bool clean() {
        pos = 0;
        size = 0;
        finished = false;
        return tape.rewind();
    }
};

#endif
