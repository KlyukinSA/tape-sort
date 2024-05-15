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
    TempTape(std::string fileName, const FileTapeConfig& config, std::ios_base::openmode mode = std::ios::out);
    bool read(int& value);
    bool write(int value);
    bool rewind();
    bool shift(int n);
    bool isFinished();
    bool clean();
};

#endif
