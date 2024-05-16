#ifndef FILE_TAPE_H
#define FILE_TAPE_H

#include "TapeInterface.hpp"
#include "FileTapeConfig.hpp"

#include <fstream>

class FileTape : public TapeInterface {
public:
    FileTape(std::fstream&& file, const FileTapeConfig& config);
    bool read(int& value) override;
    bool write(int value) override;
    bool rewind() override;
    bool shift(int n) override;
    bool isFinished() override;
private:
    bool step(int& v);

    std::fstream file;
    const FileTapeConfig& config;
};

#endif
