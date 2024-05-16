#ifndef FILE_TAPE_FACTORY_H
#define FILE_TAPE_FACTORY_H

#include "TapeFactory.hpp"
#include "FileTape.hpp"

class FileTapeFactory : public TapeFactory<FileTape> {
public:
    FileTapeFactory(const FileTapeConfig& config, const std::string& directory);
    FileTape create();
private:
    std::string getTempTapeFileName(int id);

    int i;
    const FileTapeConfig& config;
    const std::string& directory;
};

#endif
