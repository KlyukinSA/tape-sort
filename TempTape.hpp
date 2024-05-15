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
    TempTape(FileTape&& tape);
    bool read(int& value);
    bool write(int value);
    bool rewind();
    bool shift(int n);
    bool isFinished();
    bool clean();
};

#endif
