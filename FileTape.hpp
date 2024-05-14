#ifndef FILE_TAPE_H
#define FILE_TAPE_H

#include "TapeInterface.hpp"

#include <fstream>

class FileTape : public TapeInterface {
private:
    std::fstream file;
public:
    FileTape(std::string fileName, std::ios_base::openmode mode = std::ios::out);
    bool read(int& value) override;
    bool write(int value) override;
    bool rewind() override;
    bool shift(int n) override;
    bool isFinished() override;
};

#endif
