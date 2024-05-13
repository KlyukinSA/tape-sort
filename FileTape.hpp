#ifndef FILE_TAPE_H
#define FILE_TAPE_H

#include "TapeInterface.hpp"

#include <fstream>

class FileTape : public TapeInterface {
private:
    std::fstream& file;
public:
    FileTape(std::fstream& fileStream);
    bool read(int& value) override;
    bool write(int value) override;
    bool rewind() override;
    bool shift(int n) override;
    bool isFinished() override;
};

#endif
